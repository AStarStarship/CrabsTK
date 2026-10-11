#!/usr/bin/env python3
# Copyright AStarship <https://astarship.net>.
"""Run with python3 _Seams/Data/VerifyTest.py; no compiler needed."""
import ast
import hashlib
from pathlib import Path
import struct
import unittest
import zlib

VERIFIER = Path(__file__).with_name('Verify.py')


class MemoryFixture:
    def __init__(self, data):
        self.data = bytes(data)

    def read_bytes(self):
        return self.data

    def __str__(self):
        return 'in-memory rollout fixture'


def fixture_bytes():
    packets = []
    for sequence, episodes in [(88, [111, 222]), (89, [333])]:
        payload = b''.join(
            struct.pack('<QiQ2fifB2B2f2I', episode, i, i + 5,
                        float(i + 1), float(-i - 1), i, float(i) + 0.25,
                        i, 1, 1, 0.5, -0.5, i + 7, 0xffffffff)
            for i, episode in enumerate(episodes))
        packet = bytearray(struct.pack('<8s8I3Q2I', b'CRBROLL1', 1, 72,
                                      len(payload), len(episodes), 2, 2, 2,
                                      0, 73, sequence, 17, 0, 0) + payload)
        struct.pack_into('<I', packet, 64,
                         zlib.crc32(packet[:64] + packet[68:]))
        packets.append(packet)
    return b''.join(packets)


def load_verifier(optimization):
    namespace: dict = {'__name__': 'verify_regression', '__file__': str(VERIFIER)}
    exec(compile(VERIFIER.read_text(), str(VERIFIER), 'exec',
                 optimize=optimization), namespace)
    return namespace


class VerifyOptimizationTest(unittest.TestCase):
    def test_crc_corruption_rejected_at_every_optimization(self):
        valid = fixture_bytes()
        self.assertEqual(len(valid), 309)
        self.assertEqual(hashlib.sha256(valid).hexdigest(),
                         'b3db6576ceecdd3e0a2420eb9d865ccfe56f895abf93274f3771aa11d4831acd')
        corrupt = bytearray(valid)
        corrupt[72] ^= 1  # Reproduce the payload flip without repairing its CRC.
        for optimization in (0, 1, 2):
            with self.subTest(optimization=optimization):
                decode = load_verifier(optimization)['decode_fixture']
                # Positive control is outside the invalid-input exception scope.
                decoded = decode(MemoryFixture(valid))
                self.assertEqual(decoded['records'], 2)
                self.assertEqual(decoded['transitions'], 3)
                self.assertEqual(decoded['sequences'], [88, 89])
                with self.assertRaises((AssertionError, RuntimeError)):
                    decode(MemoryFixture(corrupt))

    def test_wire_fields_rejected_with_repaired_crc(self):
        valid = fixture_bytes()
        first_end = 72 + struct.unpack_from('<I', valid, 16)[0]
        mutations = [('magic', '<8s', 0, b'INVALID1'),
                     ('version', '<I', 8, 2), ('count', '<I', 20, 0),
                     ('observations', '<I', 24, 0),
                     ('actions', '<I', 28, 0), ('states', '<I', 32, 1),
                     ('reserved', '<I', 36, 1), ('tail', '<I', 68, 1),
                     ('run_id', '<Q', 40, 74), ('sequence', '<Q', 48, 90),
                     ('seed', '<Q', 56, 18), ('episode', '<Q', 72, 110)]
        for optimization in (0, 1, 2):
            decode = load_verifier(optimization)['decode_fixture']
            for name, fmt, offset, value in mutations:
                with self.subTest(optimization=optimization, field=name):
                    corrupt = bytearray(valid)
                    struct.pack_into(fmt, corrupt, offset, value)
                    struct.pack_into('<I', corrupt, 64,
                                     zlib.crc32(corrupt[:64] +
                                                corrupt[68:first_end]))
                    with self.assertRaises((AssertionError, RuntimeError)):
                        decode(MemoryFixture(corrupt))

    def test_verification_has_no_removable_assertions(self):
        tree = ast.parse(VERIFIER.read_text())
        lines = [node.lineno for node in ast.walk(tree)
                 if isinstance(node, ast.Assert)]
        self.assertEqual(lines, [],
                         'Wire, field, audit and demo checks must survive -O/-OO')


if __name__ == '__main__':
    unittest.main()

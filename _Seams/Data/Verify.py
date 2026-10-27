#!/usr/bin/env python3
# Copyright AStarship <https://astarship.net>.
"""Build/exercise the opt-in Data module; independently decode its real file."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import shutil
import struct
import subprocess
import tempfile
import zlib

ROOT = Path(__file__).resolve().parents[2]


def require(condition, message):
    """Keep verification checks active under optimized Python."""
    if not condition:
        raise RuntimeError(message)


def decode_fixture(path):
    data = path.read_bytes()
    offset = 0
    records = []
    header_format = '<8s8I3Q2I'
    require(struct.calcsize(header_format) == 72, 'Unexpected wire header size')
    while offset < len(data):
        header = struct.unpack_from(header_format, data, offset)
        (magic, version, header_bytes, payload, count, obs, actions, states,
         reserved, run_id, sequence, seed, crc, tail) = header
        require(magic == b'CRBROLL1' and version == 1 and header_bytes == 72,
                'Invalid wire magic, version or header size')
        require(reserved == tail == 0 and 1 <= count <= 4096,
                'Invalid reserved fields or transition count')
        require(1 <= obs <= 4096 and 1 <= actions <= 4096 and states <= 4096,
                'Invalid observation, action or state dimensions')
        end = offset + header_bytes + payload
        require(end <= len(data) and end - offset <= 16 * 1024 * 1024,
                'Record exceeds file bounds or wire size limit')
        packet = data[offset:end]
        require(zlib.crc32(packet[:64] + packet[68:]) == crc,
                'Record CRC mismatch')
        row_format = '<QiQ' + str(obs) + 'fifB' + str(actions) + 'B2f'
        row_format += str(states) + 'I'
        row_bytes = struct.calcsize(row_format)
        require(payload == count * row_bytes, 'Payload size does not match rows')
        rows = [struct.unpack_from(row_format, packet, header_bytes + i * row_bytes)
                for i in range(count)]
        records.append({'run_id': run_id, 'sequence': sequence, 'seed': seed,
                        'shape': [obs, actions, states], 'bytes': len(packet),
                        'rows': rows})
        offset = end
    require(offset == len(data), 'Unexpected trailing fixture bytes')
    require(len(records) == 2, 'Expected two fixture records')
    for record, expected_count, seq, episodes in zip(
            records, [2, 1], [88, 89], [[111, 222], [333]]):
        require(record['run_id'] == 73 and record['seed'] == 17,
                'Unexpected fixture run ID or seed')
        require(record['sequence'] == seq and record['shape'] == [2, 2, 2],
                'Unexpected fixture sequence or shape')
        require(len(record['rows']) == expected_count,
                'Unexpected fixture transition count')
        for i, row in enumerate(record['rows']):
            expected = (episodes[i], i, i + 5, float(i + 1), float(-i - 1), i,
                        float(i) + 0.25, i, 1, 1, 0.5, -0.5, i + 7, 0xffffffff)
            require(row == expected, f'Unexpected fixture row: {row} != {expected}')
    return {'file': str(path), 'bytes': len(data), 'records': len(records),
            'transitions': sum(len(r['rows']) for r in records),
            'sequences': [r['sequence'] for r in records],
            'sha256': hashlib.sha256(data).hexdigest()}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--compiler', default='g++')
    parser.add_argument('--mode', choices=['release', 'ubsan', 'asan'],
                        default='release')
    parser.add_argument('--output-dir', type=Path)
    args = parser.parse_args()
    compiler = shutil.which(args.compiler)
    if not compiler:
        parser.error('Compiler not found: ' + args.compiler)
    if args.output_dir is None:
        tmpdir = os.environ.get('TMPDIR')
        if not tmpdir:
            parser.error('Set TMPDIR or pass --output-dir; no system-temp fallback')
        output = Path(tempfile.mkdtemp(prefix='hermes-verify-data-', dir=tmpdir))
    else:
        output = args.output_dir.resolve()
        output.mkdir(parents=True, exist_ok=True)
    fixture = output / 'rollouts.crabs'
    if fixture.exists():
        parser.error('Choose a fresh output directory: fixture already exists')
    commands = []
    env = os.environ.copy()
    env['ASAN_OPTIONS'] = 'detect_leaks=1:halt_on_error=1'
    env['UBSAN_OPTIONS'] = 'halt_on_error=1:print_stacktrace=1'

    def invoke(label, command):
        result = subprocess.run(command, cwd=ROOT, env=env,
                                capture_output=True, text=True)
        item = {'label': label, 'command': command, 'exit_code': result.returncode,
                'stdout': result.stdout, 'stderr': result.stderr}
        commands.append(item)
        with (output / 'commands.jsonl').open('a') as log:
            log.write(json.dumps(item) + '\n')
        if result.returncode:
            raise RuntimeError(label + ':\n' + result.stdout + result.stderr)
        return result.stdout

    flags = ['-std=c++2b', '-Wall', '-Wextra', '-Werror', '-Wno-unknown-pragmas',
             '-Wno-deprecated-enum-enum-conversion',
             '-Wno-deprecated-anon-enum-enum-conversion',
             '-I_Seams/Data', '-I.', '-I..']
    if args.mode == 'release':
        flags += ['-O2', '-fstrict-aliasing']
    else:
        flags += ['-O1', '-g', '-fno-omit-frame-pointer', '-fno-sanitize-recover=all']
        if args.mode == 'ubsan':
            flags += ['-fsanitize=undefined,float-cast-overflow']
        else:
            flags += ['-fsanitize=address']
    # -x c++: GCC 14 classifies .hxx sources as headers and would save a
    # precompiled header (gpch) instead of an executable; force the language.
    prefix = [compiler, '-x', 'c++'] + flags
    version = invoke('compiler version', [compiler, '--version']).splitlines()[0]
    seams, demo, consumer, fuzz = [output / name for name in
                                   ['data-seams', 'data-demo', 'data-consumer',
                                    'data-fuzz']]
    invoke('build seams', prefix + ['_Seams/Data/_Main.hxx', '-o', str(seams)])
    seam_stdout = invoke('run seams', [str(seams), str(fixture)])
    require('PASS: Data record/dataset/plot seams' in seam_stdout,
            'Missing Data seam PASS marker')
    decoded = decode_fixture(fixture)
    invoke('build split-TU consumer', prefix + ['_Seams/Data/Consumer.hxx',
           '_Seams/Data/Implementation.hxx', '-o', str(consumer)])
    invoke('run split-TU consumer', [str(consumer)])
    invoke('build mutation/property corpus', prefix + ['_Seams/Data/Fuzz.hxx',
                                                      '-o', str(fuzz)])
    fuzz_stdout = invoke('run mutation/property corpus', [str(fuzz)])
    require('PASS: 10000 mutated decoder inputs + 10000 plot pushes' in fuzz_stdout,
            'Missing mutation/property corpus PASS marker')
    invoke('build demo', prefix + ['_Seams/Data/Demo.hxx', '-o', str(demo)])
    demo_stdout = invoke('run demo stdout', [str(demo)])
    require(demo_stdout.startswith('Synthetic seam inputs - NOT training results.\n'),
            'Missing synthetic-input demo disclaimer')
    names = ['reward', 'policy_loss', 'value_loss', 'approx_kl', 'entropy']
    require(all('\n' + name + '\n' in demo_stdout for name in names),
            'Missing demo metric series')
    require(all(ord(c) < 128 for c in demo_stdout), 'Demo output is not ASCII')
    with (output / 'ascii-metrics.txt').open('wb') as chart_file:
        redirected = subprocess.run([str(demo)], cwd=ROOT, env=env,
                                    stdout=chart_file, stderr=subprocess.PIPE)
    require(redirected.returncode == 0,
            f'Redirected demo failed: {redirected.stderr!r}')
    require((output / 'ascii-metrics.txt').read_bytes() == demo_stdout.encode('ascii'),
            'Demo stdout and redirected file differ')

    # Source audit includes all new C++ module/test files, not vendor headers.
    sources = sorted(p for directory in [ROOT / 'Data', ROOT / '_Seams' / 'Data']
                     for p in directory.glob('*')
                     if p.suffix in ['.h', '.hpp', '.hxx', '.cpp'] and
                     p.name != '00.Core.hxx')
    allowed_headers = {'_Config.h', 'errno.h', 'fcntl.h', 'unistd.h', 'float.h'}
    hashes = {}
    case_ids = set()
    for path in sources:
        text = path.read_text()
        require(text.startswith('// Copyright AStarship <https://astarship.net>.'),
                f'Missing copyright header: {path}')
        require('std::' not in text and
                not re.search(r'#include\s+[<"]\S+\.inl[>"]', text),
                f'Forbidden std:: or .inl include: {path}')
        includes = re.findall(r'^\s*#include\s+<([^>]+)>', text, re.M)
        require(all(header in allowed_headers for header in includes),
                f'Forbidden system header: {path}: {includes}')
        case_ids.update(re.findall(r'FAIL ([RP]\d{2}):', text))
        hashes[str(path.relative_to(ROOT))] = hashlib.sha256(path.read_bytes()).hexdigest()
    expected_cases = {f'R{i:02}' for i in range(1, 13)}
    expected_cases |= {f'P{i:02}' for i in range(1, 10)}
    require(case_ids == expected_cases,
            f'Unexpected seam cases: {sorted(case_ids)} != {sorted(expected_cases)}')
    report = {'status': 'PASS', 'scope': 'opt-in Data; not default legacy/IMUL suite',
              'compiler': version, 'mode': args.mode, 'case_ids': sorted(case_ids),
              'case_groups': len(case_ids), 'independent_wire_decode': decoded,
              'split_tu_consumer': 'PASS',
              'mutation_property_corpus': {'decoder_inputs': 10000,
                                           'plot_pushes': 10000, 'status': 'PASS'},
              'ascii_stdout_file_equality': 'PASS',
              'demo_series': names, 'chart': str(output / 'ascii-metrics.txt'),
              'source_audit': 'PASS', 'source_sha256': hashes, 'commands': commands}
    (output / 'verification.json').write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps({key: value for key, value in report.items()
                      if key not in ['commands', 'source_sha256']}, indent=2))
    print('Evidence:', output / 'verification.json')


if __name__ == '__main__':
    main()

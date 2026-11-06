// Copyright AStarship <https://astarship.net>.
#include "pch.h"
#define _CrabsCore			1
#define _CrabsIToS			2
#define _CrabsFToS			3
#define _CrabsUniprinter			4
#define _CrabsClock			5
#define _CrabsArray			6
#define _CrabsStack			7
#define _CrabsString			8
#define _CrabsRegEx			9
#define _CrabsLoom			10
#define _CrabsList			11
#define _CrabsBook			12
#define _CrabsMap			13
#define _CrabsTable			14
#define _CrabsDictionary			15
#define _CrabsOp			16
#define _CrabsRoom			17
#define _CrabsMathCore			18
#define _CrabsRandom			19
#define _CrabsFile			20
#define _CrabsPromise			21
#define _CrabsScript			22
#define _CrabsRelease			23
#define Core			24
#define Release			25
#define Core			26
#define StripComments			27
#define Release			28
#define Core			29
#define Record			30
#define Dataset			31
#define AsciiPlot			32
#define Release			33
#define Core			34
#define Release			35
#define Core			36
#define Release			37
#define Core			38
#define Release			39
#define Core			40
#define Release			41
#define Core			42
#define Ring			43
#define LegacyRed			44
#define TwoProc			45
#define Release			46
#define Core			47
#define Release			48
#define Core			49
#define CudaBlas			50
#define Release			51
#define Core			52
#define Plot			53
#define Release			54
#define Core			55
#define Release			56
#define Core			57
#define Release			58
#define Core			59
#define Release			60
#define Core			61
#define Release			62
#define _SeamN Release

#include <_Config.h>
#include <ASCIICrabs/_Config.h>
#include <ASCIICrabs/_Package.hxx>
#include <ASCIICrabs/Test.hpp>
#include "_Seams.hxx"

using namespace CTK;

ISN main(ISN arg_count, CHA** args) {
  const CHA* argss = ArgsToString(arg_count, args);
  return SeamResult(CTKIMULTest::Main(argss));
}

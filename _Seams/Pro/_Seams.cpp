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
#define _CrabsTKAudioCore			24
#define _CrabsTKAudioRelease			25
#define _CrabsTKCodeCore			26
#define _CrabsTKCodeStripComments			27
#define _CrabsTKCodeRelease			28
#define _CrabsTKDataCore			29
#define _CrabsTKDataRecord			30
#define _CrabsTKDataDataset			31
#define _CrabsTKDataAsciiPlot			32
#define _CrabsTKDataRelease			33
#define _CrabsTKEnvCore			34
#define _CrabsTKEnvRelease			35
#define _CrabsTKFormsCore			36
#define _CrabsTKFormsRelease			37
#define _CrabsTKGUICore			38
#define _CrabsTKGUIRelease			39
#define _CrabsTKIMULCore			40
#define _CrabsTKIMULRelease			41
#define _CrabsTKIPCCore			42
#define _CrabsTKIPCRing			43
#define _CrabsTKIPCLegacyRed			44
#define _CrabsTKIPCTwoProc			45
#define _CrabsTKIPCRelease			46
#define _CrabsTKImageCore			47
#define _CrabsTKImageRelease			48
#define _CrabsTKMathCore			49
#define _CrabsTKMathCudaBlas			50
#define _CrabsTKMathRelease			51
#define _CrabsTKPlotCore			52
#define _CrabsTKPlotPlot			53
#define _CrabsTKPlotRelease			54
#define _CrabsTKProCore			55
#define _CrabsTKProRelease			56
#define _CrabsTKTouchCore			57
#define _CrabsTKTouchRelease			58
#define _CrabsTKWebCore			59
#define _CrabsTKWebRelease			60
#define _CrabsTKWhoCore			61
#define _CrabsTKWhoRelease			62
#define _SeamN _CrabsTKWhoRelease

#include <_Config.h>
#include <ASCIICrabs/_Config.h>
#include <ASCIICrabs/_Package.hxx>
#include <ASCIICrabs/Test.hpp>
#include "_Seams.hxx"

using namespace CTK;

ISC main(ISC arg_count, CHA** args) {
  const CHA* argss = ArgsToString(arg_count, args);
  return SeamResult(CTKProTest::Main(argss));
}

#include "pch.h"
#include <_Config.h>
#include <_Package.hxx>
// Single-TU implementations the seam tests link against (same set as the
// pre-reorg _Seams/_Main.cpp entry; _Package.hxx is NOT pulled in here —
// the root config deliberately excludes the full ASCIICrabs machine).
#include <ASCIICrabs/COut.h>
#include <ASCIICrabs/COut.hxx>
#include <ASCIICrabs/AType.hxx>
#include <ASCIICrabs/Array.hxx>
#include <ASCIICrabs/Puff.hxx>
#include <ASCIICrabs/Test.hpp>
#include <ASCIICrabs/Test.hxx>
#include <ASCIICrabs/Stringf.hxx>
#include "ASCIICrabs/00.Core.hxx"
#include "Data/00.Core.hxx"
#include "../IPC/_Seams/00.Core.hxx"
#include "../Math/_Seams/00.Core.hxx"
#include "Who/00.Core.hxx"
#include "Code/00.Core.hxx"
#include "Env/00.Core.hxx"
#include "GUI/00.Core.hxx"
#include "IMUL/00.Core.hxx"
#include "Plot/00.Core.hxx"
#include "Touch/00.Core.hxx"
#include "Release/00.Core.hxx"
using namespace _;
using namespace CT;
namespace {
::TestCase CrabsTkTree[] = {
  CT::ASCIICrabs::Test::CrabsCore,
  CT::Data::Test::DataRecord,
  CT::Data::Test::DataDataset,
  CT::Data::Test::DataAsciiplot,
  CT::IPC::Test::IpcCore,
  CT::IPC::Test::IpcRing,
  CT::IPC::Test::IpcLegacyred,
  CT::IPC::Test::IpcTwoproc,
  CT::Math::Test::MathBlas,
  CT::Math::Test::MathCudablas,
  CT::Math::Test::MathBlastest,
  CT::Who::Test::WhoCore,
  CT::Who::Test::WhoAccount,
  CT::Who::Test::WhoAddress,
  CT::Who::Test::WhoAddressemail,
  CT::Who::Test::WhoAuthenticator,
  CT::Who::Test::WhoEntity,
  CT::Who::Test::WhoEntitygroup,
  CT::Who::Test::WhoEntitylist,
  CT::Who::Test::WhoHandle,
  CT::Who::Test::WhoName,
  CT::Who::Test::WhoOrganization,
  CT::Who::Test::WhoPassword,
  CT::Who::Test::WhoPerson,
  CT::Who::Test::WhoPrivilege,
  CT::Who::Test::WhoProfile,
  CT::Who::Test::WhoRegistrar,
  CT::Who::Test::WhoRelationship,
  CT::Who::Test::WhoRole,
  CT::Who::Test::WhoRoster,
  CT::Who::Test::WhoSymbol,
  CT::Who::Test::WhoUidassigner,
  CT::Who::Test::WhoUser,
  CT::Who::Test::WhoUserlist,
  CT::Code::Test::CodeCore,
  CT::Code::Test::CodeCodemodule,
  CT::Code::Test::CodeCommentstripper,
  CT::Code::Test::CodeLexer,
  CT::Code::Test::CodeFitter,
  CT::Env::Test::EnvCore,
  CT::Env::Test::EnvVar,
  CT::GUI::Test::GuiCore,
  CT::IMUL::Test::ImulCore,
  CT::IMUL::Test::ImulRelease,
  CT::Plot::Test::PlotCore,
  CT::Touch::Test::TouchCore,
  CT::Release::Test::Release,
};
}
ISN main(ISN arg_count, CHA** args) {
  return SeamTreeTest(arg_count, args, CrabsTkTree,
                      sizeof(CrabsTkTree) / sizeof(CrabsTkTree[0]));
}

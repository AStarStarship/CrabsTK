/* CRABSTK
@file    /_Seams/_Main.cpp
@author  Cale McCollough <https://cookingwithcale.org>
@license Copyright 2019-20 (C) AStarship <astarship.net>; all rights
reserved (R). This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed with this file,
You can obtain one at <https://mozilla.org/MPL/2.0/>. */

#include <_Config.h>
#include <ASCIICrabs/COut.h>
#include <ASCIICrabs/COut.hxx>
#include <ASCIICrabs/AType.hxx>
#include <ASCIICrabs/Array.hxx>
#include <ASCIICrabs/Puff.hxx>

#include <_Package.hxx>

#include <ASCIICrabs/Test.hpp>
#include <ASCIICrabs/Test.hxx>
#include <ASCIICrabs/Stringf.hxx>

#include "Crab/00.Core.h"
#include "Who/00.Core.h"
#include "Pro/00.Core.h"
#include "Code/00.Core.h"
#include "Imul/00.Core.h"
#include "Data/00.Core.h"
#include "Plot/00.Core.h"
#include "Touch/00.Core.h"
#include "Gui/00.Core.h"
#include "Env/00.Core.h"
#include "Release/00.Core.h"

using namespace _;
using namespace CT;

ISN main(ISN arg_count, CHA** args) {
#if SEAM == CRABSTK_RELEASE
  return SeamResult(Release(ArgsToString(arg_count, args)));
#else
  return TTestTree<Crab::Test::CrabsCore, Crab::Test::CrabsRng, Crab::Test::CrabsItos, Crab::Test::CrabsFtos, Crab::Test::CrabsSprinter, Crab::Test::CrabsClock, Crab::Test::CrabsStack, Crab::Test::CrabsMatrix, Crab::Test::CrabsString, Crab::Test::CrabsLoom, Crab::Test::CrabsList, Crab::Test::CrabsBook, Crab::Test::CrabsMap, Crab::Test::CrabsTable, Crab::Test::CrabsDic, Crab::Test::CrabsMachine, Crab::Test::CrabsDoor, Crab::Test::CrabsFilecabinet, Who::Test::WhoCore, Who::Test::WhoAccount, Who::Test::WhoAddress, Who::Test::WhoAddressemail, Who::Test::WhoAuthenticator, Who::Test::WhoEntity, Who::Test::WhoEntitygroup, Who::Test::WhoEntitylist, Who::Test::WhoHandle, Who::Test::WhoName, Who::Test::WhoOrganization, Who::Test::WhoPassword, Who::Test::WhoPerson, Who::Test::WhoPrivilege, Who::Test::WhoProfile, Who::Test::WhoRegistrar, Who::Test::WhoRelationship, Who::Test::WhoRole, Who::Test::WhoRoster, Who::Test::WhoSymbol, Who::Test::WhoUidassigner, Who::Test::WhoUser, Who::Test::WhoUserlist, Pro::Test::ProCore, Pro::Test::ProGit, Pro::Test::ProIssue, Pro::Test::ProMission, Pro::Test::ProProject, Pro::Test::ProSchedule, Pro::Test::ProTask, Pro::Test::ProWorkspace, Code::Test::CodeCore, Code::Test::CodeCodemodule, Code::Test::CodeCommentstripper, Code::Test::CodeLexer, Code::Test::CodeFitter, Imul::Test::ImulCore, Imul::Test::ImulRelease, Data::Test::DataRecord, Data::Test::DataDataset, Data::Test::DataAsciiplot, Plot::Test::PlotCore, Touch::Test::TouchCore, Gui::Test::GuiCore, Env::Test::EnvCore, Env::Test::EnvVar, Release::Test::Release>(arg_count, args);
#endif
}

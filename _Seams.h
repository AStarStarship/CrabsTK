// CrabsTK Seams — single consistent sequence, no duplicates.
//
// Seam IDs are monotonic. Each module's CRABSTK_<MOD>_CORE is the
// first ID in that module's range. Gating: #if SEAM >= CRABSTK_<MOD>_CORE
// pulls in the entire module.

// CRABS core IDs (seam table; CRABSTK_* module IDs follow below).
#define CRABS_CORE                        1
#define CRABS_RNG                         2
#define CRABS_ITOS                        3
#define CRABS_FTOS                        4
#define CRABS_SPRINTER                    5
#define CRABS_CLOCK                       6
#define CRABS_STACK                       7
#define CRABS_MATRIX                      8
#define CRABS_STRING                      9
#define CRABS_LOOM                        10
#define CRABS_LIST                        11
#define CRABS_BOOK                        12
#define CRABS_MAP                         13
#define CRABS_TABLE                       14
#define CRABS_DIC                         15
#define CRABS_MACHINE                     16
#define CRABS_DOOR                        17
#define CRABS_FILECABINET                 18

// Who API
#define CRABSTK_WHO_CORE             19
#define CRABSTK_WHO_ACCOUNT          19
#define CRABSTK_WHO_ADDRESS          20
#define CRABSTK_WHO_ADDRESSEMAIL     21
#define CRABSTK_WHO_AUTHENTICATOR    22
#define CRABSTK_WHO_ENTITY           23
#define CRABSTK_WHO_ENTITYGROUP      24
#define CRABSTK_WHO_ENTITYLIST       25
#define CRABSTK_WHO_HANDLE           26
#define CRABSTK_WHO_NAME             27
#define CRABSTK_WHO_ORGANIZATION     28
#define CRABSTK_WHO_PASSWORD         29
#define CRABSTK_WHO_PERSON           30
#define CRABSTK_WHO_PRIVILEGE        31
#define CRABSTK_WHO_PROFILE          32
#define CRABSTK_WHO_REGISTRAR        33
#define CRABSTK_WHO_RELATIONSHIP     34
#define CRABSTK_WHO_ROLE             35
#define CRABSTK_WHO_ROSTER           36
#define CRABSTK_WHO_SYMBOL           37
#define CRABSTK_WHO_UIDASSIGNER      38
#define CRABSTK_WHO_USER             39
#define CRABSTK_WHO_USERLIST         40

// Pro API
#define CRABSTK_PRO_CORE             41
#define CRABSTK_PRO_GIT              41
#define CRABSTK_PRO_ISSUE            42
#define CRABSTK_PRO_MISSION          43
#define CRABSTK_PRO_PROJECT          44
#define CRABSTK_PRO_SCHEDULE         45
#define CRABSTK_PRO_TASK             46
#define CRABSTK_PRO_WORKSPACE        47

// Code API
#define CRABSTK_CODE_CORE            48
#define CRABSTK_CODE_CODEMODULE      48
#define CRABSTK_CODE_COMMENTSTRIPPER 49
#define CRABSTK_CODE_LEXER           50
#define CRABSTK_CODE_FITTER          51

// IMUL API
#define CRABSTK_IMUL_CORE            52
#define CRABSTK_IMUL_RELEASE         53

// Data API
#define CRABSTK_DATA_RECORD          54
#define CRABSTK_DATA_DATASET         55
#define CRABSTK_DATA_ASCIIPLOT       56

// Plot API
#define CRABSTK_PLOT_CORE            57

// Touch API
#define CRABSTK_TOUCH_CORE           58

// GUI API
#define CRABSTK_GUI_CORE             59

// Release: build and run the full test tree.
#define CRABSTK_RELEASE              60
#define SEAM_N                       60

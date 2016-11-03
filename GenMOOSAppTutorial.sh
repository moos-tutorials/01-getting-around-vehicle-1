#!/bin/bash

###############################################################################
# If statements to protect from misusage
if [ -z "${1}" ] ; then
  echo "GenMOOSAppTutorial: usage: ${0} [app-name] [prefix] [your-name]"
  exit 1
fi

if [ -z "${2}" ]; then
  echo "GenMOOSAppTutorial: usage: ${0} [app-name] [prefix] [your-name]"
  exit 2
fi

if [ -z "${3}" ] ; then
  echo "Please enter your name."
  echo "GenMOOSAppTutorial usage: ${0} [app-name] [prefix] [your-name]"
  exit 3
fi

if [[ -d "src/${2}${1}" ]]; then
  echo "A directory with the name src/${2}${1} seem to exist already."
  echo "Aborting to avoid any damage."
  exit 4
fi

###############################################################################
# Saving original directory and cd to new one
DATE=`date +%Y/%m/%d`
CURR_DIR=$PWD
NEW_DIR=$CURR_DIR/src/${2}${1}
mkdir -p $NEW_DIR
cd $NEW_DIR

###############################################################################
# Creating the top-level CMakeLists.txt
if [ -e ../CMakeLists.txt ]; then
  echo "Seems like a src/CMakeLists.txt exists."
  echo "Please make sure to add the following line to src/CMakeLists.txt"
  echo "  ADD_SUBDIRECTORY( ${2}${1} ) "
else
cat > ../CMakeLists.txt <<EOF
#============================================================================
# Add the libraries in the current directory to the include path
#============================================================================
FILE(GLOB LOCAL_LIBRARY_DIRS ./lib_*)
INCLUDE_DIRECTORIES(\${LOCAL_LIBRARY_DIRS})

#============================================================================
# List the subdirectories to build...
#============================================================================
ADD_SUBDIRECTORY( ${2}${1} )

EOF
fi


###############################################################################
# Creating the app CMakeLists.txt
cat > CMakeLists.txt <<EOF
#===========================================================================
# FILE:  $NEW_DIR/CMakeLists.txt
# DATE:  $DATE
# INFO:  Source-level CMakeLists.txt file for the ${2}${1} project
# NAME:  ${3}
#===========================================================================

SET(SRC
  ${1}.cpp
  ${1}_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(${2}${1} \${SRC})

TARGET_LINK_LIBRARIES(${2}${1}
  \${MOOS_LIBRARIES}
  \${MOOSGeodesy_LIBRARIES}
  mbutil
  apputil
  m
  pthread
)

EOF

cat > ${1}.h <<EOF
/************************************************************/
/*    NAME: ${3}                                            */
/*    ORGN: moos-tutorials                                  */
/*    FILE: ${1}.h                                          */
/*    DATE: $DATE                                      */
/************************************************************/

#ifndef ${1}_HEADER
#define ${1}_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"

class ${1} : public AppCastingMOOSApp
{
 public:
  ~${1}() {};
  ${1}();

 protected: // Standard MOOSApp functions to overload
  bool OnNewMail(MOOSMSG_LIST &NewMail);
  bool Iterate();
  bool OnConnectToServer();
  bool OnStartUp();

 protected: // Standard AppCastingMOOSApp function to overload
  bool buildReport();

 protected:
  void registerVariables();

 private: // Configuration variables

 private: // State variables
};

#endif

EOF

cat > main.cpp <<EOF
/************************************************************/
/*    NAME: ${3}                                            */
/*    ORGN: moos-tutorials                                  */
/*    FILE: main.cpp                                        */
/*    DATE: $DATE                                      */
/************************************************************/

#include <string>
#include "MBUtils.h"
#include "ColorParse.h"
#include "${1}.h"
#include "${1}_Info.h"

using namespace std;

int main(int argc, char *argv[])
{
  string mission_file;
  string run_command = argv[0];

  for(int i=1; i<argc; i++) {
    string argi = argv[i];
    if((argi=="-v") || (argi=="--version") || (argi=="-version"))
      showReleaseInfoAndExit();
    else if((argi=="-e") || (argi=="--example") || (argi=="-example"))
      showExampleConfigAndExit();
    else if((argi == "-h") || (argi == "--help") || (argi=="-help"))
      showHelpAndExit();
    else if((argi == "-i") || (argi == "--interface"))
      showInterfaceAndExit();
    else if(strEnds(argi, ".moos") || strEnds(argi, ".moos++"))
      mission_file = argv[i];
    else if(strBegins(argi, "--alias="))
      run_command = argi.substr(8);
    else if(i==2)
      run_command = argi;
  }

  if(mission_file == "")
    showHelpAndExit();

  cout << termColor("green");
  cout << "${2}${1} launching as " << run_command << endl;
  cout << termColor() << endl;

  ${1} ${1};

  ${1}.Run(run_command.c_str(), mission_file.c_str());

  return(0);
}

EOF

cat > ${2}${1}.moos <<EOF
//------------------------------------------------
// ${2}${1} config block

ProcessConfig = ${2}${1}
{
  AppTick   = 4
  CommsTick = 4

  // ${2}${1} configuration here
}

EOF

cat > ${1}.cpp <<EOF
/************************************************************/
/*    NAME: ${3}                                            */
/*    ORGN: moos-tutorials                                  */
/*    FILE: ${1}.cpp                                        */
/*    DATE: $DATE                                      */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "ACTable.h"
#include "${1}.h"

using namespace std;

//---------------------------------------------------------
// Constructor

${1}::${1}()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool ${1}::OnNewMail(MOOSMSG_LIST &NewMail)
{
  AppCastingMOOSApp::OnNewMail(NewMail);

  MOOSMSG_LIST::iterator p;
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;
    string key    = msg.GetKey();

#if 0 // Keep these around just for template
    string comm  = msg.GetCommunity();
    double dval  = msg.GetDouble();
    string sval  = msg.GetString();
    string msrc  = msg.GetSource();
    double mtime = msg.GetTime();
    bool   mdbl  = msg.IsDouble();
    bool   mstr  = msg.IsString();
#endif

    if(key == "FOO")
      cout << "great!";

    else if(key != "APPCAST_REQ") // handle by AppCastingMOOSApp
      reportRunWarning("Unhandled Mail: " + key);
   }

  return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool ${1}::OnConnectToServer()
{
  registerVariables();
  return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool ${1}::Iterate()
{
  AppCastingMOOSApp::Iterate();
  // Do your thing here!
  AppCastingMOOSApp::PostReport();
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool ${1}::OnStartUp()
{
  AppCastingMOOSApp::OnStartUp();

  STRING_LIST sParams;
  m_MissionReader.EnableVerbatimQuoting(false);
  if(!m_MissionReader.GetConfiguration(GetAppName(), sParams))
    reportConfigWarning("No config block found for " + GetAppName());

  STRING_LIST::iterator p;
  for(p=sParams.begin(); p!=sParams.end(); p++) {
    string orig  = *p;
    string line  = *p;
    string param = toupper(biteStringX(line, '='));
    string value = line;

    bool handled = false;
    if(param == "FOO") {
      handled = true;
    }
    else if(param == "BAR") {
      handled = true;
    }

    if(!handled)
      reportUnhandledConfigWarning(orig);

  }

  registerVariables();
  return(true);
}

//---------------------------------------------------------
// Procedure: registerVariables

void ${1}::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  // Register("FOOBAR", 0);
}


//------------------------------------------------------------
// Procedure: buildReport()

bool ${1}::buildReport()
{
  m_msgs << "============================================ \n";
  m_msgs << "File:                                        \n";
  m_msgs << "============================================ \n";

  ACTable actab(4);
  actab << "Alpha | Bravo | Charlie | Delta";
  actab.addHeaderLines();
  actab << "one" << "two" << "three" << "four";
  m_msgs << actab.getFormattedString();

  return(true);
}

EOF


cat >> ${1}_Info.h <<EOF
/****************************************************************/
/*   NAME: ${3}                                                 */
/*   ORGN: moos-tutorials                                       */
/*   FILE: ${1}_Info.h                                      */
/*   DATE: $DATE                                        */
/****************************************************************/

#ifndef ${1}_INFO_HEADER
#define ${1}_INFO_HEADER

void showSynopsis();
void showHelpAndExit();
void showExampleConfigAndExit();
void showInterfaceAndExit();
void showReleaseInfoAndExit();

#endif

EOF


cat >> ${1}_Info.cpp <<EOF
/****************************************************************/
/*   NAME: ${3}                                             */
/*   ORGN: moos-tutorials                                       */
/*   FILE: ${1}_Info.cpp                               */
/*   DATE: $DATE                                        */
/****************************************************************/

#include <cstdlib>
#include <iostream>
#include "${1}_Info.h"
#include "ColorParse.h"
#include "ReleaseInfo.h"

using namespace std;

//----------------------------------------------------------------
// Procedure: showSynopsis

void showSynopsis()
{
  blk("SYNOPSIS:                                                       ");
  blk("------------------------------------                            ");
  blk("  The ${2}${1} application is used for               ");
  blk("                                                                ");
  blk("                                                                ");
  blk("                                                                ");
  blk("                                                                ");
}

//----------------------------------------------------------------
// Procedure: showHelpAndExit

void showHelpAndExit()
{
  blk("                                                                ");
  blu("=============================================================== ");
  blu("Usage: ${2}${1} file.moos [OPTIONS]                   ");
  blu("=============================================================== ");
  blk("                                                                ");
  showSynopsis();
  blk("                                                                ");
  blk("Options:                                                        ");
  mag("  --alias","=<ProcessName>                                      ");
  blk("      Launch ${2}${1} with the given process name         ");
  blk("      rather than ${2}${1}.                           ");
  mag("  --example, -e                                                 ");
  blk("      Display example MOOS configuration block.                 ");
  mag("  --help, -h                                                    ");
  blk("      Display this help message.                                ");
  mag("  --interface, -i                                               ");
  blk("      Display MOOS publications and subscriptions.              ");
  mag("  --version,-v                                                  ");
  blk("      Display the release version of ${2}${1}.        ");
  blk("                                                                ");
  blk("Note: If argv[2] does not otherwise match a known option,       ");
  blk("      then it will be interpreted as a run alias. This is       ");
  blk("      to support pAntler launching conventions.                 ");
  blk("                                                                ");
  exit(0);
}

//----------------------------------------------------------------
// Procedure: showExampleConfigAndExit

void showExampleConfigAndExit()
{
  blk("                                                                ");
  blu("=============================================================== ");
  blu("${2}${1} Example MOOS Configuration                   ");
  blu("=============================================================== ");
  blk("                                                                ");
  blk("ProcessConfig = ${2}${1}                              ");
  blk("{                                                               ");
  blk("  AppTick   = 4                                                 ");
  blk("  CommsTick = 4                                                 ");
  blk("                                                                ");
  blk("}                                                               ");
  blk("                                                                ");
  exit(0);
}


//----------------------------------------------------------------
// Procedure: showInterfaceAndExit

void showInterfaceAndExit()
{
  blk("                                                                ");
  blu("=============================================================== ");
  blu("${2}${1} INTERFACE                                    ");
  blu("=============================================================== ");
  blk("                                                                ");
  showSynopsis();
  blk("                                                                ");
  blk("SUBSCRIPTIONS:                                                  ");
  blk("------------------------------------                            ");
  blk("  NODE_MESSAGE = src_node=alpha,dest_node=bravo,var_name=FOO,   ");
  blk("                 string_val=BAR                                 ");
  blk("                                                                ");
  blk("PUBLICATIONS:                                                   ");
  blk("------------------------------------                            ");
  blk("  Publications are determined by the node message content.      ");
  blk("                                                                ");
  exit(0);
}

//----------------------------------------------------------------
// Procedure: showReleaseInfoAndExit

void showReleaseInfoAndExit()
{
  showReleaseInfo("${2}${1}", "mit");
  exit(0);
}

EOF


echo "${2}${1} generated"

###############################################################################
# Returning to original directory and exiting with success
cd $CURR_DIR
exit 0

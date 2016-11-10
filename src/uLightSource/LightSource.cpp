/************************************************************/
/*    NAME: Your Name Here                                            */
/*    ORGN: moos-tutorials                                  */
/*    FILE: LightSource.cpp                                        */
/*    DATE: 2016/11/01                                      */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "ACTable.h"
#include "LightSource.h"

using namespace std;

//---------------------------------------------------------
// Constructor

LightSource::LightSource(): m_intensity(0.0),
  m_x(0.0), m_y(0.0)
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool LightSource::OnNewMail(MOOSMSG_LIST &NewMail)
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

    if(key == "ULSOURCE_SET_INTENSITY")
      m_intensity = msg.GetDouble();

    else if(key != "APPCAST_REQ") // handle by AppCastingMOOSApp
      reportRunWarning("Unhandled Mail: " + key);
   }

  return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool LightSource::OnConnectToServer()
{
  registerVariables();
  return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool LightSource::Iterate()
{
  AppCastingMOOSApp::Iterate();
  // Do your thing here!
  Notify("ULSOURCE_INTENSITY", m_intensity);
  Notify("ULSOURCE_POS_X", m_x);
  Notify("ULSOURCE_POS_Y", m_y);
  AppCastingMOOSApp::PostReport();
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool LightSource::OnStartUp()
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
    if(param == "I") {
      m_intensity = atof(line.c_str());
      handled = true;
    } else if(param == "START_X") {
      m_x = atof(line.c_str());
      handled = true;
    } else if(param == "START_Y") {
      m_y = atof(line.c_str());
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

void LightSource::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  Register("ULSOURCE_SET_INTENSITY", 0);
}


//------------------------------------------------------------
// Procedure: buildReport()

bool LightSource::buildReport()
{
  m_msgs << "Configutation                                \n";
  m_msgs << "============================================ \n";

  ACTable actab(3);
  actab << "Position X | Position Y | Intensity";
  actab.addHeaderLines();
  actab << m_x << m_y << m_intensity;
  m_msgs << actab.getFormattedString();

  return(true);
}

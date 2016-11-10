/************************************************************/
/*    NAME: Your Name Here                                            */
/*    ORGN: moos-tutorials                                  */
/*    FILE: SimVehicle1.cpp                                        */
/*    DATE: 2016/11/03                                      */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "ACTable.h"
#include "SimVehicle1.h"
#include "AngleUtils.h"

using namespace std;

//---------------------------------------------------------
// Constructor

SimVehicle1::SimVehicle1():
  m_k_factor(0.0),
  m_max_speed(100.0),
  m_sensor_reading(0.0),
  m_pos_x(0.0), m_pos_y(0.0), m_pos_heading(0.0)
{
  m_time = MOOSTime();
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool SimVehicle1::OnNewMail(MOOSMSG_LIST &NewMail)
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

    if(key == "ULSENSOR_READING")
      m_sensor_reading = msg.GetDouble();

    else if(key != "APPCAST_REQ") // handle by AppCastingMOOSApp
      reportRunWarning("Unhandled Mail: " + key);
   }

  return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool SimVehicle1::OnConnectToServer()
{
  registerVariables();
  return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool SimVehicle1::Iterate()
{
  AppCastingMOOSApp::Iterate();

  m_v_speed = m_k_factor * m_sensor_reading;
  if (m_v_speed > m_max_speed)
    m_v_speed = m_max_speed;
  Notify("NAV_SPEED", m_v_speed);

  double radians = headingToRadians(m_pos_heading);
  double h = m_time - MOOSTime();
  m_time = MOOSTime();

  m_pos_x += h*cos(headingToRadians(m_pos_heading))*m_v_speed;
  m_pos_y += h*sin(headingToRadians(m_pos_heading))*m_v_speed;

  Notify("NAV_X", m_pos_x);
  Notify("NAV_Y", m_pos_y);
  Notify("NAV_HEADING", m_pos_heading);

  AppCastingMOOSApp::PostReport();
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool SimVehicle1::OnStartUp()
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
    if(param == "K") {
      m_k_factor = atof(value.c_str());
      handled = true;
    }
    else if(param == "MAX_SPEED") {
      m_max_speed = atof(value.c_str());
      handled = true;
    }
    else if(param == "START_X") {
      m_pos_x = atof(value.c_str());
      handled = true;
    }
    else if(param == "START_Y") {
      m_pos_y = atof(value.c_str());
      handled = true;
    }
    else if(param == "START_HEADING") {
      m_pos_heading = atof(value.c_str());
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

void SimVehicle1::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  Register("ULSENSOR_READING");
}


//------------------------------------------------------------
// Procedure: buildReport()

bool SimVehicle1::buildReport()
{
  m_msgs << "Configutation:\n";
  m_msgs << "============================================ \n";

  ACTable actab(2);
  actab << "K factor | Max speed";
  actab.addHeaderLines();
  actab << m_k_factor << m_max_speed;
  m_msgs << actab.getFormattedString();
  m_msgs << endl;

  m_msgs << "Data:\n";
  m_msgs << "============================================ \n";
  actab = ACTable(4);
  actab << "NAV_X | NAV_Y | NAV_HEADING | NAV_SPEED";
  actab << m_pos_x << m_pos_y << m_pos_heading << m_v_speed;
  m_msgs << actab.getFormattedString();
  m_msgs << endl;

  return(true);
}

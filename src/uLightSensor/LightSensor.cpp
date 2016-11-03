/************************************************************/
/*    NAME: Your Name Here                                            */
/*    ORGN: moos-tutorials                                  */
/*    FILE: LightSensor.cpp                                        */
/*    DATE: 2016/11/03                                      */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "ACTable.h"
#include "LightSensor.h"

using namespace std;

//---------------------------------------------------------
// Constructor

LightSensor::LightSensor():
  m_source_intensity(0.0),
  m_i_reading(0.0),
  m_sensitivity(1.0),
  m_source_pos_x(0.0), m_source_pos_y(0.0),
  m_sensor_pos_x(0.0), m_sensor_pos_y(0.0)
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool LightSensor::OnNewMail(MOOSMSG_LIST &NewMail)
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

    if(key == "ULSOURCE_INTENSITY")
      m_source_intensity = msg.GetDouble();
    else if(key == "ULSOURCE_POS_X")
      m_source_pos_x = msg.GetDouble();
    else if(key == "ULSOURCE_POS_Y")
      m_source_pos_y = msg.GetDouble();
    else if(key == "NAV_X")
      m_sensor_pos_x = msg.GetDouble();
    else if(key == "NAV_Y")
      m_sensor_pos_y = msg.GetDouble();
    else if(key == "ULSENSOR_SET_SENSITIVITY")
      m_sensitivity = msg.GetDouble();

    else if(key != "APPCAST_REQ") // handle by AppCastingMOOSApp
      reportRunWarning("Unhandled Mail: " + key);
   }

  return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool LightSensor::OnConnectToServer()
{
  registerVariables();
  return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool LightSensor::Iterate()
{
  AppCastingMOOSApp::Iterate();

  if ((m_sensor_pos_x == m_source_pos_x)
      && (m_sensor_pos_y == m_source_pos_y))
    m_i_reading = 0.;
  else
    m_i_reading = m_sensitivity/(pow(m_sensor_pos_x - m_source_pos_x, 2.)
                              + pow(m_sensor_pos_y - m_source_pos_y, 2.));

  Notify("ULSENSOR_READING", m_i_reading, 0.);

  AppCastingMOOSApp::PostReport();
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool LightSensor::OnStartUp()
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
    if(param == "S") {
      m_sensitivity = atof(value.c_str());
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

void LightSensor::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  Register("ULSOURCE_INTENSITY", 0);
  Register("ULSOURCE_POS_X", 0);
  Register("ULSOURCE_POS_Y", 0);
  Register("NAV_X", 0);
  Register("NAV_Y", 0);
  Register("ULSENSOR_SET_SENSITIVITY", 0);
}


//------------------------------------------------------------
// Procedure: buildReport()

bool LightSensor::buildReport()
{
  m_msgs << "Configutation:                               \n";
  m_msgs << "============================================ \n";

  ACTable actab(1);
  actab << "Sensitivity";
  actab.addHeaderLines();
  actab << m_sensitivity;
  m_msgs << actab.getFormattedString();
  m_msgs << endl;

  m_msgs << "Data:                                        \n";
  m_msgs << "============================================ \n";
  actab = ACTable(5);
  actab << "NAV_X | NAV_Y | SOURCE_X | SOURCE_Y | SOURCE _I";
  actab.addHeaderLines();
  actab << m_sensor_pos_x << m_sensor_pos_y << m_source_pos_x <<
        m_source_pos_y << m_source_intensity;
  m_msgs << actab.getFormattedString();
  m_msgs << endl;

  m_msgs << " Simulated reading:" << m_i_reading ;

  return(true);
}

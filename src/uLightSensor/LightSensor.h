/************************************************************/
/*    NAME: Your Name Here                                            */
/*    ORGN: moos-tutorials                                  */
/*    FILE: LightSensor.h                                          */
/*    DATE: 2016/11/03                                      */
/************************************************************/

#ifndef LightSensor_HEADER
#define LightSensor_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"

class LightSensor : public AppCastingMOOSApp
{
 public:
  ~LightSensor() {};
  LightSensor();

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
  double m_sensitivity;

 private: // State variables
  double m_source_intensity;
  double m_i_reading;
  double m_sensor_pos_x, m_sensor_pos_y;
  double m_source_pos_x, m_source_pos_y;
};

#endif

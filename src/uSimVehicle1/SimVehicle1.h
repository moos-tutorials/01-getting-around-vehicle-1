/************************************************************/
/*    NAME: Your Name Here                                            */
/*    ORGN: moos-tutorials                                  */
/*    FILE: SimVehicle1.h                                          */
/*    DATE: 2016/11/03                                      */
/************************************************************/

#ifndef SimVehicle1_HEADER
#define SimVehicle1_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"

class SimVehicle1 : public AppCastingMOOSApp
{
 public:
  ~SimVehicle1() {};
  SimVehicle1();

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
  double m_max_speed;
  double m_k_factor;

 private: // State variables
  double m_pos_x, m_pos_y, m_pos_heading;
  double m_v_speed;
  double m_sensor_reading;
  double m_time;
};

#endif

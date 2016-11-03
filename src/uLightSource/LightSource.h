/************************************************************/
/*    NAME: Your Name Here                                            */
/*    ORGN: moos-tutorials                                  */
/*    FILE: LightSource.h                                          */
/*    DATE: 2016/11/01                                      */
/************************************************************/

#ifndef LightSource_HEADER
#define LightSource_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"

class LightSource : public AppCastingMOOSApp
{
 public:
  ~LightSource() {};
  LightSource();

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
  double m_intensity;
  double m_x, m_y;

 private: // State variables
};

#endif

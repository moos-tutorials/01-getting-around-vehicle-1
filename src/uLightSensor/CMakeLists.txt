#===========================================================================
# FILE:  /home/msis/projects/moos-tutorials/01-getting-around-vehicle-1/src/uLightSensor/CMakeLists.txt
# DATE:  2016/11/03
# INFO:  Source-level CMakeLists.txt file for the uLightSensor project
# NAME:  Your Name Here
#===========================================================================

SET(SRC
  LightSensor.cpp
  LightSensor_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(uLightSensor ${SRC})

TARGET_LINK_LIBRARIES(uLightSensor
  ${MOOS_LIBRARIES}
  ${MOOSGeodesy_LIBRARIES}
  mbutil
  apputil
  m
  pthread
)


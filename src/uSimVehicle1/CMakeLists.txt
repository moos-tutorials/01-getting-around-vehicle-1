#===========================================================================
# FILE:  /home/msis/projects/moos-tutorials/01-getting-around-vehicle-1/src/uSimVehicle1/CMakeLists.txt
# DATE:  2016/11/03
# INFO:  Source-level CMakeLists.txt file for the uSimVehicle1 project
# NAME:  Your Name Here
#===========================================================================

SET(SRC
  SimVehicle1.cpp
  SimVehicle1_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(uSimVehicle1 ${SRC})

TARGET_LINK_LIBRARIES(uSimVehicle1
  ${MOOS_LIBRARIES}
  ${MOOSGeodesy_LIBRARIES}
  geometry
  mbutil
  apputil
  m
  pthread
)

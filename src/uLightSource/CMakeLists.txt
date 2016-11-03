#===========================================================================
# FILE:  /home/msis/projects/moos-tutorials/01-getting-around-vehicle-1/src/uLightSource/CMakeLists.txt
# DATE:  2016/11/01
# INFO:  Source-level CMakeLists.txt file for the uLightSource project
# NAME:  Your Name Here
#===========================================================================

SET(SRC
  LightSource.cpp
  LightSource_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(uLightSource ${SRC})

TARGET_LINK_LIBRARIES(uLightSource
  ${MOOS_LIBRARIES}
  ${MOOSGeodesy_LIBRARIES}
  mbutil
  apputil
  m
  pthread
)


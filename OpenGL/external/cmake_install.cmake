# Install script for directory: /Users/shaikazmath/Desktop/Desktop Folder/Hackathon/STEM 2014-2015/OpenGL/OpenGL-tutorial_v0014_21/external

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/Users/shaikazmath/Desktop/Desktop Folder/Hackathon/STEM 2014-2015/OpenGL/external/glfw-3.0.3/cmake_install.cmake")
  include("/Users/shaikazmath/Desktop/Desktop Folder/Hackathon/STEM 2014-2015/OpenGL/external/assimp-3.0.1270/contrib/zlib/cmake_install.cmake")
  include("/Users/shaikazmath/Desktop/Desktop Folder/Hackathon/STEM 2014-2015/OpenGL/external/assimp-3.0.1270/code/cmake_install.cmake")
  include("/Users/shaikazmath/Desktop/Desktop Folder/Hackathon/STEM 2014-2015/OpenGL/external/bullet-2.81-rev2613/src/BulletSoftBody/cmake_install.cmake")
  include("/Users/shaikazmath/Desktop/Desktop Folder/Hackathon/STEM 2014-2015/OpenGL/external/bullet-2.81-rev2613/src/BulletCollision/cmake_install.cmake")
  include("/Users/shaikazmath/Desktop/Desktop Folder/Hackathon/STEM 2014-2015/OpenGL/external/bullet-2.81-rev2613/src/BulletDynamics/cmake_install.cmake")
  include("/Users/shaikazmath/Desktop/Desktop Folder/Hackathon/STEM 2014-2015/OpenGL/external/bullet-2.81-rev2613/src/LinearMath/cmake_install.cmake")

endif()


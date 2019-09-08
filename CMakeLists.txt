cmake_minimum_required( VERSION 3.5 )

# Require C++17 (or later)
set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_CXX_EXTENSIONS OFF )

# Create Project
project(obstacle)
add_executable(obstacle src/main.cpp)

# Set StartUp Project
set_property( DIRECTORY PROPERTY VS_STARTUP_PROJECT "obstacle" )

# OpenCV
find_package( OpenCV REQUIRED )

if( OpenCV_FOUND )
  # Additional Include Directories
  include_directories(obstacle ${OpenCV_INCLUDE_DIRS} )
  

  # Additional Dependencies
  
  target_link_libraries(obstacle ${OpenCV_LIBS})
  if( NOT WIN32 )
      target_link_libraries(obstacle stdc++fs)
  endif()
endif()


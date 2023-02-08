cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

#---------------------------
set( PackageName DDCry )
#---------------------------

project(${PackageName})

# project version
SET( ${PackageName}_VERSION_MAJOR 0 )
SET( ${PackageName}_VERSION_MINOR 1 )
SET( ${PackageName}_VERSION_PATCH 0 )

SET( ${PackageName}_VERSION "${${PackageName}_VERSION_MAJOR}.${${PackageName}_VERSION_MINOR}" )
SET( ${PackageName}_SOVERSION "${${PackageName}_VERSION_MAJOR}.${${PackageName}_VERSION_MINOR}" )

set(CMAKE_MODULE_PATH  ${CMAKE_MODULE_PATH}  ${CMAKE_SOURCE_DIR}/cmake ) 
set(LIBRARY_OUTPUT_PATH    ${PROJECT_BINARY_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

#------------- set the default installation directory to be the source directory

IF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  SET( CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR} CACHE PATH  
    "install prefix path  - overwrite with -D CMAKE_INSTALL_PREFIX = ..." 
    FORCE )
  MESSAGE(STATUS "CMAKE_INSTALL_PREFIX is ${CMAKE_INSTALL_PREFIX} - overwrite with -D CMAKE_INSTALL_PREFIX" )
ENDIF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)



#---Options-------------------------------------------------------------------------

option(BUILD_TESTING "Enable and build tests" ON)
option(CMAKE_MACOSX_RPATH "Build with rpath on macos" ON)
option(INSTALL_COMPACT_FILES "Copy compact files to install area" OFF)

#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


find_package(DD4hep REQUIRED COMPONENTS DDG4 )

set(sources ./src/DDCryPlugin.cpp )

add_dd4hep_plugin(${PackageName} SHARED ${sources})

target_include_directories(${PackageName}   PRIVATE ${CMAKE_SOURCE_DIR}/cry/src )

target_link_libraries(${PackageName} DD4hep::DDG4 -L${CMAKE_CURRENT_LIST_DIR}/cry/lib/ libCRY.a)

#Create this_package.sh file, and install
dd4hep_instantiate_package(${PackageName})

#---Testing-------------------------------------------------------------------------
if(BUILD_TESTING)

  include(CTest)
  enable_testing()
  add_subdirectory(cryTests)
  set(BUILDNAME "${CMAKE_SYSTEM}-${CMAKE_CXX_COMPILER}-${CMAKE_BUILD_TYPE}" CACHE STRING "set build string for cdash")

endif(BUILD_TESTING)

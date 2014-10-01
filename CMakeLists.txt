cmake_minimum_required(VERSION 2.8.10)

set(exec main)
set(comm communication)
set(model model)
set(util util)

project(roboime-control)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules)
include(${PROJECT_SOURCE_DIR}/cmake/Utils.cmake)

standard_paths(${PROJECT_SOURCE_DIR} bin lib)
#message(STATUS "Project src dir: ${PROJECT_SOURCE_DIR}")

if(WIN32)
    # Define minimum Windows version for headers 0x501 = Vista IIRC
    add_definitions(-D_WIN32_WINNT=0x0501)
    add_definitions(-DWINVER=0x0501)
    add_definitions(-DZMQ_STATIC)
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

set(src src)
set(BOOST_ROOT "" CACHE PATH "Boost path")
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
if (WIN32)
    # Avoids needing to distribute the boost dlls
    set(Boost_USE_STATIC_LIBS ON)
else()
    set(Boost_USE_STATIC_LIBS OFF)
endif()

find_package(ZMQ REQUIRED)

find_path(CPPZMQ_INCLUDE_DIRS
    NAMES
    zmq.hpp
    PATHS
    /usr/include
    /usr/local/include
    /opt/local/include
    /sw/include
    ${ZMQ_INCLUDE_DIRS}
)

# find_package(Protobuf REQUIRED)
find_package(LibUSB REQUIRED)
find_package(Boost 1.47.0 COMPONENTS date_time filesystem system thread regex REQUIRED)

include_directories(
    ${Boost_INCLUDE_DIRS}
    ${LibUSB_INCLUDE_DIRS}
    ${ZMQ_INCLUDE_DIRS}
    ${CPPZMQ_INCLUDE_DIRS}
    ${src}
)

set(SOURCES
    ${src}/main.cc
    ${src}/zmq_worker.h
    ${src}/usb_worker.h
    ${src}/zmq_worker.cc
    ${src}/usb_worker.cc
    ${src}/world.cc
    ${src}/move_action.cc
    ${src}/goto_action.cc
    ${src}/kickto_action.cc
    ${src}/move_action.h
    ${src}/goto_action.h
    ${src}/kickto_action.h
    ${src}/robot.cc
    ${src}/world.h
    ${src}/robot.h
    ${src}/config.cc
    ${src}/pid_controller.cc
    ${src}/config.h
    ${src}/pid_controller.h
    ${src}/multicast_worker.h
    ${src}/multicast_worker.cc
)

set(app ${CMAKE_PROJECT_NAME})
add_executable(${app} ${SOURCES} ${HEADERS})
target_link_libraries(${app} ${Boost_LIBRARIES} ${ZMQ_LIBRARY} ${LibUSB_LIBRARIES})

# vim: et sw=4 ts=4 sts=4

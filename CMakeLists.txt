cmake_minimum_required(VERSION 2.8.10)

set(exec main)
set(comm communication)
set(model model)
set(util util)

project(${exec} C CXX)
project(${comm} C CXX)
project(${model} C CXX)
project(${util} C CXX)
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

find_package(LibUSB REQUIRED)
find_package(Boost 1.47.0 COMPONENTS date_time filesystem system thread regex REQUIRED)

include_directories(
    ${Boost_INCLUDE_DIRS}
    ${LibUSB_INCLUDE_DIRS}
    ${ZMQ_INCLUDE_DIRS}
    ${CPPZMQ_INCLUDE_DIRS}
    "${src}/${util}"
    "${src}/${exec}"
    "${src}/${comm}"
    "${src}/${model}"
)

set(Exec_SOURCES
    ${src}/${exec}/main.cc
)
set(Exec_HEADERS
)

set(Comm_SOURCES
    ${src}/${comm}/zmq_worker.h
    ${src}/${comm}/usb_worker.h
)

set(Comm_HEADERS
    ${src}/${comm}/zmq_worker.cc
    ${src}/${comm}/usb_worker.cc
)

set(Model_SOURCES
    ${src}/${model}/world.cc
    ${src}/${model}/move_action.cc
    ${src}/${model}/goto_action.cc
    ${src}/${model}/robot.cc
)

set(Model_HEADERS
    ${src}/${model}/world.h
    ${src}/${model}/action.h
    ${src}/${model}/move_action.h
    ${src}/${model}/goto_action.h
    ${src}/${model}/robot.h
)

set(Util_SOURCES
    ${src}/${util}/config.cc
    ${src}/${util}/pid_controller.cc
)

set(Util_HEADERS
    ${src}/${util}/config.h
    ${src}/${util}/pid_controller.h
)

add_library(${comm} ${Comm_SOURCES} ${Comm_HEADERS})
add_library(${model} ${Model_SOURCES} ${Model_HEADERS})
add_library(${util} ${Util_SOURCES} ${Util_HEADERS})
add_executable(${CMAKE_PROJECT_NAME} ${Exec_SOURCES} ${Exec_HEADERS})

target_link_libraries(${model} ${util} ${Boost_LIBRARIES} ${LibUSB_LIBRARIES})
target_link_libraries(${comm} ${util} ${model} ${Boost_LIBRARIES} ${ZMQ_LIBRARY}  ${LibUSB_LIBRARIES})
target_link_libraries(${util} ${Boost_LIBRARIES} ${LibUSB_LIBRARIES})
target_link_libraries(${CMAKE_PROJECT_NAME} ${comm} ${model} ${util} ${Boost_LIBRARIES} ${ZMQ_LIBRARY} ${LibUSB_LIBRARIES})

# vim: et sw=4 ts=4 sts=4

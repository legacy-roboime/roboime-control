cmake_minimum_required (VERSION 2.8)

# Custom CMake directives
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMake")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

set(exec main)
set(core core)
set(comm communication)
set(model model)
set(util util)

project(${exec} C CXX)
project(${core} C CXX)
project(${comm} C CXX)
project(${model} C CXX)
project(${util} C CXX)

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
    set(Boost_USE_STATIC_LIBS ON) 
else()
    set(Boost_USE_STATIC_LIBS OFF)
endif()

set(Zmq_INCLUDE_DIRS "" CACHE PATH "ZeroMQ 3 include directories")
set(Zmq_LIBRARY_RELEASE "" CACHE FILEPATH "ZeroMQ 3 library")
set(Zmq_LIBRARY_DEBUG "" CACHE FILEPATH "ZeroMQ 3 library")
set(Zmq_LIBRARY optimized ${Zmq_LIBRARY_RELEASE} debug ${Zmq_LIBRARY_DEBUG})

set(Usb_INCLUDE_DIRS "" CACHE PATH "libusb include directories")
set(Usb_LIBRARY_RELEASE "" CACHE FILEPATH "libusb library")
set(Usb_LIBRARY_DEBUG "" CACHE FILEPATH "libusb library")
set(Usb_LIBRARY optimized ${Usb_LIBRARY_RELEASE} debug ${Usb_LIBRARY_DEBUG})

find_package(Boost 1.47.0 COMPONENTS date_time filesystem system thread regex) 

if(Boost_FOUND)
    include_directories(
        ${Boost_INCLUDE_DIRS} 
        ${Zmq_INCLUDE_DIRS} 
        ${Usb_INCLUDE_DIRS} 
        "${src}/${util}" 
        "${src}/${exec}" 
        "${src}/${comm}" 
        "${src}/${model}" 
        "${src}/${core}" 
    ) 

    set(Exec_SOURCES 
        ${src}/${exec}/main.cc
    )
    set(Exec_HEADERS 
    )

    set(Core_SOURCES
    )

    set(Core_HEADERS 
        ${src}/${core}/core_worker.h
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
        ${src}/${model}/action.cc
        ${src}/${model}/robot.cc
    )

    set(Model_HEADERS 
        ${src}/${model}/world.h
        ${src}/${model}/action.h
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
    add_library(${core} ${Core_SOURCES} ${Core_HEADERS}) 
    add_library(${util} ${Util_SOURCES} ${Util_HEADERS}) 
    add_executable(${exec} ${Exec_SOURCES} ${Exec_HEADERS}) 

    target_link_libraries(${model} ${util} ${Boost_LIBRARIES} ${Usb_LIBRARY})
    target_link_libraries(${comm} ${util} ${model} ${Boost_LIBRARIES} ${Zmq_LIBRARY}  ${Usb_LIBRARY})
    target_link_libraries(${util} ${Boost_LIBRARIES} ${Usb_LIBRARY})
    target_link_libraries(${core} ${comm} ${model} ${util} ${Boost_LIBRARIES} ${Zmq_LIBRARY} ${Usb_LIBRARY})
    target_link_libraries(${exec} ${core} ${comm} ${model} ${util} ${Boost_LIBRARIES} ${Zmq_LIBRARY} ${Usb_LIBRARY})
else()
    message( SEND_ERROR "Boost is required!" )
endif()

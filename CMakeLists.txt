cmake_minimum_required(VERSION 3.12.0)
project(gnss_utils)

find_package(Eigen3 3.3 REQUIRED NO_MODULE)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/findDeps.cmake)

find_deps(geometry https://github.com/superjax/geometry)

include_directories(gnss_utils
    include)
add_library(gnss_utils SHARED
    src/gtime.cpp
    src/datetime.cpp
    src/satellite.cpp
    )
target_link_libraries(gnss_utils
    Eigen3::Eigen stdc++fs geometry)

target_compile_options(gnss_utils PUBLIC -std=c++17)

option(BUILD_TESTS "Build Tests" OFF)
if (BUILD_TESTS)
    find_package(GTest REQUIRED)
    add_definitions(-DGNSS_UTILS_DIR="${CMAKE_CURRENT_LIST_DIR}")
    add_executable(gnss_utils_test
        src/test/test_time.cpp
        src/test/test_wgs84.cpp
        src/test/test_satellite.cpp
        src/reference/reference_algorithms.cpp
        src/test/test_bit_utils.cpp)
    target_link_libraries(gnss_utils_test
            gtest
            gtest_main
            pthread
            gnss_utils)
    target_include_directories(gnss_utils_test PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>  
        $<INSTALL_INTERFACE:include/>)
endif()

option(BUILD_ROS_EXAMPLE "Build ROS Example" OFF)
if (BUILD_ROS_EXAMPLE)
    find_package(catkin REQUIRED COMPONENTS
    roscpp
    nav_msgs
    geometry_msgs
    sensor_msgs
    inertial_sense)
    message(STATUS "Building gnss_utils ROS demo")
    add_message_files(FILES SatInfo.msg)
    generate_messages(DEPENDENCIES std_msgs geometry_msgs inertial_sense)
    catkin_package(
        INCLUDE_DIRS include
        CATKIN_DEPENDS geometry_msgs nav_msgs inertial_sense sensor_msgs
        LIBRARIES gnss_utils)
    add_executable(point_position_demo
        src/positioning_demo.cpp)
    # target_include_directories(point_position_demo PUBLIC
    #     $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>  
    #     $<INSTALL_INTERFACE:include/>
    #     ${catkin_INCLUDE_DIRS})
    target_link_libraries(point_position_demo gnss_utils ${catkin_LIBRARIES})
    add_dependencies(point_position_demo
        gnss_utils_generate_messages_cpp)

    # install package.xml for ROS release
    install(FILES package.xml DESTINATION share/${PROJECT_NAME})
else()
    message(STATUS "Disabling ROS Example Node Build")
endif()


# install
set(LIB_DEST lib/${PROJECT_NAME})
set(INCLUDE_DEST include)

install(TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}-targets
  ARCHIVE DESTINATION ${LIB_DEST}
  LIBRARY DESTINATION ${LIB_DEST}
)
install(DIRECTORY include/${PROJECT_NAME}
  DESTINATION ${INCLUDE_DEST}
  FILES_MATCHING PATTERN "*.h"
)

# install CMake package configuration
install(EXPORT ${PROJECT_NAME}-targets DESTINATION ${LIB_DEST})
install(FILES ${PROJECT_NAME}-config.cmake DESTINATION ${LIB_DEST})


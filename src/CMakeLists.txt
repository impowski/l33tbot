cmake_minimum_required(VERSION 3.4)
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -lpthread")

find_package(Boost REQUIRED system)
if(NOT Boost_FOUND)
  message(SEND_ERROR "Failed to find Boost.")
  return()
else()
  include_directories(${Boost_INCLUDE_DIRS})
endif()

set(SOURCES
  main.cpp
  impbot.cpp
  twitch_api.cpp
  twitch_irc.cpp
  )

set(HEADERS
  impbot.hpp
  twitch_api.hpp
  twitch_irc.hpp
  )

add_executable(twitch_api ${SOURCES} ${HEADERS})
target_link_libraries(twitch_api ${Boost_LIBRARIES})

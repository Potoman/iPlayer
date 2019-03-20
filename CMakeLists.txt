cmake_minimum_required (VERSION 3.10.0)
project (iplayer)

add_executable(${PROJECT_NAME} src/Player.cpp src/Track.cpp src/Library.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC
  clipp/include
  rxterm/rxterm
  include
)
target_link_libraries (${PROJECT_NAME} pthread)


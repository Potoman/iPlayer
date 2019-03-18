cmake_minimum_required (VERSION 3.10.0)
project (hello)

add_executable(${PROJECT_NAME} src/main.cpp src/Player.cpp src/Track.cpp src/Library.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC
  clipp/include
  rxterm/rxterm
  include
)


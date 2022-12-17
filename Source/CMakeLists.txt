cmake_minimum_required(VERSION 3.10.0)

FILE(GLOB_RECURSE SouceFiles *.cpp *.h)

add_executable(${PROJECT_NAME} ${SouceFiles})

# Link to submodules here.
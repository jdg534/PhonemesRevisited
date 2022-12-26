cmake_minimum_required(VERSION 3.10.0)

FILE(GLOB_RECURSE SouceFiles *.cpp *.h)

add_executable(${PROJECT_NAME} ${SouceFiles})

# Link to submodules here.
target_include_directories(${PROJECT_NAME} PUBLIC SignalAnalysisHeaderDir)
target_link_libraries(${PROJECT_NAME} SignalAnalysis)

target_include_directories(${PROJECT_NAME} PUBLIC SdlHeaderDir)
target_link_libraries(${PROJECT_NAME} SDL3-static)

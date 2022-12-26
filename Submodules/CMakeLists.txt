cmake_minimum_required(VERSION 3.10.0)

# folders for submodule setup here.

add_subdirectory(SignalAnalysis/SignalAnalysisLib)
add_subdirectory(sdl)

set(SdlHeaderDir ${CMAKE_CURRENT_SOURCE_DIR}/sdl/include CACHE INTERNAL "" FORCE)

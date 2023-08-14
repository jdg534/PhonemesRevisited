cmake_minimum_required(VERSION 3.10.0)

FILE(GLOB_RECURSE SouceFiles *.cpp *.h)

add_executable(TestExecutable ${SouceFiles})

# Link to submodules here.
target_include_directories(TestExecutable PUBLIC ${SignalAnalysisHeaderDir})
target_link_libraries(TestExecutable SignalAnalysis)

target_include_directories(TestExecutable PUBLIC SdlHeaderDir)
target_link_libraries(TestExecutable SDL3-static)

# nlohmann_json
target_include_directories(TestExecutable PUBLIC JsonHeaderDir)
target_link_libraries(TestExecutable nlohmann_json)


# Copy the phoneme folder to the output directory.
add_custom_command(
	TARGET TestExecutable POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
			"${PROJECT_SOURCE_DIR}/Phonemes"
			${CMAKE_CURRENT_BINARY_DIR}/Phonemes
)

# Copy the Visemes folder to the output directory.
add_custom_command(
	TARGET TestExecutable POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
			"${PROJECT_SOURCE_DIR}/Visemes"
			${CMAKE_CURRENT_BINARY_DIR}/Visemes
)

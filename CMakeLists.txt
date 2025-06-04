cmake_minimum_required(VERSION 3.15)
project(CoolProject
	LANGUAGES
		CXX
)

# Use C11 and strict warnings
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Later: include(helper modules here)
include("${CMAKE_CURRENT_LIST_DIR}/cmake/check.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/cmake/common.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/cmake/add_defines.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/cmake/add_target.cmake")

# Add subprojects
# add_subdirectory(projects/p01)
add_subdirectory(projects/roco_core)

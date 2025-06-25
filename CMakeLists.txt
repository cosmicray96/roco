cmake_minimum_required(VERSION 3.15)
project(CoolProject
	LANGUAGES
		C
)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Later: include(helper modules here)
include("${CMAKE_CURRENT_LIST_DIR}/cmake/common.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/cmake/check.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/cmake/add_defines.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/cmake/add_target.cmake")

# Add subprojects
# add_subdirectory(projects/p01)

include("${CMAKE_CURRENT_LIST_DIR}/cmake/custom_build.cmake")

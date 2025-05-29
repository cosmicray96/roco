add_dynamic_target("project03")


if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/")
	if(testing_build)
		add_subdirectory("./tests")
	endif()
endif()

add_static_target("roco_core")
	
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/")
	if(testing_build)
		add_subdirectory("./tests")
	endif()
endif()


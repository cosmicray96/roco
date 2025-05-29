add_executable_target("project02")
target_link_libraries("project02" 
	PRIVATE
		"project01")
target_link_libraries("project02" 
	PRIVATE
		"project03"
)
	
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/")
	if(testing_build)
		add_subdirectory("./tests")
	endif()
endif()


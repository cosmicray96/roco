if(DEFINED passed_projects)
	foreach(proj ${passed_projects})
		add_subdirectory("${CMAKE_SOURCE_DIR}/projects/${proj}")
	endforeach()

	if(DEFINED passed_tests)
		foreach(proj ${passed_projects})
			add_subdirectory("${CMAKE_SOURCE_DIR}/projects/${proj}/tests/")
		endforeach()
	endif()
endif()




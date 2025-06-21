if(DEFINED passed_projects)
	foreach(proj IN LIST ${passed_projects})
		add_subdirectory(${CMAKE_SOURCE_DIR}/projects/${proj})
	endforeach()
else()
    message(WARNING "No projects passed")
endif()


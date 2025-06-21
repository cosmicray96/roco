if(DEFINED passed_projects)
	foreach(proj ${passed_projects})
		add_subdirectory(${CMAKE_SOURCE_DIR}/projects/${proj})
	endforeach()
else()
	message(WARNING "No projects passed")
endif()


if(DEFINED passed_tests)

	file(GLOB subdirs LIST_DIRECTORIES true CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/projects/*")

	set(project_dirs "")
	foreach(entry ${subdirs})
		if(IS_DIRECTORY "${entry}")
			list(APPEND project_dirs "${entry}")
		endif()
	endforeach()

	foreach(entry ${project_dirs})
		if(EXISTS "${entry}/tests/CMakeLists.txt")
			add_subdirectory("${entry}/tests/")
		endif()
	endforeach()

endif()


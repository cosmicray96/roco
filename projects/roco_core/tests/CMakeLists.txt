set(project_name "roco_core")
set(tests 
	"roco_core_first_test"
	"roco_core_array_test"
	"roco_core_iterator_test"
	"roco_core_ptr_test"
	"roco_core_allocators_test"
)

if(DEFINED passed_tests)
	foreach(test IN LIST ${tests})

		list(FIND passed_tests "${project_name}/${test}" index)
		if(NOT index STREQUAL "-1")
			add_custom_test("${test}" "${project_name}")
		endif()

	endforeach()
endif()


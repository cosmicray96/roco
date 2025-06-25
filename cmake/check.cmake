
if("${language}" STREQUAL "CPP")
	if(NOT CMAKE_CXX_COMPILER)
		message(FATAL_ERROR "C compiler not found!")
	endif()
	if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
		message(FATAL_ERROR "Only Clang C compiler is supported!")
	endif()
	
elseif("${language}" STREQUAL "C")

	if(NOT CMAKE_C_COMPILER)
		message(FATAL_ERROR "C compiler not found!")
	endif()
	if(NOT CMAKE_C_COMPILER_ID STREQUAL "Clang")
		message(FATAL_ERROR "Only Clang C compiler is supported!")
	endif()

endif()

if(NOT CMAKE_SYSTEM_NAME STREQUAL "Linux")
	message(FATAL_ERROR "Only builds for linux is supported!")
endif()

// export.h
#pragma once

// This file must be included after defining:
//   - <LIBNAME>_PLATFORM   (1 = Windows, 2 = Linux, etc.)
//   - <LIBNAME>_COMPILER   (1 = GCC, 2 = Clang, 3 = MSVC, etc.)
//   - <LIBNAME>_INSIDE     (Defined if building the library)

#if !defined(project01_PLATFORM)
	#error "project01_PLATFORM not defined"
#endif

#if !defined(project01_COMPILER)
	#error "project01_COMPILER not defined"
#endif

#if project01_PLATFORM == 1	// Windows
	#if defined(project01_INSIDE)
		#define project01_API	__declspec(dllexport)
	#else
		#define project01_API	__declspec(dllimport)
	#endif

#elif project01_COMPILER == 1 || project01_COMPILER == 2	// GCC or Clang (Linux/macOS)
	#define project01_API	__attribute__((visibility("default")))

#else
	#define project01_API
#endif

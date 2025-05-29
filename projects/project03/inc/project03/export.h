// export.h
#pragma once

// This file must be included after defining:
//   - <LIBNAME>_PLATFORM   (1 = Windows, 2 = Linux, etc.)
//   - <LIBNAME>_COMPILER   (1 = GCC, 2 = Clang, 3 = MSVC, etc.)
//   - <LIBNAME>_INSIDE     (Defined if building the library)

#if !defined(project03_PLATFORM)
	#error "project03_PLATFORM not defined"
#endif

#if !defined(project03_COMPILER)
	#error "project03_COMPILER not defined"
#endif

#if project03_PLATFORM == 1	// Windows
	#if defined(project03_INSIDE)
		#define project03_API	__declspec(dllexport)
	#else
		#define project03_API	__declspec(dllimport)
	#endif

#elif project03_COMPILER == 1 || project03_COMPILER == 2	// GCC or Clang (Linux/macOS)
	#define project03_API	__attribute__((visibility("default")))

#else
	#define project03_API
#endif

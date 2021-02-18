// Since GRPC actively uses winapi, we need to forbid windows macros
// (such as GetMessage, MemoryBarrier, etc.) in our remaining code.
// To do it, we 'wrap' all the C++ file's including ANY GRPC header files
// content into Allow/Hide WindowsPlatformTypes.
// We're unable to 'isolate' the WinAPI usage within a single C++ file thanks to Unity Build.
#if PLATFORM_WINDOWS
	#pragma warning(pop)
	#ifndef UE4_MINIMAL_WINDOWS_INCLUDE
		#define UE4_MINIMAL_WINDOWS_INCLUDE
	#endif
	#include "Windows/AllowWindowsPlatformTypes.h"
#elif PLATFORM_COMPILER_CLANG
	#pragma clang diagnostic pop
#endif

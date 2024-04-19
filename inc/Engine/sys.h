#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
   	//define something for Windows (32-bit and 64-bit, this part is common)
   	#define ENGINE_WINDOWS
   	#ifdef _WIN64
      //define something for Windows (64-bit only)
	#else
      //define something for Windows (32-bit only)
	#endif
#elif __APPLE__
	#include <TargetConditionals.h>
	#if TARGET_IPHONE_SIMULATOR
		#define ENGINE_IOS
         // iOS, tvOS, or watchOS Simulator
    #elif TARGET_OS_MACCATALYST
		#define ENGINE_IOS
         // Mac's Catalyst (ports iOS API into Mac, like UIKit).
    #elif TARGET_OS_IPHONE
		#define ENGINE_IOS
        // iOS, tvOS, or watchOS device
    #elif TARGET_OS_MAC
		#define ENGINE_OSX
        // Other kinds of Apple platforms
    #else
    	#error "Unknown Apple platform"
    #endif
#elif __ANDROID__
	#define ENGINE_ANDROID
    // Below __linux__ check should be enough to handle Android,
    // but something may be unique to Android.
#elif __linux__
	#define ENGINE_LINUX
    // linux
#elif __unix__ // all unices not caught above
	#define ENGINE_UNIX
    // Unix
#elif defined(_POSIX_VERSION)
	#define ENGINE_POSIX
    // POSIX
#else
	#error "Unknown compiler"
#endif
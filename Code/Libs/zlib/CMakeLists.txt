#START-FILE-LIST
# File List auto generated by waf2cmake.py, do not modify manually.

start_sources()
# -- zlib.waf_files --
sources_platform(ALL)
add_sources("NoUberFile"
	SOURCE_GROUP "Root"
		"../../SDKs/zlib-1.2.11/adler32.c"
		"../../SDKs/zlib-1.2.11/compress.c"
		"../../SDKs/zlib-1.2.11/crc32.c"
		"../../SDKs/zlib-1.2.11/crc32.h"
		"../../SDKs/zlib-1.2.11/deflate.c"
		"../../SDKs/zlib-1.2.11/deflate.h"
		"../../SDKs/zlib-1.2.11/gzclose.c"
		"../../SDKs/zlib-1.2.11/gzguts.h"
		"../../SDKs/zlib-1.2.11/gzlib.c"
		"../../SDKs/zlib-1.2.11/gzread.c"
		"../../SDKs/zlib-1.2.11/gzwrite.c"
		"../../SDKs/zlib-1.2.11/infback.c"
		"../../SDKs/zlib-1.2.11/inffast.c"
		"../../SDKs/zlib-1.2.11/inffast.h"
		"../../SDKs/zlib-1.2.11/inffixed.h"
		"../../SDKs/zlib-1.2.11/inflate.c"
		"../../SDKs/zlib-1.2.11/inflate.h"
		"../../SDKs/zlib-1.2.11/inftrees.c"
		"../../SDKs/zlib-1.2.11/inftrees.h"
		"../../SDKs/zlib-1.2.11/trees.c"
		"../../SDKs/zlib-1.2.11/trees.h"
		"../../SDKs/zlib-1.2.11/uncompr.c"
		"../../SDKs/zlib-1.2.11/zconf.h"
		"../../SDKs/zlib-1.2.11/zlib.h"
		"../../SDKs/zlib-1.2.11/zutil.c"
		"../../SDKs/zlib-1.2.11/zutil.h"
)

end_sources()
#END-FILE-LIST

CryEngineStaticModule(zlib SOLUTION_FOLDER "Libs")

if (WIN32 OR DURANGO)
target_compile_definitions(${THIS_PROJECT} PRIVATE -D_CRT_NONSTDC_NO_DEPRECATE -D_CRT_SECURE_NO_WARNINGS)
endif()

if (ORBIS OR DARWIN OR LINUX OR ANDROID)
	target_compile_definitions(${THIS_PROJECT} PRIVATE -DHAVE_UNISTD_H)
	target_compile_options(${THIS_PROJECT} PRIVATE -w)
endif()
target_include_directories(${THIS_PROJECT} PUBLIC "${SDK_DIR}/zlib-1.2.11")

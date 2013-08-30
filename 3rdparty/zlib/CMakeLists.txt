cmake_minimum_required(VERSION 2.8)

option(LINK_CRT_AS_DLL "If set (default), CrashRpt modules link C run-time (CRT) as multi-threaded dynamic libraries, otherwise as multi-threaded static libs." OFF)





project(zlib)

message(STATUS "Project: zlib")
message(STATUS "Project Directory: ${PROJECT_SOURCE_DIR}")



if(MSVC)
    set(CMAKE_DEBUG_POSTFIX "d")
    add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
    add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR})
endif()



if (MSVC)
    
    # For MSVC, CMake sets certain flags to defaults we want to override.
    # This replacement code is taken from sample in the CMake Wiki at
    # http://www.cmake.org/Wiki/CMake_FAQ#Dynamic_Replace.
    foreach (flag_var
            CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
            CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE )
        if (NOT LINK_CRT_AS_DLL)     					
            string(REPLACE "/MD" "-MT" ${flag_var} "${${flag_var}}")			  
        endif(NOT LINK_CRT_AS_DLL)        

        # We prefer more strict warning checking.
        # Replaces /W3 with /W4 in defaults.
        string(REPLACE "/W3" "-W4" ${flag_var} "${${flag_var}}")
                    
    endforeach()
    
    #SET (CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}" CACHE STRING "MSVC C Debug MT flags " FORCE)    
    #SET (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}" CACHE STRING "MSVC CXX Debug MT flags " FORCE)
    #SET (CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}" CACHE STRING "MSVC C Release MT flags " FORCE)
    #SET (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}" CACHE STRING "MSVC CXX Release MT flags " FORCE)
    
endif(MSVC)


# Set output directory for LIB files
if(CMAKE_CL_64)
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/lib/x64)
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/lib/x64)
else(CMAKE_CL_64)

    # 静态库目录
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/../../lib/win32d)
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/../../lib/win32)

    # 动态库目录
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/../../product/win32d)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/../../product/win32)

    # EXE目录
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/../../product/win32d)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/../../prodcut/win32)
endif(CMAKE_CL_64)


set(CMAKE_CONFIGURATION_TYPES Debug Release)
set(CMAKE_BUILE_TYPE Debug)
set(CMAKE_SUPPRESS_REGENERATION 1)
set(CMAKE_USE_RELATIVE_PATHS ON)
set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS ON)
set(CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}/../../product")

set(3RDPARTY_INCLUDE_PATH W:/3rdparty)


# Define _UNICODE (use wide-char encoding)
add_definitions(-D_UNICODE -D_CRT_SECURE_NO_DEPRECATE /wd4996 /wd4131 /wd4244 /wd4127)


if(WIN32)
    set(GDAL_HDRS ${3RDPARTY_INCLUDE_PATH})
else()
    set(GDAL_HDRS "/home/sunsc/gdal/")
endif()

# 设置头文件引用路径
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${GDAL_HDRS})
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/zlib")


# 设置引用库路径
set(LIBS ${LIBRARY_OUTPUT_PATH})
link_directories(${LIBS})
link_directories("${CMAKE_CURRENT_SOURCE_DIR}/../lib/win32")



# 设置工程.cpp文件
AUX_SOURCE_DIRECTORY(. SRC_LIST)
list(REMOVE_ITEM SRC_LIST ./compress_1.c ./adler32_1.c)

# 添加资源文件
#list(APPEND SRC_LIST ./MFCDemo.rc )

# 设置工程.h文件
file( GLOB header_files *.h )


# 编译库
add_library(zlib STATIC ${SRC_LIST} ${header_files})


if(NOT LINK_CRT_AS_DLL)     					
    set_target_properties(zlib PROPERTIES DEBUG_POSTFIX -mt-d )
    set_target_properties(zlib PROPERTIES RELEASE_POSTFIX -mt )
else(NOT LINK_CRT_AS_DLL)
    set_target_properties(zlib PROPERTIES DEBUG_POSTFIX -md-d )
    set_target_properties(zlib PROPERTIES RELEASE_POSTFIX -md )        
endif(NOT LINK_CRT_AS_DLL) 



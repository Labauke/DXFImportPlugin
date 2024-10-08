# CMakeLists.txt file for DXFImportPlugin and dependent libraries

# Require a fairly recent cmake version
cmake_minimum_required( VERSION 2.8...3.10 )

# The project name
project( DXFImportPlugin )

# enable @rpath on MacOS
cmake_policy(SET CMP0042 NEW)

# -------------------------------------------------------------
# MACRO definitions
# -------------------------------------------------------------

# Macros to hide/show cached variables.
# These two macros can be used to "hide" or "show" in the
# list of cached variables various variables and/or options
# that depend on other options.
# Note that once a variable is modified, it will preserve its
# value (hidding it merely makes it internal)

MACRO(HIDE_VARIABLE var)
  IF(DEFINED ${var})
    SET(${var} "${${var}}" CACHE INTERNAL "")
  ENDIF(DEFINED ${var})
ENDMACRO(HIDE_VARIABLE)

MACRO(SHOW_VARIABLE var type doc default)
  IF(DEFINED ${var})
    SET(${var} "${${var}}" CACHE "${type}" "${doc}" FORCE)
  ELSE(DEFINED ${var})
    SET(${var} "${default}" CACHE "${type}" "${doc}")
  ENDIF(DEFINED ${var})
ENDMACRO(SHOW_VARIABLE)

# -------------------------------------------------------------
# Initial commands
# -------------------------------------------------------------

# Uncomment this to enable detailed make output
#set( CMAKE_VERBOSE_MAKEFILE ON )

# Hide some more cache variables to keep things tidy
MARK_AS_ADVANCED(CMAKE_BACKWARDS_COMPATIBILITY)
MARK_AS_ADVANCED(EXECUTABLE_OUTPUT_PATH LIBRARY_OUTPUT_PATH)

# Set default build type
if (NOT CMAKE_BUILD_TYPE)
	set( CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING
		"Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif (NOT CMAKE_BUILD_TYPE)

if (MSVC)

	# add VC compiler-specific flags (NONMINMAX define and disable "unsafe" warnings)
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D\"NOMINMAX\" /wd4996" )

else (MSVC)

	# on Unix we want really detailed warnings
	ADD_DEFINITIONS( -Wall -fPIC)
	# when building with gcc/icc add compile flag
	if (APPLE)
		#message( STATUS "Mac compiler ID = ${CMAKE_C_COMPILER_ID}" )
		if ( CMAKE_C_COMPILER_ID MATCHES "GNU")
			set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" )
		else ( CMAKE_C_COMPILER_ID MATCHES "GNU")
			set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++" )
		endif ( CMAKE_C_COMPILER_ID MATCHES "GNU")
	else (APPLE)
		set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -std=gnu++11" )
	endif (APPLE)

endif (MSVC)


# -------------------------------------------------------------
# Packages
# -------------------------------------------------------------

# automatically add CMAKE_CURRENT_SOURCE_DIR and CMAKE_CURRENT_BINARY_DIR to the include directories in every processed CMakeLists.txt
set(CMAKE_INCLUDE_CURRENT_DIR ON)

if (DISABLE_QT)
	message("*** Building without Qt5 ***")
else (DISABLE_QT)
	find_package(Qt5Widgets REQUIRED)
	find_package(Qt5Xml REQUIRED)
	find_package(Qt5Svg REQUIRED)
	find_package(Qt5PrintSupport REQUIRED)
	find_package(Qt5Network REQUIRED)
	find_package(Qt5Concurrent REQUIRED)

	message("*** Building with Qt5, Version ${Qt5Widgets_VERSION} ***")
	
	# we need OpenGL
	find_package( OpenGL REQUIRED )

endif (DISABLE_QT)

if( APPLE )
	find_library(IOKIT NAMES IOKit REQUIRED)
	find_library(COREFOUNDATION NAMES CoreFoundation REQUIRED)
	find_library(SECURITY_FRAMEWORK Security REQUIRED)
	find_library(SYSTEM_CONFIGURATION SystemConfiguration REQUIRED)

	set(APPLE_FRAMEWORKS
		${IOKIT}
		${SECURITY_FRAMEWORK}
		${COREFOUNDATION}
		${SYSTEM_CONFIGURATION}
	)
endif( APPLE )

# -------------------------------------------------------------
# Support for build flags
# -------------------------------------------------------------
include( ../../externals/IBK/projects/cmake_local/cmake_pri/CMakeLists.txt  )

# -------------------------------------------------------------
# subdirectories and subprojects
# -------------------------------------------------------------

# add subdirectories for all builds
add_subdirectory( ../../externals/IBK/projects/cmake_local IBK )
add_subdirectory( ../../externals/IBKMK/projects/cmake_local IBKMK )
add_subdirectory( ../../externals/libdxfrw/projects/cmake_local libdxfrw )
add_subdirectory( ../../externals/TiCPP/projects/cmake_local TiCPP )

if (NOT DISABLE_QT)
	message("*** Building Qt-based libs ***")	
	add_subdirectory( ../../externals/QtExt/projects/cmake_local QtExt )
	add_subdirectory( ../../externals/DXFImportPlugin/projects/cmake_local DXFImportPlugin )
endif()


# -------------------------------------------------------------
# applications
# -------------------------------------------------------------


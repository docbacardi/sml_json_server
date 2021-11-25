cmake_minimum_required(VERSION 3.7)

PROJECT("sml_json_server")


# Get the VCS version for the jonchki configuration.
INCLUDE(cmake/version.cmake)
INCLUDE(cmake/platform.cmake)
MESSAGE("Building for: ${JONCHKI_PLATFORM}")

IF(CMAKE_VERSION VERSION_LESS 3.12.0)
	FIND_PACKAGE(PythonInterp 3.5 REQUIRED)
	SET(PYTHON_EXE "${PYTHON_EXECUTABLE}")
ELSE(CMAKE_VERSION VERSION_LESS 3.12.0)
	FIND_PACKAGE(Python3 REQUIRED)
	SET(PYTHON_EXE "${Python3_EXECUTABLE}")
ENDIF(CMAKE_VERSION VERSION_LESS 3.12.0)

# Enable tests (see here for details: http://www.vtk.org/Wiki/CMake/Testing_With_CTest)
ENABLE_TESTING()

#----------------------------------------------------------------------------
#
# Include the projects in the subdirectories.
#
ADD_SUBDIRECTORY(external)
ADD_SUBDIRECTORY(server)

#----------------------------------------------------------------------------


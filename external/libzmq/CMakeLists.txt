cmake_minimum_required(VERSION 3.8)

PROJECT("libzmq")

INCLUDE(ExternalProject)

SET(VERSION_EXTERNAL "4.3.4")

#----------------------------------------------------------------------------
#
# Build the project.
#
IF(${CMAKE_CROSSCOMPILING})
    IF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	# Get the prefix of the compiler.
	GET_FILENAME_COMPONENT(GCC_BASENAME ${CMAKE_C_COMPILER} NAME)
	IF(${GCC_BASENAME} MATCHES "([^-]+-[^-]+-[^-]+)-gcc")
	    SET(CONFIGURE_HOST "--host=${CMAKE_MATCH_1}")
	ELSE(${GCC_BASENAME} MATCHES "([^-]+-[^-]+-[^-]+)-gcc")
	    MESSAGE(FATAL_ERROR "Failed to extract the compiler prefix from the C compiler ${CMAKE_C_COMPILER}")
	ENDIF(${GCC_BASENAME} MATCHES "([^-]+-[^-]+-[^-]+)-gcc")
    ELSE("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	MESSAGE(FATAL_ERROR "Cross compiling detected, but not using GCC. This is currently not supported by the CMake wrapper for OpenSSL.")
    ENDIF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
ELSE(${CMAKE_CROSSCOMPILING})
    SET(CONFIGURE_HOST "")
ENDIF(${CMAKE_CROSSCOMPILING})

ExternalProject_Add(TARGET_zeromq
                    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/zeromq
                    URL ${CMAKE_CURRENT_SOURCE_DIR}/zeromq-${VERSION_EXTERNAL}.tar.gz
                    URL_HASH SHA1=47277a64749049123d1401600e8cfbab10a3ae28
                    CONFIGURE_COMMAND ${CMAKE_CURRENT_BINARY_DIR}/zeromq/src/TARGET_zeromq/configure --prefix=${CMAKE_CURRENT_BINARY_DIR}/install --enable-static=yes --enable-shared=no --with-pic ${CONFIGURE_HOST}
                    INSTALL_COMMAND ${CMAKE_MAKE_PROGRAM} install
)

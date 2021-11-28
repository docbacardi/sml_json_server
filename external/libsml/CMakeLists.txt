cmake_minimum_required(VERSION 3.8)

PROJECT("libsml")

INCLUDE(ExternalProject)

SET(VERSION_EXTERNAL "d656822")


#----------------------------------------------------------------------------
#
# Build the project.
#

ExternalProject_Add(TARGET_libsml
                    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/libsml
                    URL ${CMAKE_CURRENT_SOURCE_DIR}/libsml-${VERSION_EXTERNAL}.zip
                    URL_HASH SHA1=65d0869cb880c4836b8998f8f1bc32f42649f68a
                    CONFIGURE_COMMAND pwd
                    BUILD_IN_SOURCE TRUE
                    BUILD_COMMAND pwd && make -C sml
                    INSTALL_COMMAND pwd
)

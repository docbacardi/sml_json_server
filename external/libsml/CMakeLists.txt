cmake_minimum_required(VERSION 3.8)

PROJECT("libsml")

INCLUDE(ExternalProject)

SET(VERSION_EXTERNAL "d656822")


#----------------------------------------------------------------------------
#
# Build the project.
#

SET(PRJ_CMAKE_ARGS "")

LIST(APPEND PRJ_CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/install")
LIST(APPEND PRJ_CMAKE_ARGS "-DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}")

ExternalProject_Add(TARGET_libsml
                    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/libsml
                    URL ${CMAKE_CURRENT_SOURCE_DIR}/libsml-${VERSION_EXTERNAL}.zip
                    URL_HASH SHA1=65d0869cb880c4836b8998f8f1bc32f42649f68a
                    SOURCE_SUBDIR sml
                    PATCH_COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/copy_over/CMakeLists.txt ${CMAKE_CURRENT_BINARY_DIR}/libsml/src/TARGET_libsml/sml/
                    CMAKE_ARGS ${PRJ_CMAKE_ARGS}
                    INSTALL_COMMAND make install
)

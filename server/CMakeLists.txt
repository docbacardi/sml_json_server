PROJECT("server")

SET(PROJECT_VERSION "1.0.0")

SET(CJSON_DIR "${CMAKE_BINARY_DIR}/external/cJSON/install")
SET(LIBSML_DIR "${CMAKE_BINARY_DIR}/external/libsml/install")
SET(LIBZMQ_DIR "${CMAKE_BINARY_DIR}/external/libzmq/install")

ADD_EXECUTABLE(TARGET_server
               server.cpp)
TARGET_LINK_LIBRARIES(TARGET_server "${CJSON_DIR}/lib/libcjson.a" "${LIBSML_DIR}/lib/libsml.a" "${LIBZMQ_DIR}/lib/libzmq.a" "-luuid" "-lm" "-pthread")
TARGET_INCLUDE_DIRECTORIES(TARGET_server
                           PRIVATE "${CJSON_DIR}/include" "${LIBSML_DIR}/include" "${LIBZMQ_DIR}/include")

# On mingw link all compiler libraries static.
IF((CMAKE_SYSTEM_NAME STREQUAL "Windows") AND (${CMAKE_COMPILER_IS_GNUCC}))
    SET_PROPERTY(TARGET TARGET_server PROPERTY LINK_FLAGS "-static -static-libgcc -static-libstdc++")
ENDIF((CMAKE_SYSTEM_NAME STREQUAL "Windows") AND (${CMAKE_COMPILER_IS_GNUCC}))

# Set the name of the output file to "sml_json_server".
SET_TARGET_PROPERTIES(TARGET_server PROPERTIES PREFIX "" OUTPUT_NAME "sml_json_server")

# Add tests for this module.
IF((CMAKE_SYSTEM_NAME STREQUAL "Windows") AND (${CMAKE_COMPILER_IS_GNUCC}))
    # Here are the MinGW specific tests.
    ADD_TEST(NAME server_MinGW_DLL_dependencies
        COMMAND "${PYTHON_INTERPRETER}" ${TEST_SCRIPT_DIR}/mingw_dll_dependencies.py $<TARGET_FILE:TARGET_server>)
ENDIF((CMAKE_SYSTEM_NAME STREQUAL "Windows") AND (${CMAKE_COMPILER_IS_GNUCC}))


ADD_EXECUTABLE(TARGET_client
               client.cpp)
TARGET_LINK_LIBRARIES(TARGET_client "${LIBZMQ_DIR}/lib/libzmq.a" "-pthread")
TARGET_INCLUDE_DIRECTORIES(TARGET_client
                           PRIVATE "${LIBZMQ_DIR}/include")

# On mingw link all compiler libraries static.
IF((CMAKE_SYSTEM_NAME STREQUAL "Windows") AND (${CMAKE_COMPILER_IS_GNUCC}))
    SET_PROPERTY(TARGET TARGET_client PROPERTY LINK_FLAGS "-static -static-libgcc -static-libstdc++")
ENDIF((CMAKE_SYSTEM_NAME STREQUAL "Windows") AND (${CMAKE_COMPILER_IS_GNUCC}))

# Set the name of the output file to "demo_client".
SET_TARGET_PROPERTIES(TARGET_client PROPERTIES PREFIX "" OUTPUT_NAME "demo_client")

# Add tests for this module.
IF((CMAKE_SYSTEM_NAME STREQUAL "Windows") AND (${CMAKE_COMPILER_IS_GNUCC}))
    # Here are the MinGW specific tests.
    ADD_TEST(NAME client_MinGW_DLL_dependencies
        COMMAND "${PYTHON_INTERPRETER}" ${TEST_SCRIPT_DIR}/mingw_dll_dependencies.py $<TARGET_FILE:TARGET_client>)
ENDIF((CMAKE_SYSTEM_NAME STREQUAL "Windows") AND (${CMAKE_COMPILER_IS_GNUCC}))

#----------------------------------------------------------------------------
#
# Build a package for the selected platform.
#

SET(CPACK_OUTPUT_FILE_PREFIX ${CMAKE_BINARY_DIR}/../repository/sml_json_server/${PROJECT_VERSION})
SET(CPACK_INCLUDE_TOPLEVEL_DIRECTORY FALSE)
SET(CPACK_PACKAGE_FILE_NAME "sml_json_server-${PROJECT_VERSION}-${JONCHKI_PLATFORM}")

SET(CPACK_GENERATOR "TXZ")
INCLUDE(CPack)

INSTALL(TARGETS TARGET_server TARGET_client
        DESTINATION sml_json_server-${PROJECT_VERSION})

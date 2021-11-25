PROJECT("server")

SET(CJSON_DIR "${CMAKE_BINARY_DIR}/external/cJSON/install")

ADD_EXECUTABLE(TARGET_server
               main.c)
TARGET_LINK_LIBRARIES(TARGET_server "${CJSON_DIR}/lib/libcjson.a")
TARGET_INCLUDE_DIRECTORIES(TARGET_server
                           PRIVATE "${CJSON_DIR}/include")

# On mingw link all compiler libraries static.
IF((CMAKE_SYSTEM_NAME STREQUAL "Windows") AND (${CMAKE_COMPILER_IS_GNUCC}))
    SET_PROPERTY(TARGET TARGET_server PROPERTY LINK_FLAGS "-static -static-libgcc -static-libstdc++")
ENDIF((CMAKE_SYSTEM_NAME STREQUAL "Windows") AND (${CMAKE_COMPILER_IS_GNUCC}))

# Set the name of the output file to "sml_json_server".
SET_TARGET_PROPERTIES(TARGET_server PROPERTIES PREFIX "" OUTPUT_NAME "sml_json_server")

# Add tests for this module.
IF((CMAKE_SYSTEM_NAME STREQUAL "Windows") AND (${CMAKE_COMPILER_IS_GNUCC}))
    # Here are the MinGW specific tests.
    ADD_TEST(NAME romloader_uart_MinGW_DLL_dependencies
        COMMAND "${PYTHON_INTERPRETER}" ${TEST_SCRIPT_DIR}/mingw_dll_dependencies.py $<TARGET_FILE:TARGET_server>)
ENDIF((CMAKE_SYSTEM_NAME STREQUAL "Windows") AND (${CMAKE_COMPILER_IS_GNUCC}))

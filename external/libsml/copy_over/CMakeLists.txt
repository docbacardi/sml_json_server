PROJECT("libsml")

CMAKE_MINIMUM_REQUIRED(VERSION 3.8)

ADD_LIBRARY(TARGET_libsml STATIC
            src/sml_file.c
            src/sml_attention_response.c
            src/sml_transport.c
            src/sml_octet_string.c
            src/sml_shared.c
            src/sml_number.c
            src/sml_message.c
            src/sml_time.c
            src/sml_list.c
            src/sml_status.c
            src/sml_value.c
            src/sml_tree.c
            src/sml_boolean.c
            src/sml_crc16.c
            src/sml_open_request.c
            src/sml_open_response.c
            src/sml_get_list_request.c
            src/sml_get_list_response.c
            src/sml_close_request.c
            src/sml_close_response.c
            src/sml_set_proc_parameter_request.c
            src/sml_get_proc_parameter_request.c
            src/sml_get_proc_parameter_response.c
            src/sml_get_profile_pack_request.c
            src/sml_get_profile_pack_response.c
            src/sml_get_profile_list_request.c
            src/sml_get_profile_list_response.c)
TARGET_LINK_LIBRARIES(TARGET_libsml "-luuid")
TARGET_INCLUDE_DIRECTORIES(TARGET_libsml
                           PUBLIC "include")

# On mingw link all compiler libraries static.
IF((CMAKE_SYSTEM_NAME STREQUAL "Windows") AND (${CMAKE_COMPILER_IS_GNUCC}))
    SET_PROPERTY(TARGET TARGET_libsml PROPERTY LINK_FLAGS "-static -static-libgcc -static-libstdc++")
ENDIF((CMAKE_SYSTEM_NAME STREQUAL "Windows") AND (${CMAKE_COMPILER_IS_GNUCC}))

# Set the name of the output file to "libsml".
SET_TARGET_PROPERTIES(TARGET_libsml PROPERTIES PREFIX "lib" OUTPUT_NAME "sml")

# Add tests for this module.
IF((CMAKE_SYSTEM_NAME STREQUAL "Windows") AND (${CMAKE_COMPILER_IS_GNUCC}))
    # Here are the MinGW specific tests.
    ADD_TEST(NAME libsml_MinGW_DLL_dependencies
        COMMAND "${PYTHON_INTERPRETER}" ${TEST_SCRIPT_DIR}/mingw_dll_dependencies.py $<TARGET_FILE:TARGET_libsml>)
ENDIF((CMAKE_SYSTEM_NAME STREQUAL "Windows") AND (${CMAKE_COMPILER_IS_GNUCC}))

# Install files.
INSTALL(TARGETS TARGET_libsml
        ARCHIVE DESTINATION lib)
INSTALL(DIRECTORY ${CMAKE_SOURCE_DIR}/include
        DESTINATION .)

MESSAGE (STATUS "===== Examples =====")
OPTION (BUILD_EXAMPLES "Build the libplayerc, libplayerc++ and plugin examples" ON)

IF (BUILD_EXAMPLES)
    ADD_SUBDIRECTORY (libplayerc)
    ADD_SUBDIRECTORY (libplayerc++)
    IF (BUILD_SHARED_LIBS)
        ADD_SUBDIRECTORY (plugins)
    ENDIF (BUILD_SHARED_LIBS)
ENDIF (BUILD_EXAMPLES)
MESSAGE (STATUS "====================")
MESSAGE (STATUS "")

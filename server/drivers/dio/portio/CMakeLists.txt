IF (PLAYER_OS_QNX)
    PLAYERDRIVER_OPTION (portio build_portio ON)
    PLAYERDRIVER_REQUIRE_HEADER (portio build_portio hw/inout.h)
ELSE (PLAYER_OS_QNX)
    INCLUDE(CheckSymbolExists)
    CHECK_SYMBOL_EXISTS (inb_p sys/io.h HAVE_INB_P)
    CHECK_SYMBOL_EXISTS (outb_p sys/io.h HAVE_OUTB_P)
    IF (HAVE_INB_P AND HAVE_OUTB_P)
        PLAYERDRIVER_OPTION (portio build_portio ON)
    ELSE (HAVE_INB_P AND HAVE_OUTB_P)
        PLAYERDRIVER_OPTION (portio build_portio OFF)
    ENDIF (HAVE_INB_P AND HAVE_OUTB_P)
    PLAYERDRIVER_REQUIRE_HEADER (portio build_portio sys/io.h)
ENDIF (PLAYER_OS_QNX)
PLAYERDRIVER_REJECT_OS (portio build_portio PLAYER_OS_WIN)
PLAYERDRIVER_ADD_DRIVER (portio build_portio SOURCES portio.cc)

INCLUDE (${PLAYER_CMAKE_DIR}/internal/Utils.cmake)

INCLUDE (CheckFunctionExists)
INCLUDE (CheckIncludeFiles)
INCLUDE (CheckLibraryExists)

CHECK_FUNCTION_EXISTS (cfmakeraw HAVE_CFMAKERAW)
CHECK_FUNCTION_EXISTS (dirname HAVE_DIRNAME)
CHECK_FUNCTION_EXISTS (getaddrinfo HAVE_GETADDRINFO)
CHECK_INCLUDE_FILES (jpeglib.h HAVE_JPEGLIB_H)
CHECK_LIBRARY_EXISTS (GL glBegin "" HAVE_LIBGL)
CHECK_LIBRARY_EXISTS (GLU main "" HAVE_LIBGLU)
CHECK_LIBRARY_EXISTS (glut main "" HAVE_LIBGLUT)
CHECK_LIBRARY_EXISTS (jpeg jpeg_read_header "" HAVE_LIBJPEG)
CHECK_LIBRARY_EXISTS (ltdl lt_dlopenext "" HAVE_LIBLTDL)
CHECK_LIBRARY_EXISTS (z compress2 "" HAVE_LIBZ)
CHECK_INCLUDE_FILES (linux/joystick.h HAVE_LINUX_JOYSTICK_H)
CHECK_FUNCTION_EXISTS (poll HAVE_POLL)
CHECK_FUNCTION_EXISTS (round HAVE_ROUND)
CHECK_INCLUDE_FILES (stdint.h HAVE_STDINT_H)
CHECK_INCLUDE_FILES (strings.h HAVE_STRINGS_H)
CHECK_INCLUDE_FILES (zlib.h HAVE_ZLIB_H)
CHECK_FUNCTION_EXISTS (compressBound NEED_COMPRESSBOUND)
CHECK_INCLUDE_FILES (dns_sd.h HAVE_DNS_SD)
IF (HAVE_DNS_SD)
    CHECK_LIBRARY_EXISTS (dns_sd DNSServiceRefDeallocate "" HAVE_DNS_SD)
ENDIF (HAVE_DNS_SD)

# Endianess check
INCLUDE (TestBigEndian)
TEST_BIG_ENDIAN (WORDS_BIGENDIAN)

# GTK checks
CHECK_PACKAGE_EXISTS (libgnomecanvas-2.0 WITH_GNOMECANVAS GNOMECANVAS_INCLUDEDIR GNOMECANVAS_LIBDIR GNOMECANVAS_LINKFLAGS GNOMECANVAS_CFLAGS)
CHECK_PACKAGE_EXISTS (gtk+-2.0 WITH_GTK GTK_INCLUDEDIR GTK_LIBDIR GTK_LINKFLAGS GTK_CFLAGS)
CHECK_PACKAGE_EXISTS (gdk-pixbuf-2.0 WITH_GDKPIXBUF GDKPIXBUF_INCLUDEDIR GDKPIXBUF_LIBDIR GDKPIXBUF_LINKFLAGS GDKPIXBUF_CFLAGS)
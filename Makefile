#
# Makefile
#

all:    libs test_ioctl

libs:   libwrap_ioctl.so

libwrap_ioctl.so:   wrap_ioctl.c
    rm -f libwrap_ioctl.so*
    gcc -fPIC -shared -Wl,-soname,libwrap_ioctl.so.1 -ldl -o libwrap_ioctl.so.1.0  wrap_ioctl.c
    ln -s libwrap_ioctl.so.1.0 libwrap_ioctl.so.1
    ln -s libwrap_ioctl.so.1 libwrap_ioctl.so

clean: 
    rm -f libwrap_ioctl.so* test_ioctl

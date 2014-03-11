#!/bin/sh

LD_PRELOAD=./libwrap_ioctl.so test_ioctl $@

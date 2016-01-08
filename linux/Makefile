# Makefile for ASPERES (Linux / GCC)
# Copyright (C) L. Menczel, 2010
# laszlo.menczel@gmail.com

src_dir = ../src

obj_dir = ./obj

CFLAGS = -c -Wall -I$(src_dir) -D_LINUX

EXEFLAGS = -Wall -D_LINUX -I$(src_dir) -L.

HDRS = \
       $(src_dir)/dsp.h \
       $(src_dir)/image_io.h \
       $(src_dir)/sound_io.h \
       $(src_dir)/util.h

LIBS = -lfftw3 -lmutil -lm

OBJS = \
      $(obj_dir)/asperes.o \
      $(obj_dir)/dsp.o \
      $(obj_dir)/image_io.o \
      $(obj_dir)/sound_io.o \
      $(obj_dir)/util.o

all: asperes

asperes: $(OBJS)
	$(CC) $(EXEFLAGS) -o asperes $(OBJS) $(LIBS)

$(obj_dir)/asperes.o: $(src_dir)/asperes.c $(HDRS)
	$(CC) $(CFLAGS) -o $(obj_dir)/asperes.o $(src_dir)/asperes.c

$(obj_dir)/dsp.o: $(src_dir)/dsp.c $(src_dir)/dsp.h
	$(CC) $(CFLAGS) -o $(obj_dir)/dsp.o $(src_dir)/dsp.c

$(obj_dir)/image_io.o: $(src_dir)/image_io.c \
        $(src_dir)/image_io.h $(src_dir)/util.h
	$(CC) $(CFLAGS) -o $(obj_dir)/image_io.o $(src_dir)/image_io.c

$(obj_dir)/sound_io.o: $(src_dir)/sound_io.c \
        $(src_dir)/sound_io.h $(src_dir)/util.h
	$(CC) $(CFLAGS) -o $(obj_dir)/sound_io.o $(src_dir)/sound_io.c

$(obj_dir)/util.o: $(src_dir)/util.c $(src_dir)/util.h
	$(CC) $(CFLAGS) -o $(obj_dir)/util.o $(src_dir)/util.c

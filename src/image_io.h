/* 
  image_io.h - prototypes of image save & load functions

  The Analysis & Resynthesis Sound Spectrograph (ARSS)

  Copyright (C) 2005-2008 Michel Rouzic
  Copyright (C) 2010 Laszlo Menczel

  This is free software distributed under the terms of the GNU General
  Public License version 2.

  This program has NO WARRANTY, you use ARSS at your own risk.
*/

#ifndef H_IMAGE_IO
#define H_IMAGE_IO

extern double **bmp_in(FILE * bmpfile, int32_t * y, int32_t * x);
extern void bmp_out(FILE * bmpfile, double **image, int32_t y, int32_t x);

#endif

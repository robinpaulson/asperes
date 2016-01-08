/* 
  image_io.c - functions to read & write image files

  The Analysis & Resynthesis Sound Spectrograph (ARSS)

  Copyright (C) 2005-2008 Michel Rouzic
  Copyright (C) 2010 Laszlo Menczel

  This is free software distributed under the terms of the GNU General
  Public License version 2.

  This program has NO WARRANTY, you use ARSS at your own risk.
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "util.h"
#include "image_io.h"

double **
bmp_in(FILE * bmpfile, int32_t * y, int32_t * x)
{
  int32_t iy, ix, ic;		// various iterators
  int32_t offset;
  double **image;
  uint8_t zerobytes, val;

  if (fread_le_short(bmpfile) != 19778)	// "BM" format tag check
  {
    message("This file is not in BMP format.");
    exit(EXIT_FAILURE);
  }

  fseek(bmpfile, 8, SEEK_CUR);	// skipping useless tags
  offset = fread_le_word(bmpfile) - 54;	// header offset
  fseek(bmpfile, 4, SEEK_CUR);	// skipping useless tags
  *x = fread_le_word(bmpfile);
  *y = fread_le_word(bmpfile);
  fseek(bmpfile, 2, SEEK_CUR);	// skipping useless tags

  if (fread_le_short(bmpfile) != 24)	// Only format supported
  {
    message("Wrong BMP format, BMP images must be in 24-bit colour.");
    exit(EXIT_FAILURE);
  }

  fseek(bmpfile, 24 + offset, SEEK_CUR);	// skipping useless tags

  image = malloc(*y * sizeof(double));	// image allocation
  for (iy = 0; iy < *y; iy++)
    image[iy] = calloc(*x, sizeof(double));


  zerobytes = 4 - ((*x * 3) & 3);
  if (zerobytes == 4)
    zerobytes = 0;

  for (iy = *y - 1; iy != -1; iy--)	// backwards reading
  {
    for (ix = 0; ix < *x; ix++)
    {
      for (ic = 2; ic != -1; ic--)
      {
	fread(&val, 1, 1, bmpfile);
	image[iy][ix] += (double) val *(1.0 / (255.0 * 3.0));	// Conversion to grey by averaging the three channels
      }
    }

    fseek(bmpfile, zerobytes, SEEK_CUR);	// skipping padding bytes
  }

  fclose(bmpfile);
  return image;
}

void
bmp_out(FILE * bmpfile, double **image, int32_t y, int32_t x)
{
  int32_t i, iy, ix, ic;	// various iterators
  int32_t filesize, imagesize;
  uint8_t zerobytes, val, zero = 0;
  double vald;

  zerobytes = 4 - ((x * 3) & 3);	// computation of zero bytes
  if (zerobytes == 4)
    zerobytes = 0;

  //********Tags********

  filesize = 56 + ((x * 3) + zerobytes) * y;
  imagesize = 2 + ((x * 3) + zerobytes) * y;

  fwrite_le_short(19778, bmpfile);
  fwrite_le_word(filesize, bmpfile);
  fwrite_le_word(0, bmpfile);
  fwrite_le_word(54, bmpfile);
  fwrite_le_word(40, bmpfile);
  fwrite_le_word(x, bmpfile);
  fwrite_le_word(y, bmpfile);
  fwrite_le_short(1, bmpfile);
  fwrite_le_word(24, bmpfile);
  fwrite_le_short(0, bmpfile);
  fwrite_le_word(imagesize, bmpfile);
  fwrite_le_word(2834, bmpfile);
  fwrite_le_word(2834, bmpfile);
  fwrite_le_word(0, bmpfile);
  fwrite_le_word(0, bmpfile);
  //--------Tags--------

  for (iy = y - 1; iy != -1; iy--)	// backwards writing
  {
    for (ix = 0; ix < x; ix++)
    {
      vald = image[iy][ix] * 255.0;

      if (vald > 255.0)
	vald = 255.0;

      if (vald < 0.0)
	vald = 0.0;

      val = vald;

      for (ic = 2; ic != -1; ic--)
	fwrite(&val, 1, 1, bmpfile);
    }
    for (i = 0; i < zerobytes; i++)
      fwrite(&zero, 1, 1, bmpfile);	// write padding bytes
  }

  fwrite_le_short(0, bmpfile);

  fclose(bmpfile);
}

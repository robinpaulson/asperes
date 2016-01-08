/* 
  sound_io.h - prototypes of sound save & load functions

  The Analysis & Resynthesis Sound Spectrograph (ARSS)

  Copyright (C) 2005-2008 Michel Rouzic
  Copyright (C) 2010 Laszlo Menczel

  This is free software distributed under the terms of the GNU General
  Public License version 2.

  This program has NO WARRANTY, you use ARSS at your own risk.
*/

#ifndef H_SOUND_IO
#define H_SOUND_IO

extern void in_8(FILE * wavfile, double **sound, int32_t samplecount,
		 int32_t channels);
extern void out_8(FILE * wavfile, double **sound, int32_t samplecount,
		  int32_t channels);
extern void in_16(FILE * wavfile, double **sound, int32_t samplecount,
		  int32_t channels);
extern void out_16(FILE * wavfile, double **sound, int32_t samplecount,
		   int32_t channels);
extern void in_32(FILE * wavfile, double **sound, int32_t samplecount,
		  int32_t channels);
extern void out_32(FILE * wavfile, double **sound, int32_t samplecount,
		   int32_t channels);
extern double **wav_in(FILE * wavfile, int32_t * channels,
		       int32_t * samplecount, int32_t * samplerate);
extern void wav_out(FILE * wavfile, double **sound, int32_t channels,
		    int32_t samplecount, int32_t samplerate,
		    int32_t format_param);

#endif

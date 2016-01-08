/* 
  dsp.h - prototypes of DSP functions

  The Analysis & Resynthesis Sound Spectrograph (ARSS)

  Copyright (C) 2005-2008 Michel Rouzic
  Copyright (C) 2010 Laszlo Menczel

  This is free software distributed under the terms of the GNU General
  Public License version 2.

  This program has NO WARRANTY, you use ARSS at your own risk.
*/

#ifndef H_DSP
#define H_DSP

extern void fft(double *in, double *out, int32_t N, uint8_t method);
extern void normi(double **s, int32_t xs, int32_t ys, double ratio);
extern double *freqarray(double basefreq, int32_t bands,
			 double bandsperoctave);
extern double *blackman_downsampling(double *in, int32_t Mi, int32_t Mo);
extern double *bmsq_lut(int32_t size);
extern void blackman_square_interpolation(double *in, double *out,
					  int32_t Mi, int32_t Mo,
					  double *lut, int32_t lut_size);
extern double **anal(double *s, int32_t samplecount, int32_t samplerate,
		     int32_t * Xsize, int32_t bands, double bpo,
		     double pixpersec, double basefreq);
extern double *wsinc_max(int32_t length, double bw);
extern double *synt_sine(double **d, int32_t Xsize, int32_t bands,
			 int32_t * samplecount, int32_t samplerate,
			 double basefreq, double pixpersec, double bpo);
extern double *synt_noise(double **d, int32_t Xsize, int32_t bands,
			  int32_t * samplecount, int32_t samplerate,
			  double basefreq, double pixpersec, double bpo);
extern void brightness_control(double **image, int32_t width, int32_t height,
			       double ratio);

#endif

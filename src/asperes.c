/* 
  asperes.c - main module

  Audio Spectrograph & Re-Synthesis (ASPERES)

  Copyright (C) 2005-2008 Michel Rouzic
  Copyright (C) 2010 Laszlo Menczel

  This is free software distributed under the terms of the GNU General
  Public License version 2.

  This program has NO WARRANTY, you use ARSS at your own risk.
*/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <fftw3.h>
#include <float.h>
#include <time.h>
#include <string.h>

#include "util.h"
#include "image_io.h"
#include "sound_io.h"
#include "dsp.h"
#include "mutil.h"

//======================================================================

#define PAR_UNSET	-1
#define WAV_FORMAT	16

#define DEF_WAV_RATE	44100

#define MIN_LOW_FREQ	10
#define MAX_LOW_FREQ	500
#define DEF_LOW_FREQ	27.5

#define MIN_HIGH_FREQ	1000
#define MAX_HIGH_FREQ	20000
#define DEF_HIGH_FREQ	20000

#define MIN_BPO		2
#define MAX_BPO		64
#define DEF_BPO		12

#define MIN_PPS		10
#define MAX_PPS		300
#define DEF_PPS		150

#define MIN_GAMMA	0.1
#define MAX_GAMMA	2
#define DEF_GAMMA	1

enum { MODE_ANAL, MODE_SINE_SYNTH, MODE_NOISE_SYNTH };

/* globals */

double logbase;
int32_t quiet = 0;
char *logname = "asperes.log";

//======================================================================

/* argument list for command line processing */

static int use_linear = 0;
static int help_req = 0;
static int vers_req = 0;

static char band_per_oct_s[MUT_ARG_MAXLEN];
static char config_file[MUT_ARG_MAXLEN];
static char gamma_corr_s[MUT_ARG_MAXLEN];
static char img_height_s[MUT_ARG_MAXLEN];
static char img_width_s[MUT_ARG_MAXLEN];
static char input_file[MUT_ARG_MAXLEN];
static char high_freq_s[MUT_ARG_MAXLEN];
static char low_freq_s[MUT_ARG_MAXLEN];
static char output_file[MUT_ARG_MAXLEN];
static char pix_per_sec_s[MUT_ARG_MAXLEN];
static char prog_mode_s[MUT_ARG_MAXLEN];
static char wav_rate_s[MUT_ARG_MAXLEN];

static arglist_t arglist[] =
{
  { MUT_ARG_SWITCH, MUT_ARG_OPTIONAL, "l", (void *) &use_linear }, 
  { MUT_ARG_SWITCH, MUT_ARG_OPTIONAL, "h", (void *) &help_req }, 
  { MUT_ARG_SWITCH, MUT_ARG_OPTIONAL, "q", (void *) &quiet }, 
  { MUT_ARG_SWITCH, MUT_ARG_OPTIONAL, "v", (void *) &vers_req }, 

  { MUT_ARG_PAIRED, MUT_ARG_OPTIONAL, "a", (void *) high_freq_s }, 
  { MUT_ARG_PAIRED, MUT_ARG_OPTIONAL, "b", (void *) band_per_oct_s }, 
  { MUT_ARG_PAIRED, MUT_ARG_OPTIONAL, "c", (void *) config_file }, 
  { MUT_ARG_PAIRED, MUT_ARG_OPTIONAL, "f", (void *) input_file }, 
  { MUT_ARG_PAIRED, MUT_ARG_OPTIONAL, "g", (void *) gamma_corr_s }, 
  { MUT_ARG_PAIRED, MUT_ARG_OPTIONAL, "i", (void *) low_freq_s }, 
  { MUT_ARG_PAIRED, MUT_ARG_OPTIONAL, "m", (void *) prog_mode_s }, 
  { MUT_ARG_PAIRED, MUT_ARG_OPTIONAL, "o", (void *) output_file }, 
  { MUT_ARG_PAIRED, MUT_ARG_OPTIONAL, "p", (void *) pix_per_sec_s }, 
  { MUT_ARG_PAIRED, MUT_ARG_OPTIONAL, "r", (void *) wav_rate_s }, 
  { MUT_ARG_PAIRED, MUT_ARG_OPTIONAL, "x", (void *) img_width_s }, 
  { MUT_ARG_PAIRED, MUT_ARG_OPTIONAL, "y", (void *) img_height_s }, 

  { MUT_ARG_END }
};

//======================================================================

static char *version = "0.3.0";
static char *def_cfg = "asperes.ini";

static char *help_text[] =
{
  "Audio Spectrograph & Resynthesis"				,
  "Usage: asperes [switches] [parameters]"				,
  "  switches:"							,
  "    -h             display this help & quit"			,
  "    -v             display version number & quit"		,
  "    -q             no console output (useful for scripting)"	,
  "    -l             use linear freq scale"			,
  "  parameters:"						,
  "    -c [name]      configuration file to use"		,
  "    -m [mode]      program mode (anal, sine, noise)"		,
  "    -f [name]      name of input file"			,
  "    -o [name]      name of output file to write"		,
  "    -i [float]     minimum frequency (Hz)"			,
  "    -a [float]     maximum frequency (Hz)"			,
  "    -b [int]       frequency resolution (bands per octave)"	,
  "    -p [int]       time resolution (pixels per second)"	,
  "    -y [int]       desired height of the spectrogram"	,
  "    -x [int]       desired width of the spectrogram"		,
  "    -g [float]     gamma-like brightness correction"		,
  NULL
};

/* error messages */

static char *err_1  = "Command line parsing failed";
static char *err_2  = "You must specify the mode of operation (-m)";
static char *err_3  = "Unknown operation mode.";
static char *err_4  = "Error while reading config file ";
static char *err_5  = "Input file name missing";
static char *err_7  = "Error in the specified number ";
static char *err_8  = " is not a standard WAV sample rate.";
static char *err_9  = "Minimum frequency is out of range.";
static char *err_10 = "Maximum frequency is out of range.";
static char *err_11 = "Band per octave is out of range.";
static char *err_12 = "Pixel per second is out of range.";
static char *err_13 = "Gamma correction is out of range.";
static char *err_14 = "Cannot open the specified input file.";
static char *err_15 = "Cannot create the specified output file.";
static char *err_16 = "Bad input file name.";
static char *err_17 = "Need a WAV sound file for sound analysis.";
static char *err_18 = "Need a BMP image file for sound synthesis.";
static char *err_19 = "Cannot find the configuration file";
static char *err_20 = "Error when reading configuration file:";
static char *err_21 = "You should specify a minimum frequency.";
static char *err_22 = "You should specify the 'band per octave' parameter.";
static char *err_23 = "You should specify a maximum frequency.";
static char *err_24 = "You should specify the 'pixels per second' parameter.";

static char *warn_1 = "WARNING: Too many parameters for vertical resolution.";
static char *warn_2 = "Removing the parameter '-a' (maximum frequency).";
static char *warn_3 = "WARNING: Too many parameters for horizontal resolution";
static char *warn_4 = "Removing the parameter '-p' (pixel per second).";

/* parameters */

static double low_freq = 0.0;
static double high_freq = 0.0;
static double pix_per_sec = 0.0;
static double band_per_oct = 0.0;
static double gamma_corr = DEF_GAMMA;

static int32_t channels;
static int32_t samplecount = 0;
static int32_t prog_mode = PAR_UNSET;
static int32_t img_width = 0;
static int32_t img_height = 0;
static int32_t wav_rate = DEF_WAV_RATE;
static int32_t std_wav_rates[] =
{
  8000, 11025, 22050, 44100, 48000, 96000, -1
};

/* miscellaneous variables */

static FILE *infile;
static FILE *outfile;

static int32_t start_time;
static int32_t end_time;

static char date[32];

//======================================================================

static config_t config[] =
{
  { MUT_INI_FLT, "MinFreq",    &low_freq,     6, 0, 0 },
  { MUT_INI_FLT, "MaxFreq",    &high_freq,    6, 0, 0 },
  { MUT_INI_FLT, "BandPerOct", &band_per_oct, 4, 0, 0 },
  { MUT_INI_FLT, "PixPerSec",  &pix_per_sec,  6, 0, 0 },
  { MUT_INI_FLT, "GammaCorr",  &gamma_corr,   4, 0, 0 },
  { MUT_INI_INT, "WavRate",    &wav_rate,     6, 0, 0 },
  { MUT_INI_END, NULL, NULL, 0, 0, 0 }
};

//======================================================================

static void
print_help(void)
{
  int i;

  for (i = 0; help_text[i] != NULL; i++)
    printf("%s\n", help_text[i]);
}

//======================================================================

static void
print_version(void)
{
  printf("%s\n", version);
}

//======================================================================

static void
setup
(
  int32_t *img_height, int32_t samplecount, int32_t *wav_rate,
  double *low_freq, double *high_freq, double *pix_per_sec,
  double *bpo, int32_t img_width, int32_t mode
)
{
  double f, ma;

  int32_t unset = 0,
          set_min = 0,
          set_max = 0,
          set_bpo = 0,
          set_y = 0,
          set_pps = 0,
          set_x = 0,
          i;

  if (*low_freq != 0)
    set_min = 1;

  if (*high_freq != 0)
    set_max = 1;

  if (*bpo != 0)
    set_bpo = 1;

  if (*img_height != 0)
    set_y = 1;

  unset = set_min + set_max + set_bpo + set_y;

  if (unset == 4)
  {
    message("%s", warn_1);
    message("%s", warn_2);
    *high_freq = 0.0;
    set_max = 0;
  }

  if (*pix_per_sec != 0)
    set_pps = 1;

  if (img_width != 0)
    set_x = 1;

  if (set_x + set_pps == 2 && mode == MODE_ANAL)
  {
    message("%s", warn_3);
    message("%s", warn_4);
    *pix_per_sec = 0.0;
    set_pps = 0;
  }

  *low_freq /= *wav_rate;

  if (unset < 3 && set_min == 0)
  {
    message("%s", err_21);
    exit(EXIT_FAILURE);
  }
    
  if (unset < 3 && set_bpo == 0)
  {
    message("%s", err_22);
    exit(EXIT_FAILURE);
  }

  if (unset < 3 && set_max == 0)
  {
    if (mode == MODE_ANAL)
    {
      message("%s", err_23);
      exit(EXIT_FAILURE);
    }

    i = 0;
    do
    {
      i++;
      f = *low_freq * pow(logbase, (i / *bpo));
    } while (f < 0.5);

    ma = *low_freq * pow(logbase, ((i - 2) / *bpo)) * (*wav_rate);

    if (*high_freq > ma)
    {
      if (fmod(ma, 1.0) == 0.0)
	*high_freq = ma;
      else
	*high_freq = ma - fmod(ma, 1.0);
    }

    unset++;
    set_max = 1;
  }

  if (set_min == 0)
  {
    *low_freq = pow(logbase, (*img_height - 1) / *bpo) * (*high_freq);
    message("Minimum frequency set to: %.3f Hz", *low_freq);
    *low_freq /= *wav_rate;
  }

  if (set_max == 0)
  {
    *high_freq = pow(logbase, (*img_height - 1) / *bpo) * (*low_freq * (*wav_rate));
    message("Maximum frequency set to: %.3f Hz", *high_freq);
  }

  if (set_y == 0)
  {
    *img_height = 1 + roundoff(*bpo * (log_b(*high_freq) - log_b(*low_freq * *wav_rate)));
    message("Image height set to: %d", *img_height);
  }

  if (set_bpo == 0)
  {
    if (logbase == 1.0)
      *bpo = *high_freq / *wav_rate;
    else
      *bpo =	(*img_height - 1) / (log_b(*high_freq) - log_b(*low_freq * (*wav_rate)));

    message("Bands per octave set to: %.3f", *bpo);
  }

  if (set_x == 1 && mode == MODE_ANAL)
  {
    *pix_per_sec = (double) img_width * (double) *wav_rate / (double) samplecount;
    set_pps = 1;
    message("Pixels per second set to: %.3f", *pix_per_sec);
  }

  if (
       (mode == MODE_ANAL && set_x == 0 && set_pps == 0) ||
       (mode == MODE_SINE_SYNTH && set_pps == 0)
     )
  {
    message("%s", err_24);
    exit(EXIT_FAILURE);
  }

  *pix_per_sec /= *wav_rate;
}

//======================================================================

static int
read_config_file(char *name)
{
  char *str_p, *env, path[MUT_MAX_PATH_LEN];
  
  str_p = NULL;

  if (name[0] != 0)
  {
    if (! mut_file_exist(name))
    {
      message("%s '%s'.", err_19, name);
      return -1;
    }
    else
      str_p = name;
  }
  else
  {
    if (mut_file_exist(def_cfg))
      str_p = def_cfg;
    else
    {
      env = getenv("ASPERES_HOME");
      if (env != NULL)
      {
        strcpy(path, env);
        strcat(path, "/");
        strcat(path, def_cfg);
        if (mut_file_exist(path))
          str_p = path;
      }
    }
  }
  
  if (str_p == NULL)
    return 0;

  if (! mut_load_config(str_p, config))
  {
    message("%s", err_20);
    str_p = mut_errmsg(mut_last_error());
    message("%s", str_p);
    return -1;
  }

  return 1;
}

//======================================================================

int
main(int argc, char *argv[])
{
  double **sound, **image;
  int i;

  char path[MUT_MAX_PATH_LEN], name[MUT_MAX_NAME_LEN], ext[MUT_MAX_NAME_LEN];

  //=============================
  // parse command line arguments
  //=============================

  if (! mut_parse_cmd_line(argc, argv, arglist, NULL))
  {
    fprintf(stderr, "ASPERES %s\n", version);
    fprintf(stderr, "%s:\n%s.\n", err_1, mut_errmsg(mut_last_error()));
    return 1;
  }

  //==================================
  // check for help or version request
  //==================================
  
  if (help_req)
  {
    print_help();
    return 1;
  }

  if (vers_req)
  {
    print_version();
    return 1;
  }

  //===============
  // set up logging
  //===============

  mut_set_logname(logname);
  mut_setlog(1);
  unlink(logname);
  mut_get_date(date);
  message("ASPERES version %s (%s)", version, date);

  //=================
  // read config file
  //=================

  if (read_config_file(config_file) < 0)
  {
    message("%s %s", err_4, config_file);
    return 1;
  }

  //===============================
  // process command line arguments
  //===============================

  //======= program operation mode =======

  if (prog_mode_s[0] == 0)
  {
    message("%s", err_2);
    return 1;
  }

  if (strcmp(prog_mode_s, "anal") == 0)
    prog_mode = MODE_ANAL;
  else if (strcmp(prog_mode_s, "sine") == 0)
    prog_mode = MODE_SINE_SYNTH;
  else if (strcmp(prog_mode_s, "noise") == 0)
    prog_mode = MODE_NOISE_SYNTH;
  else
  {
    message("%s (%s)", err_3, prog_mode_s);
    return 1;
  }

  //======= file names =======

  if (input_file[0] == 0)
  {
    message("%s", err_5);
    return 1;
  }
      
  if (! mut_fname_split(input_file, path, name, ext))
  {
     message("%s", err_16);
     return 1;
  }

  if (prog_mode == MODE_ANAL)
  {
     if (strcmp(ext, ".wav") != 0)
     {
       message("%s", err_17);
       return 1;
     }
  }
  else
  {
     if (strcmp(ext, ".bmp") != 0)
     {
       message("%s", err_18);
       return 1;
     }
  }
  
  if (output_file[0] == 0) 
  {
    strcpy(output_file, path);
    strcat(output_file, name);
    strcat(output_file, "~");
    
    if (prog_mode == MODE_ANAL)
      strcat(output_file, ".bmp");
    else
      strcat(output_file, ".wav");
  }
 
  //======= WAV sample rate =======

  if (wav_rate_s[0] != 0)
  {
    if (! mut_stoi(wav_rate_s, MUT_BASE_DEC, &wav_rate))
    {
      message("%s '%s'", err_7, wav_rate_s);
      return 1;
    }
  }

  i = 0;
  while (std_wav_rates[i] != -1)
  {
    if (std_wav_rates[i] == wav_rate)
      break;
    else
      i++;
  }
    
  if (std_wav_rates[i] == -1)
  {
    message("'%s' %s", wav_rate_s, err_8);
    return 1;
  }

  //======= frequency limits =======

  if (low_freq_s[0] != 0)
  {
    if (! mut_stof(low_freq_s, &low_freq))
    {
      message("%s '%s'", err_7, low_freq_s);
      return 1;
    }
  }
      
  if (low_freq < MIN_LOW_FREQ || low_freq > MAX_LOW_FREQ)
  {
    message("%s", err_9);
    return 1;
  }

  if (high_freq_s[0] != 0)
  {
    if (! mut_stof(high_freq_s, &high_freq))
    {
      message("%s '%s'", err_7, high_freq_s);
      return 1;
    }
  }

  if (high_freq < MIN_HIGH_FREQ || high_freq > MAX_HIGH_FREQ)
  {
    message("%s", err_10);
    return 1;
  }
      
  //======= resolutions =======

  if (band_per_oct_s[0] != 0)
  {
    if (! mut_stof(band_per_oct_s, &band_per_oct))
    {
      message("%s '%s'", err_7, band_per_oct_s);
      return 1;
    }
  }
      
  if (band_per_oct < MIN_BPO || band_per_oct > MAX_BPO)
  {
    message("%s", err_11);
    return 1;
  }

  if (pix_per_sec_s[0] != 0)
  {
    if (! mut_stof(pix_per_sec_s, &pix_per_sec))
    {
      message("%s '%s'", err_7, pix_per_sec_s);
      return 1;
    }
  }

  if (pix_per_sec < MIN_PPS || pix_per_sec > MAX_PPS)
  {
    message("%s", err_12);
    return 1;
  }
      
  //======= brightness control =======

  if (gamma_corr_s[0] != 0)
  {
    if (! mut_stof(gamma_corr_s, &gamma_corr))
    {
      message("%s '%s'", err_7, gamma_corr_s);
      return 1;
    }
  }

  if (gamma_corr < MIN_GAMMA || gamma_corr > MAX_GAMMA)
  {
    message("%s", err_13);
    return 1;
  }

  //======= image dimensions =======

  if (img_width_s[0] != 0)
  {
    if (! mut_stoi(img_width_s, MUT_BASE_DEC, &img_width))
    {
      message("%s '%s'", err_7, img_width_s);
      return 1;
    }
  }

  if (img_height_s[0] != 0)
  {
    if (! mut_stoi(img_height_s, MUT_BASE_DEC, &img_height))
    {
      message("%s '%s'", err_7, img_height_s);
      return 1;
    }
  }

  //======= miscellaneous parameters =======

  if (use_linear)
    logbase = 1;
  else
    logbase = 2;

  //====================
  // open & create files
  //====================

  infile = fopen(input_file, "rb");
  if (infile == NULL)
  {
    message("%s", err_14);
    return 1;
  }

  outfile = fopen(output_file, "wb");
  if (outfile == NULL)
  {
    fclose(infile);
    message("%s", err_15);
    return 1;
  }

  //===================================
  // perform the requested operation(s)
  //===================================

  srand(time(NULL));

  if (prog_mode == MODE_ANAL)
  {
    message("Sound '%s' to spectrogram '%s'", input_file, output_file);
    sound = wav_in(infile, &channels, &samplecount, &wav_rate);
    setup
    (
      &img_height, samplecount, &wav_rate, &low_freq, &high_freq,
      &pix_per_sec, &band_per_oct, img_width, 0
    );

    start_time = gettime();
    image = anal
            (
              sound[0], samplecount, wav_rate,
              &img_width, img_height, band_per_oct, pix_per_sec, low_freq
            );

    if (gamma_corr != 1.0)
      brightness_control(image, img_height, img_width, 1.0 / gamma_corr);

    bmp_out(outfile, image, img_height, img_width);
  }

  if (prog_mode == MODE_SINE_SYNTH || prog_mode == MODE_NOISE_SYNTH)
  {
    message("Image '%s' to sound '%s'", input_file, output_file);
    sound = calloc(1, sizeof(double *));
    image = bmp_in(infile, &img_height, &img_width);

    setup
    (
      &img_height, samplecount, &wav_rate, &low_freq, &high_freq,
      &pix_per_sec, &band_per_oct, img_width, 1
    );

    if (gamma_corr != 1.0)
      brightness_control(image, img_height, img_width, gamma_corr);

    start_time = gettime();

    if (prog_mode == MODE_SINE_SYNTH)
      sound[0] = synt_sine
                 (
                   image, img_width, img_height, &samplecount, wav_rate,
                   low_freq, pix_per_sec, band_per_oct
                 );
    else
      sound[0] = synt_noise
                 (
                   image, img_width, img_height, &samplecount, wav_rate,
                   low_freq, pix_per_sec, band_per_oct
                 );

    wav_out(outfile, sound, 1, samplecount, wav_rate, WAV_FORMAT);
  }

  end_time = gettime();
  message("Processing time: %.3f s", (double) (end_time - start_time) / 1000.0);

  return 0;
}

/*
   mutil.h -- header for the MUTIL library

   Copyright (C) Laszlo Menczel, 2005-2007
   menczel@invitel.hu

   This is free software without warranty. See 'licence.txt'.
*/

#if ! defined _MUTIL_H
#define _MUTIL_H

#include <stdio.h>   /* for FILE type */

#if defined _cplusplus
extern "C" {
#endif

//============================= constants ==================================

//======== key codes returned by mut_getkey()

#define DEL		0xff53
#define DOWN		0xff50
#define END		0xff4f
#define HOME		0xff47
#define INS		0xff52
#define LEFT		0xff4b
#define PGDN		0xff51
#define PGUP		0xff49
#define RIGHT		0xff4d
#define UP		0xff48

#define CTRL_DEL	0xff93
#define CTRL_DOWN	0xff91
#define CTRL_END	0xff75
#define CTRL_HOME	0xff77
#define CTRL_INS	0xff92
#define CTRL_LEFT	0xff73
#define CTRL_PGDN	0xff76
#define CTRL_PGUP	0xff86
#define CTRL_RIGHT	0xff74
#define CTRL_UP		0xff8d

#define ALT_DEL		0xffa3
#define ALT_DOWN	0xffa0
#define ALT_END		0xff9f
#define ALT_HOME	0xff97
#define ALT_INS		0xffa2
#define ALT_LEFT	0xff9b
#define ALT_PGDN	0xffa1
#define ALT_PGUP	0xff99
#define ALT_RIGHT	0xff9d
#define ALT_UP		0xff98

#define F1		0xff3b
#define F2		0xff3c
#define F3		0xff3d
#define F4		0xff3e
#define F5		0xff3f
#define F6		0xff40
#define F7		0xff41
#define F8		0xff42
#define F9		0xff43
#define F10		0xff44
#define F11		0xff85
#define F12		0xff86

#define SHIFT_F1	0xff54
#define SHIFT_F2	0xff55
#define SHIFT_F3	0xff56
#define SHIFT_F4	0xff57
#define SHIFT_F5	0xff58
#define SHIFT_F6	0xff59
#define SHIFT_F7	0xff5a
#define SHIFT_F8	0xff5b
#define SHIFT_F9	0xff5c
#define SHIFT_F10	0xff5d
#define SHIFT_F11	0xff87
#define SHIFT_F12	0xff88

#define CTRL_F1		0xff5e
#define CTRL_F2		0xff5f
#define CTRL_F3		0xff60
#define CTRL_F4		0xff61
#define CTRL_F5		0xff62
#define CTRL_F6		0xff63
#define CTRL_F7		0xff64
#define CTRL_F8		0xff65
#define CTRL_F9		0xff66
#define CTRL_F10	0xff67
#define CTRL_F11	0xff89
#define CTRL_F12	0xff8a

#define ALT_F1		0xff68
#define ALT_F2		0xff69
#define ALT_F3		0xff6a
#define ALT_F4		0xff6b
#define ALT_F5		0xff6c
#define ALT_F6		0xff6d
#define ALT_F7		0xff6e
#define ALT_F8		0xff6f
#define ALT_F9		0xff70
#define ALT_F10		0xff71
#define ALT_F11		0xff8b
#define ALT_F12		0xff8c

#define BACKSP		8
#define TAB		9
#define NEWLINE		10
#define CR		13
#define ESC		27

//======== error codes

#if defined NO_ERROR
  #undef NO_ERROR
#endif

#define NO_ERROR		0

// general error types (1 to 49)

#define MUTERR_BAD_ARG		1
#define MUTERR_ALLOC		2
#define MUTERR_FILE		3
#define MUTERR_FILE_TYPE	4
#define MUTERR_FILE_READ	5
#define MUTERR_FILE_WRITE	6
#define MUTERR_TOO_MANY		7
#define MUTERR_TOO_FEW		8
#define MUTERR_BAD_TOKEN	9
#define MUTERR_RANGE		10
#define MUTERR_EMPTY		11
#define MUTERR_TYPE		12
#define MUTERR_SIZE		13
#define MUTERR_END_OF_STR	14
#define MUTERR_TIMEOUT		15
#define MUTERR_DLL_LOAD		16
#define MUTERR_NOT_FOUND	17
#define MUTERR_NOT_READY	18
#define MUTERR_DIR		19
#define MUTERR_SYSINFO		20
#define MUTERR_OVERFLOW		21
#define MUTERR_PARTIAL_READ	22
#define MUTERR_END_OF_FILE	23
#define MUTERR_WIN32PROC	24
#define MUTERR_WIN32WAIT	25
#define MUTERR_WIN32EXITCODE	26
#define MUTERR_ACCESS_FILE	27
#define MUTERR_ACCESS_DIR	28
#define MUTERR_SYSCALL		29
#define MUTERR_BUSY		30
#define MUTERR_READ_ONLY	31

// errors in string-number conversion (50 to 69)

#define MUTERR_BAD_INT		50
#define MUTERR_BAD_FLOAT	51
#define MUTERR_INT_BASE		53
#define MUTERR_TOO_MANY_DIG	53
#define MUTERR_NO_DIG		54
#define MUTERR_NOT_DIG		55
#define MUTERR_EXTRA_CHAR	56
#define MUTERR_EXP_RANGE	57
#define MUTERR_NO_EXP		58
#define MUTERR_BAD_EXP		59
#define MUTERR_TOO_MANY_DOTS	60

// errors in config file loading (70 to 89)

#define MUTERR_PAR_FILE		70
#define MUTERR_PAR_NAME		71
#define MUTERR_PAR_TYPE		72
#define MUTERR_PAR_VAR		73
#define MUTERR_PAR_SIZE		74
#define MUTERR_PAR_INDEX	75
#define MUTERR_PAR_VALUE	76
#define MUTERR_PAR_CONV		77
#define MUTERR_PAR_LOAD		78
 
// errors in command line parsing (90 to 109)

#define MUTERR_ARG_PROGNAME	90
#define MUTERR_ARG_SIZE		91
#define MUTERR_ARG_NAME		92
#define MUTERR_ARG_PARAM	93
#define MUTERR_ARG_PARSIZE	94
#define MUTERR_ARG_BAD_PARAM	95
#define MUTERR_ARG_DUPLIC	96
#define MUTERR_ARG_MISSING	97
  
#define MUTERR_UNDEF            1000

#define MUT_LAST_ERROR		109  // update if more codes are added!

//======== limits

#define MUT_ARG_MAXLEN		256
#define MUT_MAX_STR_LEN		256
#define MUT_MAX_LINE_LEN	512
#define MUT_MAX_PATH_LEN	256
#define MUT_MAX_SUB_STRING	256
#define MUT_MAX_NUM_LEN		32
#define MUT_MAX_BIN_LEN		32
#define MUT_MAX_DEC_LEN		12    
#define MUT_MAX_HEX_LEN		8    
#define MUT_MAX_FLT_LEN		24
#define MUT_MAX_NAME_LEN	256

//======== miscellaneous

enum { MUT_BASE_BIN, MUT_BASE_DEC, MUT_BASE_HEX };
enum { MUT_CANCEL, MUT_ACCEPT };
enum { MUT_FIND_FIRST, MUT_FIND_LAST };

//======== command line parsing and config loading

enum
{
  MUT_INI_INT,
  MUT_INI_FLT,
  MUT_INI_STR,
  MUT_INI_AINT,
  MUT_INI_AFLT,
  MUT_INI_ASTR,
  MUT_INI_END
};

enum { MUT_ARG_SWITCH, MUT_ARG_PAIRED, MUT_ARG_END };
enum { MUT_ARG_OPTIONAL, MUT_ARG_REQUIRED };

//======== filename globbing

#define MUT_FLIST_HAS_DIR		1
#define MUT_FLIST_DENIED		2
#define MUT_FLIST_EMPTY			4
#define MUT_FLIST_NO_MATCH		8

enum { MUT_FLIST_SIMPLE, MUT_FLIST_RECURS };

enum { MUT_FLIST_ALL, MUT_FLIST_FILES, MUT_FLIST_DIRS };

#define MUT_FLIST_PATH			8

#define MUT_FINFO_IS_DIR		1
#define MUT_FINFO_HIDDEN		2

//========================== structures ==================================

/*
   Initialization parameter structure, used as argument for the function
   'mut_load_config'.
*/

typedef struct config_info
{
  int type;
  char *name;     /* name to search for in the INI file */
  void *var;      /* pointer to the variable to initialize */
  int len;        /* maximum length if string variable */
  int error;      /* set to non-zero if an error occurs */
  int loaded;
} config_t;

/*
   Command line argument structure, used as argument for the
   'mut_parse_cmd_line' function.
*/

typedef struct arg_list
{
  int  type;       /* switch, or identifier paired with a literal string */
  int  req;        /* required if non-zero (ignored in case of switches) */
  char *templ;     /* template string to search for */
  void *dst;       /* pointer to integer flag or string buffer */
  int  found;      /* used internally, by arg_parse() */
} arglist_t;

/* file list structures */

typedef struct
{
  int  flag;
  int  len;
  char name[MUT_MAX_STR_LEN];
} finfo_t;

typedef struct flist_s flist_t;

struct flist_s
{
  int     flag;
  int     count;
  int     filenum;
  int     dirnum;
  int     dirlen;
  char    dir[MUT_MAX_PATH_LEN];
  finfo_t *files;
  flist_t *next;
};

//======================== DOS specific functions ===========================

#if defined _DJGPP
  void Sleep(int count);
#endif

//======================== Win32 specific functions =========================

#if defined _WIN32
  int mut_exec_prog(char *prog, int wait, int *retval, char *outfile);
#endif

//======================== Linux specific functions =========================

#if defined _LINUX
  void mut_kbd_init(void);
#endif

//====================== platform independent functions =====================

//======== keyboard input

int     mut_keypress(void);
int     mut_get_str(char *buf, int max);
int     mut_getkey(void);
void    mut_kbdclear(void);

//======== string tokenization

char    *mut_get_token(char *dest, char *pos, char *delim, int maxlen);
char    *mut_get_token_ex(char *dest, char *buf, char *end, char *delim, int maxlen);
char    *mut_parse_int(char *buf, char *delim, int base, int *dest);
char    *mut_parse_float(char *buf, char *delim, double *dest);
char    *mut_parse_int_ex(char *buf, char *end, char *delim, int base, int *dest);
char    *mut_parse_float_ex(char *buf, char *end, char *delim, double *dest);
char    *mut_skip_delimiter(char *s, char *templ, int len);
char    *mut_skip_space(char *s, int len);

//======== string utilities

char    **mut_str_split(char *s, char *delim, int *res);
char    **mut_str_substr_ptr(char *buf, char delim, char end);
char    *mut_str_end(char *s);
char    *mut_strn_end(char *s, int max);
char    *mut_str_find(char *s, char c, int len, int dir);
char    *mut_str_first(char *s, char c, int len);
char    *mut_str_last(char *s, char c, int len);
int     mut_str_match(char *s, char *buf);
int     mut_strip_eol(char *s);
int     mut_strip_quote(char *s, int len);
int     mut_stripn_eol(char *s, int max);

//======== file utilities

char  **mut_drive_name_array(int *num);
int     mut_fget_line(char *buf, int len, FILE *f);
int     mut_fget_line_ex(char *buf, char *ignore, int len, int *count, FILE *f);
int     mut_file_delete(char *name);
int     mut_file_exist(char *name);
int     mut_file_line_count(char *name, char *ignore);
int     mut_file_rename(char *old, char *new);
int     mut_fname_change_ext(char *name, char *ext);
int     mut_fname_char(char c);
int     mut_fname_check_len(char *s);
int     mut_fname_check_str(char *s);
int     mut_fname_fix_delim(char *path);
int     mut_fname_get_extension(char *s, char *ext);
int     mut_fname_get_name(char *s, char *name);
int     mut_fname_get_path(char *s, char *path);
int     mut_fname_append(char *name, char *app, int maxlen);
int     mut_fname_split(char *s, char *path, char *name, char *ext);
int     mut_strip_ext(char *s);

#if defined (_WIN32) || defined (_LINUX)

//======== filename globbing

flist_t *mut_glob_dir(char *dir, char *pattern, int mode);
flist_t *mut_new_glob(void);
char    *mut_glob_format(flist_t *list, char delim, int mode);
int     mut_glob_append(flist_t *f, flist_t *list);
int     mut_glob_count(flist_t *list);
int     mut_glob_discard(flist_t **list);
int     mut_glob_fill(char *dir, char *pattern, flist_t *f);
int     mut_glob_insert(flist_t *f, flist_t *list, flist_t *pos);
int     mut_finfo_is_dir(finfo_t *f);
int     mut_finfo_hidden(finfo_t *f);
void    mut_show_flist(flist_t *f);

#endif

//======== array utilities

char    **mut_alloc_str_array(int count, int size);
int     mut_create_str_ptrs(char **array, int count, int size);
int     mut_delete_str_ptrs(char **array, int count);
int     mut_free_str_array(char **array, int count);
int     mut_iarray_sum(int *arr, int last_index);
double  mut_darray_sum(double *arr, int last_index);

//======== command line & ini file functions

int     mut_load_config(char *name, config_t *par);
int     mut_parse_cmd_line(int argc, char *argv[], arglist_t *list, char *prog);

//======== numeric conversion

int     mut_stof(char *s, double *dest);
int     mut_stoi(char *s, int base, int *dest);
int     mut_itosb(void *data, int len, char *buf);

#if defined (_WIN32) || defined (_LINUX)

//======== date and time funtions

int     mut_get_date(char *buf);
int     mut_get_date_short(char *buf);
int     mut_get_time(char *buf);
int     mut_make_time_stamp(char *buf);

unsigned int mut_systime(void);

#endif

//======== debugging and logging

void    mut_brkpt(int num);
void    mut_brkpts(char *msg);
void    mut_logprintf(char *format, ...);
void    mut_logputs(char *msg);
void    mut_logtrace(char *msg);
void    mut_logtrace2(char *msg);
void    mut_set_loglimit(int count);
void    mut_set_logname(char *name);
void    mut_setlog(int on_off);
void    mut_settrace(int level);

//======== error handling

char    *mut_errmsg(int code);
int     mut_last_error(void);

# if defined _cplusplus
}
#endif

#endif  /* _MUTIL_H */

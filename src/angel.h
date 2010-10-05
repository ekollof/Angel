/*
 * Copyright (c) 2003, Emiel Kollof <coolvibe@hackerheaven.org> All rights
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met: Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution. Neither the name of
 * the <ORGANIZATION> nor the names of its contributors may be used to
 * endorse or promote products derived from this software without specific
 * prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * Created by Emiel Kollof on Tue Nov 25 2003. Copyright (c) 2003 All rights
 * reserved.
 *
 * $Id: angel.h,v 1.2 2003/12/09 09:49:10 coolvibe Exp $
 */

#ifndef ANGEL_H
#define ANGEL_H

/* includes */

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <dirent.h>
#include <ctype.h>
#include <pwd.h>
#include <err.h>
#include <errno.h>
#include <time.h>
#include <fcntl.h>
#include <sysexits.h>

/* definitions */
#define TRUE 1
#define FALSE 0

/* commands */
#define CMD_OK 0
#define CMD_STOP 1
#define CMD_KILL 2
#define CMD_RENICE 4

/* events */
#define EV_AOK 0
#define EV_LOADTOOHIGH 1
#define EV_RAMPANT 2

#ifdef Linux
	#define NO_NATIVE_STRLCPY
	#define NO_ERRH
#endif

#ifdef FreeBSD
#endif

#ifdef SunOS
	#define NO_ASPRINTF
	#define NO_DAEMON
	#define NO_STRDUP
#endif


/* globals */
#ifdef __linux__ /* blargh */
extern const char *__progname;
#endif

extern char     sysconfigdir[MAXPATHLEN];
extern char     usrconfigdir[MAXPATHLEN];
extern char    *cbuf;           /* Buffer that contains configfile */
extern int      running;        /* Flag that says if process is running */
extern int      cmdval;         /* Value contained in cmdfile */
extern int      logfd;     /* log filedescriptor for stdout */
extern int      pid;     /* pid of forked child */

/* global configuration vars */
extern int      debug;
extern int      verbose;        /* flag for verbosity */
extern int      nodetach;       /* flag for daemon */
extern int      delay;          /* how much seconds to wait */
extern double   maxload;        /* float value for max load */
extern char     cmd[256];       /* string for program name */
extern char     args[1025];     /* string for program args */
extern char     workdir[MAXPATHLEN];    /* working directory */
extern char     logfile[MAXPATHLEN];    /* logfile */
extern int      logsize;        /* maximum log size in KB */

/* prototypes (grouped by source file) */

/* config.c */
char           *config_read(char *config);
void            config_feed(char *config);
void            config_parse(char *line);

/* utils.c */

char	       *mmap_read(char *path);
char           *suckfile(FILE * fp);
void            stripchar(char *buf, int strip);
void            cleading(char *buf);
void            ctrailing(char *buf);
void            trim(char *buf);
void            chomp(char *buf);
void            dbprintf(char *fmt,...); /* debug printf */
void            vbprintf(char *fmt,...);        /* verbose printf */
void            renice(pid_t pid, int value);

#ifdef __linux__ /* blegh */
const char      *getprogname(void);
#endif

/* command.c */

int             command_check(void);
int             command_read(char *commandfile);
void            command_handle(int command, pid_t pid);

/* event.c */
int             event_check(void);

/* logging.c */
void                             logging_init();
void                             logging_rotate();
#endif

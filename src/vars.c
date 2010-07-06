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
 * $Id: vars.c,v 1.2 2003/12/09 09:49:10 coolvibe Exp $
 */

#ifndef lint
static          const char copyright[] = "Copyright (c) 2003 Emiel Kollof <coolvibe@hackerheaven.org>";
#endif

#ifndef lint
static          const char rcsid[] = "$Id: vars.c,v 1.2 2003/12/09 09:49:10 coolvibe Exp $";
#endif

#include "angel.h"

/*
 * This file holds no real code. It's sole purpose is providing some defaults
 * and some variables for the extern declarations in the header file to lean
 * on.
 */

char            sysconfigdir[MAXPATHLEN] = SYSCONFIGDIR;
char            usrconfigdir[MAXPATHLEN] = USRCONFIGDIR;
char           *cbuf;
int             running = FALSE;
int             cmdval = 0;
int                              logfd=-1;
int                              pid=-1;

/* configuration defaults */
int             debug = 0;
int             verbose = 0;
int             nodetach = 0;
int             delay = 1;
int                              logsize = 512;
double          maxload = -1;
char            cmd[256];
char            args[1025];
char            workdir[MAXPATHLEN] = "./";
char                             logfile[MAXPATHLEN] = "out.log";

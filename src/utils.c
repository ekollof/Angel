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
 * $Id: utils.c,v 1.3 2003/12/11 21:46:51 coolvibe Exp $
 */

#ifndef lint
static          const char copyright[] = "Copyright (c) 2003 Emiel Kollof <coolvibe@hackerheaven.org>";
#endif

#ifndef lint
static          const char rcsid[] = "$Id: utils.c,v 1.3 2003/12/11 21:46:51 coolvibe Exp $";
#endif

#include "angel.h"

char * 
mmap_read(char *path)
{
	int fd = 0, r;
	char *buf, *ret;
	struct stat st;

	fd = open (path, O_RDONLY);
	if (fd < 0) {
		warn("open: %s", path);
		return NULL;
	}

	if (fstat(fd, &st) == -1) {
		warn("fstat: %s", path);
		return NULL;
	}

	buf = mmap(0, st.st_size, PROT_READ, MAP_SHARED, fd, 0);
	if (buf == MAP_FAILED) { 
		err(EX_SOFTWARE, "mmap"); /* exits */
	}

	r = close(fd);
	if (r == -1) {
		err(EX_OSFILE, "close"); /* also exits */
	}

	ret = strdup(buf);

	/* clean up mmap after we're done */
	r = munmap(buf, st.st_size);
	if (r == -1) {
		err(EX_SOFTWARE, "munmap"); /* also exits */
	}
	
	return ret;
}

char           *
suckfile(FILE * fp)
{
        /*
         * gets file from fp, and automatically allocates storage for it.
         * Make sure you free when you ever use this in a library or a
         * daemon.
         */

        char            buf[1024], *r;
        unsigned int    size = 0;

        if (fp == NULL) {
                errx(EX_OSFILE, "fp was NULL? Bailing!");
        }
        r = (char *) malloc(sizeof(buf));
        if (r == NULL) {
                err(EX_SOFTWARE, "malloc");
        }
        while (fgets(buf, sizeof(buf) - 1, fp)) {
                size += strlen(buf);
                strncat(r, buf, sizeof(buf) - size);
        }
        if (!feof(fp)) {
                err(EX_OSFILE, "suckfile: ");
                /* NOTREACHED */
        }
        return r;
}

void
stripchar(char *buf, int strip)
{
        /*
         * Strips character strip from buf. Works in situ.
         */

        char           *ptr = buf;
        int             len;
        char            c;
        len = strlen(buf) + 1;  /* adjust for \0 */
        while (--len > 0) {
                c = *ptr++;
                if (c != strip)
                        *buf++ = c;
        }
        *buf++ = '\0';
        return;
}

void
cleading(char *buf)
{
        /*
         * Nippy routine that strips leading whitespace from a string. Alters
         * buf inplace. Be sure to strdup if you don't want to destroy the
         * original.
         */

        char           *ptr = buf;
        size_t          len;
        char            c;
        int             done = 0;

        len = strlen(buf) + 1;  /* adjust for trailing 0 */
        while (--len > 0) {
                c = *ptr++;

                switch (c) {
                case ' ':
                        if (done)
                                *buf++ = c;
                        break;
                case '\t':
                        if (done)
                                *buf++ = c;
                        break;
                default:
                        *buf++ = c;
                        done = 1;
                        break;
                }
        }
        *buf++ = 0;
        return;
}

void
ctrailing(char *buf)
{
        /*
         * Routine that strips trailing whitespace from buf. Works back to
         * front.
         */

        int             len, done = 0;

        len = strlen(buf);
        while (len-- > 0) {
                switch (buf[len]) {
                case ' ':
                        if (!done)
                                buf[len] = 0;
                        break;
                case '\t':
                        if (!done)
                                buf[len] = 0;
                        break;
                default:
                        done = 1;
                        break;
                }
        }
}

void
trim(char *buf)
{
        /*
         * Uses cleading and ctrailing to strip any leading and trailing
         * whitespace from buf.
         */
        cleading(buf);
        ctrailing(buf);
        return;
}

void
chomp(char *buf)
{
        /*
         * Uses stripchar to remove any newlines from buf.
         */
        stripchar(buf, '\n');
        stripchar(buf, '\r');
        return;
}

void
dbprintf(char *fmt,...)
{
        /*
         * A "printf" for debugging. Only prints when debugging is turned on.
         */

        va_list         ap;
        char            buf[1500];

        if (debug) {
                va_start(ap, fmt);
                vsnprintf(buf, sizeof(buf), fmt, ap);
                va_end(ap);

                printf("%s", buf);
        }
        return;
}

void
vbprintf(char *fmt,...)
{
        /*
         * A "printf" for verbosity. Only prints when verbosity is turned on.
         */

        va_list         ap;
        char            buf[1500];

        if (verbose) {
                va_start(ap, fmt);
                vsnprintf(buf, sizeof(buf), fmt, ap);
                va_end(ap);

                printf("%s", buf);
        }
        return;
}


void
getisodate(char *date)
{
        /*
         * Generates an ISO date from the current time and plonks it in date.
         * No bounds checking, take care. Note that date must at least be 9
         * bytes big (that's including \0)
         */
        time_t          now;
        struct tm      *tmdate;

        time(&now);

        tmdate = localtime(&now);

        sprintf(date, "%.4d%.2d%.2d", tmdate->tm_year + 1900, tmdate->tm_mon, tmdate->tm_mday);

        return;
}

void
renice(pid_t pid, int value)
{
        /*
         * Renices a process ID. Prints a warning when unsuccessful.
         */

        if (setpriority(PRIO_PROCESS, pid, value) < 0)
                warnx("Setting prio of pid %d to %d failed!\n", pid, value);
        else
                vbprintf("Priority of pid %d is now %d\n", pid, value);

        return;
}

#ifdef __linux__
const char *getprogname(void)
{
        return(__progname);
}
#endif

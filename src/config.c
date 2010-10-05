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
 * $Id: config.c,v 1.3 2003/12/09 09:57:02 coolvibe Exp $
 */

#ifndef lint
static          const char copyright[] = "Copyright (c) 2003 Emiel Kollof <coolvibe@hackerheaven.org>";
#endif

#ifndef lint
static          const char rcsid[] = "$Id: config.c,v 1.3 2003/12/09 09:57:02 coolvibe Exp $";
#endif

#include "angel.h"

/*
 * Only for use in config_parse. Will give compile errors if used _ANYWHERE_
 * else.
 */

#define COMPARE_INT(cvar, fvar)                         \
        if (!strcasecmp(var, fvar)) {                   \
                match++;                                \
                cvar = atoi(arg);                       \
        }

#define COMPARE_DOUBLE(cvar, fvar)                      \
        if (!strcasecmp(var, fvar)) {                   \
                match++;                                \
                cvar = (double) atof(arg);              \
        }

#define COMPARE_STRING(cvar, fvar)                      \
        if (!strcasecmp(var, fvar)) {                   \
                match++;                                \
                strncpy(cvar, arg, sizeof(cvar) - 1);   \
                cvar[sizeof(cvar) - 1] = 0;             \
        }

char           *
config_read(char *config)
{
        /*
         * This function checks if a configfile exists in the system
         * and user directories. If a configuration is found, memory
         * is allocated and the configuration file is sucked in.
         * Returns a pointer to the configfile in memory.
         *
         * Don't forget to free() after you're done with this, so we won't
         * get any memory leaks.
         */

        char            dbuf[MAXPATHLEN], *buf;
        struct passwd  *pw;
        FILE           *fp;

        /* Try directory in $HOME first, then try system config. */
        pw = getpwuid(getuid());
        strncpy(dbuf, pw->pw_dir, MAXPATHLEN);
        strncat(dbuf, "/", MAXPATHLEN - strlen(dbuf));
        strncat(dbuf, USRCONFIGDIR, MAXPATHLEN - strlen(dbuf));
        strncat(dbuf, "/", MAXPATHLEN - strlen(dbuf));

        /* Try USRCONFIGDIR first */
        strncat(dbuf, config, MAXPATHLEN - strlen(dbuf));

	buf = mmap_read(dbuf);
        if (buf == NULL) {
                dbprintf("Couldn't open %s, trying %s/%s\n", dbuf, SYSCONFIGDIR, config);
                strncpy(dbuf, SYSCONFIGDIR, MAXPATHLEN - 1);
                dbuf[MAXPATHLEN - 1] = 0;

                /* Try SYSCONFIGDIR next */
                strncat(dbuf, config, MAXPATHLEN - strlen(dbuf));

		buf = mmap_read(dbuf);
                if (buf == NULL) {
                        errx(1, "PANIC: No configuration file!");
                }
        }

        dbprintf("Using configuration for `%s' in %s\n", config, dbuf);
        return buf;
}

void
config_feed(char *config)
{
        /*
         * This function feeds the configfile line by line to the configfile
         * parser.
         */

        char            newline[] = "\n";       /* delimiter for strtok() */
        unsigned int    line;
        char           *buf, *ptr;

        buf = (char *)strdup(config);

        line = 0;
        for (;;) {

                /* First strtok needs buf, consecutive ones need NULL */
                if (line == 0) {
                        ptr = (char *)strtok(buf, newline);
                } else {
                        ptr = (char *)strtok(NULL, newline);
                }
                if (ptr == NULL) {      /* End of buffer */
                        break;
                }
                config_parse(ptr);

                line++;
        }
        dbprintf("Parsed %d lines.\n", line);

}

void
config_parse(char *line)
{
        /*
         * Configfile 'parser'. Checks the fed line for comments, empty
         * lines and lines that have an '=' character. It splits up
         * the line into a part before and after the equal sign, and then
         * assigns the values to global variables with a macro.
         */

        char           *var, *arg;
        int             match = 0;

        if (*line == '#')       /* Comment */
                return;
        if (*line == '\n')      /* Empty line */
                return;

        var = line;
        arg = (char *)strchr(var, '=');


        if (arg) {
                *arg++ = 0;     /* Nuke the '=' */
                trim(arg);      /* Nuke leading and trailing whitespace */
        } else {
                return;
        }

        COMPARE_INT(nodetach, "nodetach");
        COMPARE_INT(verbose, "verbose");
        COMPARE_INT(debug, "debug");
        COMPARE_INT(delay, "delay");
        COMPARE_INT(logsize, "logsize");
        COMPARE_DOUBLE(maxload, "maxload");
        COMPARE_STRING(cmd, "exec");
        COMPARE_STRING(args, "argv");
        COMPARE_STRING(workdir, "workdir");
        COMPARE_STRING(logfile, "logfile");

        if (!match)
                return;

        dbprintf("Set: %s = '%s'\n", var, arg);

        return;
}

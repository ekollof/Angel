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
 * $Id: command.c,v 1.2 2003/12/09 09:49:10 coolvibe Exp $
 */

#ifndef lint
static          const char copyright[] = "Copyright (c) 2003 Emiel Kollof <coolvibe@hackerheaven.org>";
#endif

#ifndef lint
static          const char rcsid[] = "$Id: command.c,v 1.2 2003/12/09 09:49:10 coolvibe Exp $";
#endif

#include "angel.h"

char           *cmds[] = {
        HALTFILE,
        KILLFILE,
        NICEFILE,
        NULL
};

int
command_check(void)
{
        /*
         * This function checks the working directory for any command files.
         * Returns which commandfile is found. See angel.h.
         */

        DIR            *cdir;
        struct dirent  *dent;
        int             retval = 0, i;

        cdir = opendir(workdir);
        if (!cdir) {
                perror("opendir");
                exit(-1);
        }
        while ((dent = readdir(cdir)) != NULL) {
                if (!strcmp(dent->d_name, ".")) /* ignore . dirs */
                        continue;
                if (!strcmp(dent->d_name, ".."))        /* .. dirs too */
                        continue;

                for (i = 0; cmds[i] != NULL; i++) {
                        if (!strcmp(dent->d_name, cmds[i])) {
                                vbprintf("Found a commandfile: %s\n", cmds[i]);
                                retval = command_read(cmds[i]);
                        }
                }
        }
        closedir(cdir);
        return retval;
}

int
command_read(char *commandfile)
{
        /*
         * This function reads and processes the commandfile. It returns the
         * action to be taken.
         */

        FILE           *fp;
        char            buf[256];

        cmdval = 0;
        chdir(workdir);

        if (!strcmp(commandfile, HALTFILE)) {
                return CMD_STOP;
        }
        if (!strcmp(commandfile, KILLFILE)) {
                fp = fopen(commandfile, "r");
                fgets(buf, sizeof(buf) - 1, fp);
                cmdval = atoi(buf);
                fclose(fp);
                unlink(commandfile);
                return CMD_KILL;
        }
        if (!strcmp(commandfile, NICEFILE)) {
                fp = fopen(commandfile, "r");
                fgets(buf, sizeof(buf) - 1, fp);
                cmdval = atoi(buf);
                fclose(fp);
                unlink(commandfile);
                return CMD_RENICE;
        }
        return 0;
}

void
command_handle(int command, pid_t pid)
{
        /*
         * This functions takes the action and acts upon it. The command and
         * the process ID to work on are the arguments. Returns nothing.
         */

        switch (command) {
                case CMD_KILL:
                vbprintf("Sending signal %d to pid %d\n",
                         cmdval, pid);
                kill(pid, cmdval);
                break;
        case CMD_RENICE:
                vbprintf("Renicing pid %d to value %d\n",
                         pid, cmdval);
                renice(pid, cmdval);
                break;
        case CMD_STOP:
                vbprintf("Holding off spawning new processes.\n");
                break;
        default:
                break;
        }
        return;
}

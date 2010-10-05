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
 * $Id: main.c,v 1.2 2003/12/09 09:49:10 coolvibe Exp $
 */

#ifndef lint
static          const char copyright[] = "Copyright (c) 2003 Emiel Kollof <coolvibe@hackerheaven.org>";
#endif

#ifndef lint
static          const char rcsid[] = "$Id: main.c,v 1.2 2003/12/09 09:49:10 coolvibe Exp $";
#endif

#include "angel.h"
#include "usage.h"

void
usage(void)
{
        /*
         * Usage statement. See usage.h.
         */

        printf("%s", usageinfo);
        exit(EX_USAGE);
}

void
mainloop(void)
{
        /*
         * This is the main program loop. Here's where we're doing all the
         * spawning and the checking. This is the heart of the program.
         */

        int             exitcode, status, command = CMD_OK, event = EV_AOK;

        while (1) {

                event = event_check();
                command = command_check();
                command_handle(command, pid);

                if (command == CMD_OK && event == EV_AOK)
                        pid = fork();
                else
                        pid = -2;

                switch (pid) {
                case 0: /* we are the child */
                        vbprintf("(child) Executing %s in work directory %s\n"
                                 ,cmd, workdir);
                        chdir(workdir);

                        logging_rotate();
                        logging_init();
                        exitcode = execl(cmd, cmd, args,
                                         (char *)NULL);
                        if (exitcode < 0) {
                                perror("execl");
                        }
                        exit(exitcode);
                        break;
                case -1:        /* error */
                        err(EX_OSERR, "Couldn't fork: ");
                        break;
                case -2:        /* stop */
                        vbprintf("Not reforking. Waiting %d seconds\n", delay);
                        sleep(delay);
                        break;
                default:        /* we are the parent */
                        vbprintf("(parent) Forked process with pid %d\n", pid);
                        while (waitpid(pid, &status, WNOHANG) == 0) {

                                /* Do something while child is running */

                                dbprintf("Waiting for child (%d) to terminate\n",
                                        pid);
                                running = TRUE;
                                command = command_check();
                                command_handle(command, pid);

                                sleep(delay);
                        }
                        /* if we get here, the child process must have died */
                        running = FALSE;
                        command = command_check();
                        command_handle(command, pid);
                        break;
                }
                if (command == CMD_STOP) {

                        struct stat     st;

                        /* check if HALTFILE still exists */
                        chdir(workdir);
                        if (stat(HALTFILE, &st) < 0) {
                                vbprintf("Haltfile gone. Resuming...\n");
                                command = CMD_OK;
                        }
                        if (event == EV_AOK) {
                                vbprintf("Event over. Resuming...\n");
                                command = CMD_OK;
                        }
                }
        }
        return;
}


int
main(int argc, char **argv)
{
        /*
         * The main() function. The be-all end-all of every C program. If you
         * don't know the purpose of main(), you probably shouldn't be
         * reading this and learn C first.
         */

        if (argc == 1) {        /* check argv[0] */
                char           *buf;    /* buffer for cmdname */

                /*
                 * first check if we're called by our own name, call usage()
                 * when we are.
                 */

                /*
                 * XXX getprogname() only exist (AFAIK) on FreeBSD and NetBSD
                 * (and maybe OpenBSD). Maybe I have to implement my own (or
                 * copy it from one of those platforms). I'd rather use
                 * something that is in wide use by a base system of an OS
                 * than to kludge something together badly.
                 */

                if (!strcmp(getprogname(), "angel"))
                        usage();

                buf = (char *)malloc(strlen(argv[0] + 1));
                strcpy(buf, getprogname());     /* always smaller, thus safe */
                cbuf = (char *)config_read(buf);
                config_feed(cbuf);

                /* Clean up allocated mem */
                free(cbuf);
                free(buf);
        } else {
                printf("Don't run me like that... Read angel's documentation\n");
                exit(EX_USAGE);
        }

        /* become daemonized if we need to */
        if (!nodetach) {
                daemon(1, 1);
        }
        /* Start main program loop */
        mainloop();

        return (0);
}

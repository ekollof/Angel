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
 * $Id: usage.h,v 1.2 2003/12/09 09:49:10 coolvibe Exp $
 */

/*
 * This is a usage statement. It gets printed by usage() located in main.c
 */

char            usageinfo[] = \
"\n"
"This is Angel, the kick in the butt for unreliable programs.\n"
"\n"
"This program was written by Emiel Kollof <coolvibe@hackerheaven.org>.\n"
"\n"
"Angel is a program that runs other programs and restarts them if they crash\n"
"or die for some unknown reason. It's also handy to instantly daemonize and \n"
"restart some programs that are not really meant to be daemons. You can also\n"
"use it to keep restarting some kiosk program when some hapless user finds\n"
"out how to exit a program. Angel can start it right up when it dies.\n"
"\n"
"Angel needs configuration files that outline how to execute what progam and\n"
"with which arguments. Read angel's documentation on how to set that up.\n"
"\n"
"You can also control what happens to the processes that are run by touching\n"
"or creating files in the working directories that have special names and\n"
"contents.\n"
"\n"
"Some examples: \n"
"\n"
"echo 1 > " KILLFILE "\t\t# Signals the running process with SIGHUP\n"
"echo -20 > " NICEFILE "\t# renices the running process with niceness -20\n"
"touch " HALTFILE "\t\t# Stops angel from respawning (until it's removed)\n";

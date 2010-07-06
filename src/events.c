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
 * $Id: events.c,v 1.3 2003/12/09 10:05:27 coolvibe Exp $
 */

#ifndef lint
static          const char copyright[] = "Copyright (c) 2003 Emiel Kollof <coolvibe@hackerheaven.org>";
#endif

#ifndef lint
static          const char rcsid[] = "$Id: events.c,v 1.3 2003/12/09 10:05:27 coolvibe Exp $";
#endif

#include "angel.h"

int
event_check(void)
{
        /*
         * This function is used for checking events. Currently, only a load
         * average check is performed. But it can be extended to include a
         * whole range of checks. Returns the event or EV_AOK when everything
         * is between acceptable parameters.
         */

        double          load[3];
        static int      counter;
        int retval = EV_AOK;

        /* check load average */
        if (maxload != -1) {
                if (getloadavg(load, 3) < 0) {
                        warnx("getloadavg failed");
                }
                if (maxload < load[0]) {
                        if (counter == 0)
                                vbprintf("Load too high. Setting event to EV_LOADTOOHIGH\n");
                        counter++;

                        retval = EV_LOADTOOHIGH;
                } else {
                        counter = 0;

                        dbprintf("event_check: maxload: %.2f currload %.2f\n",
                                maxload, load[0]);
                        retval = EV_AOK;
                }
        }
        return retval;
}

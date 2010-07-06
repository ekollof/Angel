/* Simple piece of code that waits an amount of seconds specified on the
 * command line. Feel free to do with this what you want. I don't care.
 *
 * $Id: test.c,v 1.2 2003/12/09 19:48:51 coolvibe Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int ac, char **av)
{
        int secs;

        if (ac < 2) {
                printf("Args!\n");
                exit(1);
        }
        secs = atoi(av[1]);
        printf("Test app for angel...\n");
        sleep(secs);
        printf("Done sleeping after %d seconds.\n", secs);
        return secs;
}

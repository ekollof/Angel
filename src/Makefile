# Makefile for Angel, the kick in the butt for unreliable programs
#
# Please make your modifications, but don't touch anything below the
# dotted line, unless you plan to help developing this of course.

# the default name of the file to hold off forking
HALTFILE=.stop

# the default name of the file to kill the running process
KILLFILE=.kill

# the default name of the file to renice the running process
NICEFILE=.nice

# The default name of the command file
CMDFILE=angel-command

# The system-wide directory where all the configuration files
# are stored in
SYSCONFIGDIR=/etc/angel

# The directory in user's $HOME where all the configuration files
# are stored in
USRCONFIGDIR=.angel

# Any optimization flags you wish to add
COPTFLAGS=

# No user-serviceable parts below this line.
### ### ### ### ### ### ### ### ### ### ### ### ### ### ### ### ### ### ### ###

OBJS=main.o config.o vars.o utils.o command.o events.o logging.o
SRCS=main.c config.c vars.c utils.c command.c events.c logging.c
HEADERS=angel.h
EXEC=angel

CFLAGS+=$(COPTFLAGS) -g3
CFLAGS+=-DSYSCONFIGDIR=\"$(SYSCONFIGDIR)\" -DUSRCONFIGDIR=\"$(USRCONFIGDIR)\"
CFLAGS+=-DHALTFILE=\"$(HALTFILE)\" -DKILLFILE=\"$(KILLFILE)\"
CFLAGS+=-DNICEFILE=\"$(NICEFILE)\"

all: $(HEADERS) $(EXEC)

$(EXEC): $(OBJS)
	$(CC) -o $(EXEC) $(OBJS) $(LDFLAGS)

tagfile:
	ctags *.[ch]

clean:
	-rm -f $(EXEC) $(OBJS) *~ *.core core *.BAK tags

indent:
	for i in *.[ch]; do indent $$i; done

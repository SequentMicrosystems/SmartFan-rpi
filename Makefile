DESTDIR?=/usr
PREFIX?=/local

ifneq ($V,1)
Q ?= @
endif

CC	= gcc
CFLAGS	= $(DEBUG) -Wall -Wextra $(INCLUDE) -Winline -pipe 

LDFLAGS	= -L$(DESTDIR)$(PREFIX)/lib
LIBS    = -lpthread -lrt -lm -lcrypt

SRC	=	src/fan.c src/comm.c

OBJ	=	$(SRC:.c=.o)

all:	fan

fan:	$(OBJ)
	$Q echo [Link]
	$Q $(CC) -o $@ $(OBJ) $(LDFLAGS) $(LIBS)

.c.o:
	$Q echo [Compile] $<
	$Q $(CC) -c $(CFLAGS) $< -o $@

.PHONY:	clean
clean:
	$Q echo "[Clean]"
	$Q rm -f $(OBJ) fan *~ core tags *.bak

.PHONY:	install
install: fan
	$Q echo "[Install]"
	$Q cp fan		$(DESTDIR)$(PREFIX)/bin
ifneq ($(WIRINGPI_SUID),0)
	$Q chown root.root	$(DESTDIR)$(PREFIX)/bin/fan
	$Q chmod 4755		$(DESTDIR)$(PREFIX)/bin/fan
endif
#	$Q mkdir -p		$(DESTDIR)$(PREFIX)/man/man1
#	$Q cp megaio.1		$(DESTDIR)$(PREFIX)/man/man1

.PHONY:	uninstall
uninstall:
	$Q echo "[UnInstall]"
	$Q rm -f $(DESTDIR)$(PREFIX)/bin/fan
	$Q rm -f $(DESTDIR)$(PREFIX)/man/man1/fan.1

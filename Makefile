ARCH = msp430x2274
CC := msp430-gcc
UIF_TTY = /dev/ttyUSB0

CFLAGS := -g -mmcu=${ARCH} -Wall -O3 -std=gnu99
CFLAGS += -include `pwd`/config.h
LDFLAGS :=

O_FILES = main.o smps.o cmds.o
SUBDIRS = drivers libsric

LDFLAGS += -Ldrivers -ldrivers
LDFLAGS += -Llibsric -lsric

all: servo

include depend

servo: ${O_FILES} ${SUBDIRS}
	${CC} -o $@ ${O_FILES} ${CFLAGS} ${LDFLAGS}

drivers:
	$(MAKE) -C $@ CC=${CC} ARCH=${ARCH} CFLAGS="${CFLAGS}" LDFLAGS="${LDFLAGS}"

libsric:
	$(MAKE) -C $@ CC=${CC} ARCH=${ARCH} CFLAGS="${CFLAGS} -I`pwd`" LDFLAGS="${LDFLAGS}"

depend: *.c
	rm -f depend
	for file in $^; do \
		${CC} ${CFLAGS} -MM $$file -o - >> $@ ; \
	done ;

.PHONY: clean ${SUBDIRS} flash

flash: servo
	mspdebug uif -d ${UIF_TTY} "prog $<"

clean:
	-rm -f servo depend *.o
	for d in ${SUBDIRS} ; do\
		${MAKE} -C $$d clean ; \
	done ;


#
# This file and its contents are supplied under the terms of the
# Common Development and Distribution License ("CDDL"), version 1.0.
# You may only use this file in accordance with the terms of version
# 1.0 of the CDDL.
#
# A full copy of the text of the CDDL should have accompanied this
# source.  A copy of the CDDL is also available via the Internet at
# http://www.illumos.org/license/CDDL.
#

#
# Copyright 2011 Nexenta Systems, Inc.  All rights reserved.
# Copyright 2011 EveryCity Ltd. All rights reserved.
# Copyright 2013 DEY Storage Systems, Inc.
# Copyright 2015 Joyent, Inc.
#

PROG=localedef

NDIR=native
NPROG=$(NDIR)/localedef

include ../Makefile.cmd

OBJS = 	charmap.o collate.o ctype.o messages.o monetary.o numeric.o time.o \
	scanner.o localedef.o wide.o parser.tab.o
NOBJS = $(OBJS:%.o=native/%.o) native/avl.o native/mkdirp.o

HDRS	= localedef.h

SRCS   = $(OBJS:%.o=%.c)

CPPFLAGS	+= -I $(SRC)/lib/libc/port/locale
NATIVE_CPPFLAGS	+= -I $(SRC)/lib/libc/port/locale
CERRWARN	+= -_gcc=-Wno-char-subscripts
CERRWARN	+= -_gcc=-Wno-uninitialized
CERRWARN	+= -_gcc=-Wno-unused-label
LDLIBS		+= -lgen
LDLIBS 		+= -lavl

NATIVELDLIBS=

YFLAGS		= -d -b parser

CLEANFILES	= $(NOBJS) $(OBJS) parser.tab.c parser.tab.h
CLOBBERFILES	= $(PROG) $(NPROG)

all: $(PROG) $(DATA) build-data

install: all $(ROOTPROG) install-data

$(NDIR):
	-@mkdir -p $@

$(NDIR)/%.o: %.c
	$(NATIVECC) $(NATIVE_CFLAGS) $(NATIVE_CPPFLAGS) -o $@ -c $<

$(NDIR)/avl.o : ../../common/avl/avl.c
	$(NATIVECC) $(NATIVE_CFLAGS) $(NATIVE_CPPFLAGS) -o $@ -c \
	  ../../common/avl/avl.c

$(NDIR)/mkdirp.o : ../../lib/libgen/common/mkdirp.c
	$(NATIVECC) $(NATIVE_CFLAGS) $(NATIVE_CPPFLAGS) -o $@ -c \
	  ../../lib/libgen/common/mkdirp.c

$(NPROG): $(NDIR) .WAIT $(NOBJS)
	$(LINK.c) $(NOBJS) -o $@ $(NATIVELDLIBS)
	$(POST_PROCESS)

$(PROG): $(OBJS)
	$(LINK.c) $(OBJS) -o $@ $(LDLIBS)
	$(POST_PROCESS)

$(OBJS) $(NOBJS):	parser.tab.h

parser.tab.c parser.tab.h: parser.y $(HDRS)
	$(YACC) $(YFLAGS) parser.y

lint:	$(SRCS)
	$(LINT.c) $(CPPFLAGS) $(SRCS)

clean:	
	$(RM) $(CLEANFILES)
	/usr/bin/bmake -C data clean

clobber: clean
	$(RM) $(CLOBBERFILES)
	$(RM) -r $(NDIR)
	$(RM) -r $(ROOTLIB)/locale/*.*
	/usr/bin/bmake -C data cleandir

$(ROOTBIN)/%: $(ROOTBIN) %
	$(INS.file)

build-data: FRC $(NPROG)
	/usr/bin/bmake -C data -j $(DMAKE_MAX_JOBS) PATH=$$(pwd)/$(NDIR):$${PATH} all

install-data: FRC
	/usr/bin/bmake -C data install INSTALL=/usr/bin/install DESTDIR=${ROOT} MK_INSTALL_AS_USER=yes

FRC:

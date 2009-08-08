
sysconfdir = /etc
localedir = /usr/share/locale
version := $(shell dpkg-parsechangelog | grep '^Version' | cut -b10-)

CC = gcc 
CFLAGS = -g -Wall -Werror
ALL_CFLAGS = $(CFLAGS) -std=gnu99 -Ilib \
	 -DENABLE_L_DEBUG -D_GNU_SOURCE -DSYSCONF=\"$(sysconfdir)\" \
         -DHAVE_GETTEXT -DPACKAGE=\"dctrl-tools\" -DLOCALEDIR=\"$(localedir)\" 

ALL_CFLAGS += -DVERSION=\"$(version)\"
ALL_CFLAGS += -DMAINTAINER='"$(shell grep ^Maintainer: debian/control | cut -b13-)"'

#CFLAGS += -DNDEBUG

#CFLAGS += -pg
#LDFLAGS += -pg

libsrc = $(wildcard lib/*.c)
libobj = $(libsrc:.c=.o)

src = $(libsrc) \
      $(wildcard grep-dctrl/*.c) \
      $(wildcard sort-dctrl/*.c) \
      $(wildcard tbl-dctrl/*.c) \
      $(wildcard join-dctrl/*.c)

obj = $(src:.c=.o)

exe =  	grep-dctrl/grep-dctrl \
	sort-dctrl/sort-dctrl \
	tbl-dctrl/tbl-dctrl \
	join-dctrl/join-dctrl

LDLIBS = -L. -ldctrl

# List of translated languages is given in langs.mk
include langs.mk

all :	all-no-mo mo

all-no-mo :	sync-available/sync-available \
		man/grep-dctrl.1 \
		$(exe)

pot : po/dctrl-tools.pot

po : $(foreach f,$(langs),po/$(f).po)

mo : $(foreach f,$(langs),po/$(f).mo)

grep-dctrl/grep-dctrl : grep-dctrl/grep-dctrl.o grep-dctrl/rc.o libdctrl.a

sort-dctrl/sort-dctrl : sort-dctrl/sort-dctrl.o libdctrl.a

tbl-dctrl/tbl-dctrl : tbl-dctrl/tbl-dctrl.o libdctrl.a

join-dctrl/join-dctrl : join-dctrl/join-dctrl.o libdctrl.a

% : %.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

%.o : %.c
	$(CC) $(ALL_CFLAGS) -c -o $@ $<

%.d: %.c
	$(CC) -M $(ALL_CFLAGS) $< > $@.$$$$; \
	   sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	   rm -f $@.$$$$


libdctrl.a : $(libobj)
	ar cr $@ $^
	ranlib $@

libdctrl.so : $(soobj)
	$(LD) -shared -o $@ $^ -lc $(SOLDLIBS)

%.test : %.test.o

%.test.o : %.c
	$(CC) -c $(CFLAGS) -DTESTMAIN $< -o $@

so/%.o : %.c
	$(CC) -fPIC $(CFLAGS) -c $< -o $@

%.1 : %.1.cp
	sed 's*SYSCONF*$(sysconfdir)*' $< > $@

sync-available/sync-available : sync-available/sync-available.cp
	sed 's*VERSION*$(version)*' $< > $@
	chmod 755 $@

xgettext_opts=--copyright-holder="Antti-Juhani Kaijanaho" \
	      --msgid-bugs-address="ajk@debian.org" -kN_ -k_

po/%.po : po/dctrl-tools.pot
	msgmerge -q -U --backup=existing $@ $^

po/%.mo : po/%.po
	msgfmt -c --statistics -o $@ $< 

po/dctrl-tools.pot : $(src)
	xgettext $(xgettext_opts) -d grep-dctrl $^
	mv grep-dctrl.po $@

fsaf.test : fsaf.test.o msg.o

test :	all-no-mo
	sh ./tester.sh

clean :
	$(RM) core $(exe) man/grep-dctrl.1 $(obj) so/*.o libdctrl.a libdctrl.so
	$(RM) po/*.mo TAGS *.d */*.d
	$(RM) sync-available/sync-available

distclean : clean

maintainer-clean : distclean
	$(RM) po/*.pot

tags :
	etags *.[hc]

ifeq ($(MAKECMDGOALS),clean)
else ifeq ($(MAKECMDGOALS),distclean)
else ifeq  ($(MAKECMDGOALS),maintainer-clean)
else
include $(obj:.o=.d)
endif

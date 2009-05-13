###############################################
#      Makefile for single target projects    #
#                Andreas Gölzer               #
#                 version 0.31                 #
#        2006, last update 2008-03-05         #
###############################################

name := readesm
headers = $(wildcard *.h)
objects = $(headers:.h=.o)
extrafiles = $(name).doxygen EC_PK.bin batchall.sh COPYING README images.tar.bz2  $(name)-wrap-kde.sh $(name)-wrap-firefox german.po
run_args = 
sources = $(name).cpp $(filter $(wildcard *.cpp), $(objects:.o=.cpp)) $(filter $(wildcard *.h), $(objects:.o=.h))

prefix=/usr/local
distribution = $(sources) $(extrafiles) Makefile
uploadtarget=www.goelzer.de@ssh.strato.de:andreas/wordpress/download
uploader=scp -p

SHELL=/bin/bash
TEX=latex
CFLAGS=-Wall -pipe -Werror -O2
CXX=g++
CPPFLAGS=-DPREFIX=\"$(prefix)\"
CXXFLAGS=$(CFLAGS)
LDLIBS=-lgmp -lboost_program_options -lgcrypt

all: $(name)

$(name) : $(patsubst %.cpp, %.o, $(filter $(wildcard *.cpp), $(objects:.o=.cpp)))
$(name).o: $(filter $(wildcard *.h), $(objects:.o=.h))

$(name).pot: $(sources)
	xgettext -d $(name) -a -s --from-code utf-8 -o $(name).pot $(sources)
german.po: $(name).pot
	msgmerge -s -U $@ $<
german.mo: german.po
	msgfmt -c -v -o $@ $<

readesm-wrap-kde: readesm-wrap-kde.sh
	sed -e s,PREFIX,$(prefix), > $@

.PHONY:all clean distclean doc dist backup depend run upload install uninstall packageinstall debian

run: $(name)
	./$(name) $(run_args)

install: $(name) german.mo readesm-wrap-kde
	install -d $(prefix)/share/readesm
	install -d $(prefix)/bin
	install -d $(prefix)/share/locale/de/LC_MESSAGES
	install -m 755 $(name) $(prefix)/bin
	install -m 755 $(name)-wrap-kde $(prefix)/bin
	install -m 755 $(name)-wrap-firefox $(prefix)/bin
	install -m 644 images.tar.bz2 $(prefix)/share/readesm
	install -m 644 german.mo $(prefix)/share/locale/de/LC_MESSAGES/$(name).mo
	install -m 644 EC_PK.bin $(prefix)/share/readesm


packageinstall: $(name)
	checkinstall -y --maintainer "Andreas Goelzer \<andreas@goelzer.de\>" --pkgsource "http://andreas.goelzer.de/download/$(name).tar.bz2" --pkggroup text --requires "libgcrypt11, libgmp3c2, libboost-program-options1.34.1"

uninstall:
	rm $(prefix)/bin/$(name)
	rm $(prefix)/bin/$(name)-wrap-kde
	rm $(prefix)/bin/$(name)-wrap-firefox
	rm $(prefix)/share/images.tar.bz2
	rm $(prefix)/share/locale/de/LC_MESSAGES/$(name).mo
	rm $(prefix)/share/readesm/EC_PK.bin

profiling: $(name)
	valgrind --tool=callgrind ./$(name) $(run_args)

depend: 
	@echo Makedepend is pretty insane, and will report lots of warnings about missing iostream and
	@echo similar standard headers. That is okay, or more precisely can hardly be prevented.
	makedepend -Y $(sources)


clean:
	-$(RM) $(RMFLAGS) $(name) $(objects) $(name).o readesm-wrap-kde

distclean: clean
	-rm -r html/ latex/
	-rm *~
	@echo Directory should be clean now, except for the .tar.bz2 files which
	@echo are created by \"make dist\" and \"make backup\". Remove them yourself
	@echo if you have to.
	@echo The following files in this directory would normally not be included in the
	@echo distribution:
	@ls -hl $(addprefix -I ,$(distribution))

dist: $(name).tar.bz2
$(name).tar.bz2: $(distribution)
	@echo Creating $(name).tar.bz2 ...
	@tar -C .. -chvjf $(name).tar.bz2 $(addprefix $(name)/,$(distribution))

debian:
	debuild -S -sa -k4B41883C -i
	#Dput my-ppa ../readesm_*.changes

upload: dist
	$(uploader) $(name).tar.bz2 $(uploadtarget)
	scp $(name).tar.bz2 evil_k@frs.sourceforge.net:uploads
	
backup: dist
	@echo Copying $(name).tar.bz2 to $(name)-backup-`date +%Y%m%d-%H%M`.tar.bz2
	@cp $(name).tar.bz2 $(name)-backup-`date +%Y%m%d-%H%M`.tar.bz2

doc: html/index.html
html/index.html: $(sources) $(name).doxygen
	doxygen $(name).doxygen

#-- additional implicit rules --

%.pdf : %.eps
	epstopdf $<
%.pdf : %.tex
	pdflatex $<

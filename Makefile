include makeinclude

default: test-OpDesk test-app

test-app: FORCE
	( cd test-app; make )

Fl_OpButton.o: Fl_OpButton.C Fl_OpButton.H Fl_OpBox.H Fl_OpDesk.H
	${CXX} ${CXXFLAGS} -fsanitize=address ${INC} ${FLTKCXXFLAGS} $< -c

Fl_OpBox.o: Fl_OpBox.C Fl_OpBox.H Fl_OpButton.H Fl_OpDesk.H
	${CXX} ${CXXFLAGS} -fsanitize=address ${INC} ${FLTKCXXFLAGS} $< -c

Fl_OpDesk.o: Fl_OpDesk.C Fl_OpDesk.H Fl_OpButton.H Fl_OpBox.H
	${CXX} ${CXXFLAGS} -fsanitize=address ${INC} ${FLTKCXXFLAGS} $< -c

test-OpDesk.o: test-OpDesk.C Fl_OpDesk.o
	${CXX} ${CXXFLAGS} -fsanitize=address ${INC} ${FLTKCXXFLAGS} $< -c

test-OpDesk: Fl_OpDesk.o Fl_OpButton.o Fl_OpBox.o test-OpDesk.o
	${CXX} -fsanitize=address ${LDFLAGS} \
		Fl_OpDesk.o Fl_OpBox.o Fl_OpButton.o test-OpDesk.o \
		$(FLTKIMAGELIBS) -o $@

default: test-OpDesk
	
foo: foo.C
	echo Compiling $@
	$(CXX) ${INC} ${CXXFLAGS} foo.C $(FLTKIMAGELIBS) $(LDFLAGS) -o $@

clean:
	-rm -f *.o test-OpDesk 2> /dev/null
	-rm -f *.obj 2> /dev/null
	-rm -f *.exe 2> /dev/null
	-rm -f *.manifest 2> /dev/null
	-rm -f *.ilk  2> /dev/null
	-rm -f *.pdb 2> /dev/null
	-rm -f core 2> /dev/null
	-rm -f core.[0-9]* 2> /dev/null
	( cd test-app; make clean )
	( cd documentation; make clean )

docs: FORCE
	( cd documentation; make docs )

# For releases of Fl_OpDesk
tar: FORCE
	make clean
	if grep -q ERCODEBUG *.[CH] test-app/*.[CH]; then \
	    echo '### NO DEBUG CODE IN RELEASE'; \
	    grep ERCODEBUG *.[CH] test-app/*.[CH]; \
	    exit 1; \
	fi
	make docs
	#
	# Purpose of the following:
	#	o Automatically include version number from CHANGES.txt file
	#       o 'make clean' to get rid of core files, etc
	#       o 'cd documentation; make' to make the docs (include in tar file)
	#	o Both tar file and subdir name within has dirname
	#       o Expand tabs (detab)
	#	o Inject LGPL header into all code (so main copy is easier to read)
	#       o Make sure FLTKCONFIG line in makeinclude has no absolute path (Kai-Uwe Behrmann,07/22/11)
	#	o Clean tar file, no junk files/dirs
	#	o Proper modes on files; dirs 755, files 644
	#
	( export VERSION=`awk '/^[0-9]*\.[^ \t]/ {print $$1; exit(0);}'<CHANGES.txt`; \
	  TARFILE=/var/tmp/Fl_OpDesk-$$VERSION.tar.gz; \
	  RELDIR=.release/Fl_OpDesk-$$VERSION; \
	  rm -rf documentation/images/.xvpics; \
	  make clean; \
	  cd documentation; make; cd ..; \
	  if [ -d .release ]; then rm -rf .release; fi; \
	  mkdir -p $$RELDIR; \
	  for i in *.[CH]; do \
	      perl ./etc/lgpl-header.pl $$i | perl ./detab.pl > $$RELDIR/$$i; \
	  done; \
	  cp Make* makeinclude make.bat CHANGES.txt COPYING.txt TODO README.txt $$RELDIR; \
	  sed 's%^FLTKCONFIG.*%FLTKCONFIG    = fltk-config%' < makeinclude > $$RELDIR/makeinclude; \
	  mkdir $$RELDIR/documentation; \
	  mkdir $$RELDIR/test-app; \
	  cp documentation/Doxyfile $$RELDIR/documentation; \
	  cp -rp documentation/html $$RELDIR/documentation/html; \
	  cp -rp etc $$RELDIR/etc; \
	  cp -p test-app/* $$RELDIR/test-app; \
	  for i in test-app/*.[CH]; do \
	      perl ./etc/lgpl-header.pl $$i > $$RELDIR/$$i; \
	  done; \
	  cp TODO $$RELDIR; \
	  find .release -type d -exec chmod 755 {} \; ; \
	  find .release -type f -exec chmod 644 {} \; ; \
	  ( cd .release && tar cvfz $$TARFILE '--exclude=.nfs*' --owner=0 --group=0 Fl_OpDesk-$$VERSION ); \
	  echo "*** Created: $$TARFILE"; \
	  if [ -x ./.upload ]; then ./.upload $$TARFILE; fi; \
	)

# DO NOT REMOVE THE FOLLOWING LINES
FORCE:

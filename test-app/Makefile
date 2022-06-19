include ../makeinclude

OPDESKOBJS=../Fl_OpDesk.o ../Fl_OpButton.o ../Fl_OpBox.o
TESTAPPOBJS=MyConstantDialog.o MyBoxInfoDialog.o \
	    MyBox.o MyButton.o MyDesk.o Subs.o 

default: test-app

test-app.o: test-app.C MyButton.H MyBox.H MyDesk.H MyConstantDialog.H Subs.H
	${CXX} ${CXXFLAGS} ${INC} ${FLTKCXXFLAGS} -I.. $< -c

MyConstantDialog.o: MyConstantDialog.C MyConstantDialog.H
	${CXX} ${CXXFLAGS} ${INC} ${FLTKCXXFLAGS} -I.. $< -c

MyBoxInfoDialog.o: MyBoxInfoDialog.C MyBoxInfoDialog.H
	${CXX} ${CXXFLAGS} ${INC} ${FLTKCXXFLAGS} -I.. $< -c

MyBox.o: MyBox.C MyBox.H
	${CXX} ${CXXFLAGS} ${INC} ${FLTKCXXFLAGS} -I.. $< -c
 
MyButton.o: MyButton.C MyButton.H
	${CXX} ${CXXFLAGS} ${INC} ${FLTKCXXFLAGS} -I.. $< -c
 
MyDesk.o: MyDesk.C MyDesk.H
	${CXX} ${CXXFLAGS} ${INC} ${FLTKCXXFLAGS} -I.. $< -c
 
Subs.o: Subs.C Subs.H
	${CXX} ${CXXFLAGS} ${INC} ${FLTKCXXFLAGS} -I.. $< -c
 
test-app: $(OPDESKOBJS) $(TESTAPPOBJS) test-app.o
	${CXX} ${LDFLAGS} $(OPDESKOBJS) $(TESTAPPOBJS) test-app.o $(FLTKIMAGELIBS) -o $@


clean: FORCE
	-rm -f test-app 2> /dev/null
	-rm -f *.o 2> /dev/null
	-rm -f *.obj 2> /dev/null
	-rm -f *.exe 2> /dev/null
	-rm -f *.manifest 2> /dev/null
	-rm -f *.ilk  2> /dev/null
	-rm -f *.pdb 2> /dev/null
	-rm -f core 2> /dev/null
	-rm -f core.[0-9]* 2> /dev/null

FORCE:
	

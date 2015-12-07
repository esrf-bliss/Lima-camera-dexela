dexela-objs = DexelaBinCtrlObj.o DexelaInterface.o DexelaSyncCtrlObj.o DexelaDetInfoCtrlObj.o \
DexelaReconstructionCtrlObj.o

SRCS = $(dexela-objs:.o=.cpp)

GLIB_INCLUDES = $(shell pkg-config --cflags glib-2.0)

CXXFLAGS += -I../include -I../../../hardware/include -I../../../common/include \
	$(GLIB_INCLUDES) \
	-I/usr/local -DOS_LINUX \
	-I../../../third-party/Processlib/core/include\
        -Wall -pthread -fPIC -g

all:	Dexela.o

Dexela.o:	$(dexela-objs)
	$(LD) -o $@ -r $+

clean:
	rm -f *.o *.P

%.o : %.cpp
	$(COMPILE.cpp) -MD $(CXXFLAGS) -o $@ $<
	@cp $*.d $*.P; \
	sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
	-e '/^$$/ d' -e 's/$$/ :/' < $*.d >> $*.P; \
	rm -f $*.d


-include $(SRCS:.cpp=.P)



.PHONY: check-syntax
check-syntax:
	$(CXX) -Wall -Wextra -fsyntax-only $(CXXFLAGS) $(CHK_SOURCES)

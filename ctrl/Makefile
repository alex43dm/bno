ifndef VER
VER = 1
endif
ifndef REL
REL = X
endif

.SUFFIXES:
.SUFFIXES:		.o .c .cpp .a

ifndef OPTIM
OPTIM = -Or
endif
ifndef DEBUG
DEBUG = -g1
endif
#OBJS = ctrl.o 

LIBS = -l socket -l unix -l mqueue
NAME = ctrl
VNAME = $(NAME).$(VER)$(REL)

BIN = Bin/$(VNAME)
MAPF = Maps/$(VER)$(REL)

CFLAGS =
CFLAGS = -3 -ms $(OPTIM) $(DEBUG) 
VPATH = ../

$(BIN):	$(OBJS) Makefile
	cc -c $(CFLAGS) version.c -D NAME=\"$(NAME)\" -D VERSION=$(VER) -D RELEASE=\"$(REL)\"
	cc -c $(CFLAGS) ctrl.cpp -D NAME=\"$(NAME)\" -D VERSION=$(VER) -D RELEASE=\"$(REL)\"
	cc -o $(BIN) "-Wl,OP MAP=$(MAPF)" -T 1 -b -N4096 "-Wl,op H=512" $(CFLAGS) $(OBJS) version.o $(LIBS)
	print "Version : $(VER)$(REL)" | cat Use - | usemsg $(BIN) -

install:
	beirut cp -v $(BIN) /bin/$(VNAME)

clean:
	rm -f *.o *.err $(BIN)


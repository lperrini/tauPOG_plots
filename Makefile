CXX=g++
CXXFLAGS=-O -Wall -fPIC
RINT=-DWITHRINT
THREAD_LIB=-lpthread

#use TRint or TApplication
CXXFLAGS += $(RINT)

## Libs
ROOTCXXFLAGS = $(shell root-config --cflags)
ROOTLDFLAGS  = $(shell root-config --ldflags)
ROOTLIBS     = $(shell root-config --libs)
ROOTGLIBS    = $(shell root-config --glibs)

LIBS  = ${THREAD_LIB} -lcurses 
LIBS += $(ROOTLIBS)

CXXFLAGS += $(ROOTCXXFLAGS)

#objects
PLOTT_OBJ  = Plotter.o
PLMAK_OBJ  = PlotMaker.o
##Build
all: PlotMaker Plotter

Plotter:	$(PLOTT_OBJ)
		@echo ""
		@echo "Compiling objects ($(PLOTT_OBJ)) ... "
		@$(CXX) $(PLOTT_OBJ) $(ROOTLDFLAGS) -o Plotter PlotMaker.o -g $(LIBS) -lm -lEG

PlotMaker:	$(PLMAK_OBJ)
		@echo ""
		@echo "Compiling objects ($(PLMAK_OBJ)) ... "
		@$(CXX) -c $(PLMAK_OBJ) $(ROOTLDFLAGS) -o -g $(LIBS) -lm -lEG

## Clean
clean:
	@echo "Cleaning path ... "
	rm -rf *.o
	rm -rf Plotter
	rm -rf *~

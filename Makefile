BIN     := ./bin
INC     := ./include
SRC_ACC := ./src-acc
SRC_RAD := ./src-rad

CXX := g++ -std=c++11
CFLAGS := -Wall -g
#CXXFLAGS  += -O2 -Wall -fPIC $(ROOTCFLAGS)
#LD := g++

ROOTCFLAGS  := $(shell root-config --cflags)
ROOTLDFLAGS := $(shell root-config --ldflags)
ROOTINCDIR  := $(shell root-config --incdir)
ROOTLIBS    := $(shell root-config --libs) -lEG
CERNLIBS    := -lpdflib804 -lmathlib -lphtools -lpacklib -lkernlib -lpawlib
LDFLAGS     := -O2 ${ROOTLDFLAGS} -lgfortran -Wl,-rpath,${HAPRAD_CPP}/slib

INCLUDES_RC  := -I$(ROOTINCDIR) -I$(HAPRAD_CPP) -I${INC}
LIBS_RC      := -L$(HAPRAD_CPP)/slib -lTRadCor $(ROOTLIBS) -lMathMore -L/opt/cern/pro/lib/ $(CERNLIBS)

all: ${BIN}/acceptance ${BIN}/centroids ${BIN}/fit-phipq ${BIN}/get-rc

#all: ${BIN}/get-rc

${BIN}/acceptance: ${SRC_ACC}/acceptance.cpp
	${CXX} ${ROOTCFLAGS} -I${INC} ${ROOTLIBS} ${SRC_ACC}/acceptance.cpp -o ${BIN}/acceptance

${BIN}/centroids: ${SRC_RAD}/centroids.cpp
	${CXX} ${ROOTCFLAGS} -I${INC} ${ROOTLIBS} ${SRC_RAD}/centroids.cpp -o ${BIN}/centroids

${BIN}/fit-phipq: ${SRC_RAD}/fit-phipq.cpp
	${CXX} ${ROOTCFLAGS} -I${INC} ${ROOTLIBS} ${SRC_RAD}/fit-phipq.cpp -o ${BIN}/fit-phipq

${BIN}/get-rc: ${SRC_RAD}/get-rc.cpp
		${CXX} ${LDFLAGS} ${ROOTCFLAGS} ${INCLUDES_RC} ${LIBS_RC} ${SRC_RAD}/get-rc.cpp -o ${BIN}/get-rc

#${BIN}/get-rc:${BIN}/get-rc.o
#	$(LD) $(LDFLAGS) $^ $(LIBS) -o $@

#${BIN}/get-rc.o:${SRC_RAD}/get-rc.cpp
#	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm ${BIN}/*

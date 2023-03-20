BIN     := ./bin
INC     := ./include
SRC_ACC := ./src-acc
SRC_RAD := ./src-rad

CXX    := g++ -std=c++11
CFLAGS := -Wall -g

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
	${CXX} ${ROOTCFLAGS} ${SRC_ACC}/acceptance.cpp -I${INC} ${ROOTLIBS} -o ${BIN}/acceptance

${BIN}/centroids: ${SRC_RAD}/centroids.cpp
	${CXX} ${SRC_RAD}/centroids.cpp ${ROOTCFLAGS} -I${INC} ${ROOTLIBS} -o ${BIN}/centroids

${BIN}/fit-phipq: ${SRC_RAD}/fit-phipq.cpp
	${CXX} ${SRC_RAD}/fit-phipq.cpp ${ROOTCFLAGS} -I${INC} ${ROOTLIBS} -o ${BIN}/fit-phipq

${BIN}/get-rc: ${SRC_RAD}/get-rc.cpp
	${CXX} ${SRC_RAD}/get-rc.cpp ${LDFLAGS} ${ROOTCFLAGS} ${INCLUDES_RC} ${LIBS_RC} -o ${BIN}/get-rc

clean:
	rm ${BIN}/*

#${BIN}/get-rc:${BIN}/get-rc.o
#	$(LD) $(LDFLAGS) $^ $(LIBS) -o $@

#${BIN}/get-rc.o:${SRC_RAD}/get-rc.cpp
#	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

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
LDFLAGS     := -O3 ${ROOTLDFLAGS} -lgfortran -Wl,-rpath,./haprad-cpp/slib

INCLUDES_RC  := -I$(ROOTINCDIR) -I./haprad-cpp -I${INC}
LIBS_RC      := -L./haprad-cpp/slib -lTRadCor $(ROOTLIBS) -lMathMore #-L/opt/cern/pro/lib/ $(CERNLIBS)

all: ${BIN}/acceptance ${BIN}/centroids ${BIN}/fit-phipq ${BIN}/apply-rad ${BIN}/get-rc

${BIN}/acceptance: ${SRC_ACC}/acceptance.cpp
	${CXX} ${ROOTCFLAGS} ${SRC_ACC}/acceptance.cpp -I${INC} ${ROOTLIBS} -o ${BIN}/acceptance

${BIN}/centroids: ${SRC_RAD}/centroids.cpp
	${CXX} ${SRC_RAD}/centroids.cpp ${ROOTCFLAGS} -I${INC} ${ROOTLIBS} -o ${BIN}/centroids

${BIN}/fit-phipq: ${SRC_RAD}/fit-phipq.cpp
	${CXX} ${SRC_RAD}/fit-phipq.cpp ${ROOTCFLAGS} -I${INC} ${ROOTLIBS} -o ${BIN}/fit-phipq

${BIN}/get-rc: ${SRC_RAD}/get-rc.cpp
	${CXX} ${SRC_RAD}/get-rc.cpp ${LDFLAGS} ${ROOTCFLAGS} ${INCLUDES_RC} ${LIBS_RC} -o ${BIN}/get-rc

${BIN}/apply-rad: ${SRC_RAD}/apply-rad.cpp
	${CXX} ${SRC_RAD}/apply-rad.cpp ${ROOTCFLAGS} -I${INC} ${ROOTLIBS} -o ${BIN}/apply-rad

clean:
	rm ${BIN}/*

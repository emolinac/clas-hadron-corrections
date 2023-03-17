BIN     := ./bin
INC     := ./include
SRC_ACC := ./src-acc
SRC_RAD := ./src-rad

CXX := g++ -std=c++11
CFLAGS := -Wall -g

ROOTCFLAGS  := $(shell root-config --cflags)
ROOTLDFLAGS := $(shell root-config --ldflags)
ROOTLIBS    := $(shell root-config --libs) -lEG

all: ${BIN}/acceptance ${BIN}/centroids ${BIN}/fit-phipq ${BIN}/get-rc

${BIN}/acceptance: ${SRC_ACC}/acceptance.cpp
	${CXX} -I${INC} ${ROOTCFLAGS} ${SRC_ACC}/acceptance.cpp -o ${BIN}/acceptance ${ROOTLIBS}

${BIN}/centroids: ${SRC_RAD}/centroids.cpp
	${CXX} -I${INC} ${ROOTCFLAGS} ${SRC_RAD}/centroids.cpp -o ${BIN}/centroids ${ROOTLIBS}

${BIN}/fit-phipq: ${SRC_RAD}/fit-phipq.cpp
	${CXX} -I${INC} ${ROOTCFLAGS} ${SRC_RAD}/fit-phipq.cpp -o ${BIN}/fit-phipq ${ROOTLIBS}

${BIN}/get-rc: ${SRC_RAD}/get-rc.cpp
	${CXX} -I${INC} -I$(HAPRAD_CPP) ${ROOTCFLAGS} ${SRC_RAD}/get-rc.cpp -o ${BIN}/get-rc ${ROOTLIBS} -L$(HAPRAD_CPP)/slib

clean:
	rm ${BIN}/*

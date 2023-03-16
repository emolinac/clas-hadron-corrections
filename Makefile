BIN     := ./bin
INC     := ./include
SRC_ACC := ./src-acc
SRC_RAD := ./src-rad

CXX := g++ -std=c++17
CFLAGS := -Wall -g

ROOTCFLAGS  := $(shell root-config --cflags)
ROOTLDFLAGS := $(shell root-config --ldflags)
ROOTLIBS    := $(shell root-config --libs) -lEG

all: ${BIN}/acceptance 

${BIN}/acceptance: ${SRC_ACC}/acceptance.cpp
	${CXX} -I${INC} ${ROOTCFLAGS} ${SRC_ACC}/acceptance.cpp -o ${BIN}/acceptance ${ROOTLIBS}

clean:
	rm ${BIN}/*

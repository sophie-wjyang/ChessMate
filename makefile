CXX=g++
CXXFLAGS=-std=c++14 -Wall -g -w -MMD
EXEC=chess
CCFILES=$(wildcard *.cc)
OBJECTS=${CCFILES:.cc=.o}
DEPENDS=${CCFILES:.cc=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${OBJECTS} -o ${EXEC} -lX11

-include ${DEPENDS}

# Extra recipe that allows us to delete temporary files by saying
# make clean

.PHONY: clean

clean:
	rm ${OBJECTS} ${DEPENDS} ${EXEC}

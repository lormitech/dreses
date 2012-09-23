
#
# Copyright (c) 2009-2010
# Author: Lorenzo Milazzo
#
# All rights reserved.
#



#
#======================= variables =======================
#


#--

PLATFORM = LINUX


#--

SRC_DIR = .
BIN_DIR = ../bin


#--

RM = /bin/rm -f


#--

#
# Libraries
#

# CGAL_ROOT = <CGAL root directory>
CGAL_ROOT=/usr/local/cgal_lib/CGAL-3.5.1

# GMP_ROOT = <GMP root directory>
GMP_ROOT=/usr

# MPFR_ROOT = <MPFR root directory>
MPFR_ROOT=/usr/local/mpfr_lib/mpfr-2.4.2


LIBRARY_SEARCH_PATHS = -L${GMP_ROOT}/lib64 -L${MPFR_ROOT}/lib -L${CGAL_ROOT}/lib

LIBRARY_NAMES = -lgmp -lmpfr -lCGAL
EXTRA_LIBS = -lc -lm


INCLUDES = -I. -I${GMP_ROOT}/include -I${MPFR_ROOT}/include -I${CGAL_ROOT}/include


#--

COMPILER = g++

CC = ${COMPILER} -c -D${PLATFORM}
LINK = ${COMPILER} -D${PLATFORM}

CXXFLAGS = ${CXXOPTIMIZE} ${CXXWARNINGS} ${CXXDEBUG} ${CXXFPR}
CXXOPTIMIZE = -O2
#CXXOPTIMIZE = -O0
CXXWARNINGS = -Wall -W -Wno-deprecated
CXXDEBUG = -g
#CXXPROFILING = -pg
CXXFPR = -frounding-math


#--

PROG = dreses

SOURCES = dreses.cpp geodesic_dist.cpp \
          grid.cpp graph.cpp \
          io_processing.cpp

OBJECTS = $(SOURCES:.cpp=.o)

DEPS = $(SOURCES:.cpp=.d)



#
#==================== rules, targets ====================
#

all: $(BIN_DIR)/$(PROG)

$(BIN_DIR)/$(PROG): $(OBJECTS)
	${LINK} ${OBJECTS} -o $@ ${LIBRARY_SEARCH_PATHS} ${LIBRARY_NAMES} ${EXTRA_LIBS}

test: $(BIN_DIR)/$(PROG)
	./$(BIN_DIR)/$(PROG)

clean:
	$(RM) $(BIN_DIR)/$(PROG) $(OBJECTS) $(DEPS)



#
#==================== pattern rules ====================
#


%.o: %.cpp
	$(CC) $(CXXFLAGS) ${INCLUDES} $< -o $@

%.d: %.cpp
	$(CC) -M $(CXXFLAGS) ${INCLUDES} $< | sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' > $@



-include $(DEPS)

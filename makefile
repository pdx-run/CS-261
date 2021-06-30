C = gcc
CFLAGS = -std=c99
CFLAGS += -Wall
#CXXFLAGS += -pedantic-errors
CFLAGS += -g
CFLAGS += -lm
#LDFLAGS = -lboost_date_time

OBJS = main.o triangle.o 

SRCS = main.c triangle.c 

HEADERS = triangle.h

#target:  dependencies
#  rule to build

output: ${OBJS} ${HEADERS}
	${CXX} ${LDFLAGS} ${OBJS} -o output

${OBJS}: ${SRCS}
	${CXX} ${CXXFLAGS} -c $(@:.o=.c)

clean:  
	rm *.o output

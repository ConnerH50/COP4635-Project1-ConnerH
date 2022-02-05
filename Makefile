CXX = g++
CXXFLAGS = -std=c++11 -g -Wall -pthread #makefile for threads 


OBJECTS = httpServer.o

server:  $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	$(RM) *.o main server



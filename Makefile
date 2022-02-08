CXX = g++
CXXFLAGS = -std=c++11 -g -Wall -pthread #makefile for threads 


OBJECTS = httpServer.o

httpServer:  $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	$(RM) *.o main httpServer



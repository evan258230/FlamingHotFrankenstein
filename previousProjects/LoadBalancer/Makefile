# Compiler settings
CXX = g++
CXXFLAGS = -Wall -std=c++11
TARGET = loadbalancer_sim


SRCS = main.cpp LoadBalancer.cpp WebServer.cpp RequestQueue.cpp Request.cpp

OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

docs:
	doxygen Doxyfile

clean:
	rm -f $(OBJS) $(TARGET)
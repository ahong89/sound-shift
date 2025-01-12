# Variables
CXX = g++
CXXFLAGS = -Wall
TARGET = main
TARGET_DEL = main
SRCS = src/main.cpp
OBJS = $(SRCS:.cpp=.o)
LIBS = rubberband
INCLUDE_PATH = ./include

# Rules
all: $(TARGET) run

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) 

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ -l$(LIBS) -I$(INCLUDE_PATH)

run: $(TARGET)
	./$(TARGET)

clean:
	del $(TARGET_DEL) $(OBJS)

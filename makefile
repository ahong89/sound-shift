# Variables
CXX = g++
CXXFLAGS = -Wall
TARGET = main
TARGET_DEL = main
SRCS = src/parser.cpp src/processor.cpp src/main.cpp
OBJS = $(SRCS:.cpp=.o)
LIBS = rubberband
INCLUDE_PATH = ./include
LIB_PATH = ./lib

# Rules
all: $(TARGET) run

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) -L$(LIB_PATH) -l$(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ -I$(INCLUDE_PATH)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET_DEL) $(OBJS)

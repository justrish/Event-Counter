# Indicates compilator to use
CC      = gcc
CXX 	= g++

# Specifies compilator options
CFLAGS  = -O3 -ULINUX

# Name of the main program
TARGET  = bbst

OBJS  = EventCounter.o 

all: $(TARGET) 

# Compilation and link
$(TARGET): $(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) 

%.o: %.cpp
	$(CXX)  $(CFLAGS) -c $< -o $@

clean:
	-rm -f $(TARGET)
	-rm -f *.o

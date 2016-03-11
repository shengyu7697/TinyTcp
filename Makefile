TARGET = ts

CXX = g++
CXXFLAGS = -g -O0 -Wall -Wno-unused-result
LDFLAGS =
RM = rm -rf
SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)

LIBS =
ifneq ($(LIBS),)
CXXFLAGS += $(shell pkg-config --cflags $(LIBS))
LDFLAGS += $(shell pkg-config --libs $(LIBS))
endif

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) -o $@ $<

clean:
	$(RM) *.o $(TARGET)


CXX = g++
CXXFLAGS = -Wall -Wextra

ifdef DEBUG
	CXXFLAGS += -g
endif

ifdef RELEASE
	CXXFLAGS += -o3
endif

SRCS = main.cpp todo.cpp fileio.cpp terminal_fancy.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = todoapp

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS) $(TARGET) colortest


colortest: colortest.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

.PHONY: clean

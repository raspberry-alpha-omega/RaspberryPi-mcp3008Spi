CPP_FILES := $(wildcard src/*.cpp)
OBJ_FILES := $(addprefix obj/,$(notdir $(CPP_FILES:.cpp=.o)))
CC_FLAGS := -Wall

test: $(OBJ_FILES)
	g++ $(LD_FLAGS) -o $@ $^

obj/%.o: src/%.cpp
	g++ $(CC_FLAGS) -c -o $@ $<

src/spi.cpp : src/spi.h
src/spi_lcd_test.cpp : src/spi.h
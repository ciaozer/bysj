SRC = $(wildcard ./src/*.cpp)
OBJ = $(patsubst ./src/%.cpp, ./obj/%.o, $(SRC))

CC = g++
TARGET = main
INC_PATH = ./include/
CFLAGS = -I

$(TARGET): $(OBJ)
	$(CC) $^ -o $@

$(OBJ): ./obj/%.o: ./src/%.cpp
	$(CC) -c $< -o $@ $(CFLAGS) $(INC_PATH)

.PHONY:
clean:
	-rm -rf main
	-rm -rf ./obj/*.o
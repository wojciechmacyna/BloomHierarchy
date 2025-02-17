# DIRECTORIES
SRC_DIR := ./bloom_hierarchy
INC_DIR := ./bloom_hierarchy
OUT_DIR := ./out


# COMPILER, -fno-rtti for levelbd::Comparator
CC := g++ -g -Wall
C_FLAGS := -Wall -Wextra -fno-rtti -std=c++17

#LIBRARIES
LIBS := -lleveldb -lpthread  
INCLUDE := -I ./leveldb/include/
LIB_PATH := -L ./leveldb/build/

APP_NAME := ./main.out

SRCS := $(wildcard $(SRC_DIR)/*.cpp) 
HDRS := $(wildcard $(INC_DIR)/*.hpp)

all: mybloom
mybloom : main.cpp
	$(CC) main.cpp $(SRC_DIR)/dbOperation.cpp $(SRC_DIR)/dbDumper.cpp $(SRC_DIR)/dbThreadPool.cpp $(SRC_DIR)/bloom_value.cpp $(SRC_DIR)/node.cpp $(SRC_DIR)/bloomTree.cpp $(C_FLAGS) $(LIBS) -o $(OUT_DIR)/main $(INCLUDE) $(LIB_PATH)

clean:
	rm -f $(OUT_DIR)/*
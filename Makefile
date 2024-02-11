# DIRECTORIES
SRC_DIR := ./bloom_hierarchy
OUT_DIR := ./out

# COMPILER, -fno-rtti for levelbd::Comparator
CC := g++
C_FLAGS := -Wall -Wextra -fno-rtti

#LIBRARIES
LIBS := -lleveldb -lpthread  
INCLUDE := -I ./leveldb/include/
LIB_PATH := -L ./leveldb/build/


main : $(SRC_DIR)/bloom.cpp
	$(CC) $(SRC_DIR)/bloom.cpp $(C_FLAGS) $(LIBS) -o $(OUT_DIR)/main $(INCLUDE) $(LIB_PATH)


clean:
	rm -f $(OUT_DIR)/*
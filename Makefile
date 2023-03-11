CC=clang++
CC_OPTS=-pthread -std=c++11 -Wall -Wextra -g

OBJ=main.o arg_parser.o file_info_key.o file_list.o duplicate_finder.o ui.o
DEDUP_LIBS=-lcrypto -lncurses

TARGET=deduplicate

all: $(TARGET)

main.o: main.cc
	$(CC) $(CC_OPTS) -c $< -o $@

arg_parser.o: arg_parser.cc
	$(CC) $(CC_OPTS) -c $< -o $@

file_info_key.o: file_info_key.cc
	$(CC) $(CC_OPTS) -c $< -o $@

file_list.o: file_list.cc
	$(CC) $(CC_OPTS) -c $< -o $@

duplicate_finder.o: duplicate_finder.cc
	$(CC) $(CC_OPTS) -c $< -o $@

ui.o: ui.cc
	$(CC) $(CC_OPTS) -c $< -o $@

$(TARGET): $(OBJ)
	$(CC) $(CC_OPTS) $(OBJ) $(DEDUP_LIBS) -o $@

.PHONY: clean

clean:
	- rm $(OBJ) $(TARGET)

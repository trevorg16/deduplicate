CC=g++
CC_OPTS=-pthread -std=c++11 -Wall -Wextra -g

OBJ=main.o arg_parser.o file_info_key.o file_list.o duplicate_finder.o ui.o ui_loading.o ui_data.o
DEDUP_LIBS=-lcrypto -lncurses

TARGET=deduplicate

all: $(TARGET)

main.o: main.cc arg_parser.h duplicate_finder.h file_list.h ui.h
	$(CC) $(CC_OPTS) -c $< -o $@

arg_parser.o: arg_parser.cc arg_parser.h
	$(CC) $(CC_OPTS) -c $< -o $@

file_info_key.o: file_info_key.cc file_info_key.h
	$(CC) $(CC_OPTS) -c $< -o $@

file_list.o: file_list.cc file_list.h file_info_key.h
	$(CC) $(CC_OPTS) -c $< -o $@

duplicate_finder.o: duplicate_finder.cc duplicate_finder.h ui.h file_list.h
	$(CC) $(CC_OPTS) -c $< -o $@

ui.o: ui.cc ui.h duplicate_finder.h ui_loading.h ui_data.h
	$(CC) $(CC_OPTS) -c $< -o $@

ui_loading.o: ui_loading.cc ui_loading.h
	$(CC) $(CC_OPTS) -c $< -o $@

ui_data.o: ui_data.cc ui_data.h duplicate_finder.h
	$(CC) $(CC_OPTS) -c $< -o $@

$(TARGET): $(OBJ)
	$(CC) $(CC_OPTS) $(OBJ) $(DEDUP_LIBS) -o $@

.PHONY: clean

clean:
	- rm $(OBJ) $(TARGET)

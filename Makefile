CC = g++

ESM_SOFT=/home/huangxm/zwork/esm-dev/
INCLUDE_DIR =./include
SYS_INCLUDE =$(ESM_SOFT)/include/
SOURCE_DIR = . 
LIB_DIR= $(ESM_SOFT)/lib/
BIN_DIR=./bin
LIB_INCLUDE_FLAGS=$(addprefix -L,$(LIB_DIR)) 
INCLUDE_FLAGS = $(addprefix -I, $(INCLUDE_DIR) $(SYS_INCLUDE))
LIB_FLAGS= -lnetcdf  -lpnetcdf  -lhdf5_hl -lhdf5  -lz -lcurl -lm
OBJECT_DIR = .object
SERVER_DIR = server

CLEAN_DIR=$(addsuffix /*~,$(SOURCE_DIR) $(INCLUDE_DIR))
CLEAN_OBJECT=$(addsuffix /*.o, $(SOURCE_DIR))

OBJECT_DIRS=$(OBJECT_DIR) $(addprefix $(OBJECT_DIR)/,$(SOURCE_DIR)) 

DEBUG = -DDEBUG=1
CFLAGS = $(INCLUDE_FLAGS) -Wall $(DEBUG) -O2 $(LIB_INCLUDE_FLAGS) 

HEADERS = $(shell ls $(INCLUDE_DIR) | grep '\.h$$')
ls:
	echo $(HEADERS)

SOURCE_FILES = $(addsuffix /*.c ,$(SOURCE_DIR))

GREPCMD = grep -L 'int main\s*(.*)'  $(SOURCE_FILES)
SOURCE_NAMES = $(shell $(GREPCMD) )
SOURCE_OBJECTS = $(patsubst %.c, %.o, $(SOURCE_NAMES))

GREPCMDL = grep -l 'int main\s*(.*)' $(SOURCE_FILES)
PROGRAM_FILES = $(shell $(GREPCMDL))
PROGRAM_TARGET = $(patsubst %.c, %bin, $(PROGRAM_FILES))


ALL_OBJECTS = $(SOURCE_OBJECTS) $(PROGRAM_TARGET) 

all: init $(ALL_OBJECTS)

%.o: %.c $(HEADERS)
	$(CC) -c $(CFLAGS) $^ -fpic -o $@ 
%bin: %.c $(SOURCE_OBJECTS)
	$(CC)  $^ -o $@ $(CFLAGS) $(LIB_FLAGS) 
	cp $@ $(BIN_DIR)

.PHONY:all clean clear dist_clean  init
init:
	mkdir -p bin
clean: 
	rm -rf $(CLEAN_DIR)	
	rm -rf $(CLEAN_OBJECT)
	rm -rf $(PROGRAM_TARGET)
	rm -rf *~
clear: clean


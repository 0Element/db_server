CC = g++
FLAGS = -g -Wall -std=c++11
LIBS = -pthread
INCLUDE = -Iinclude -I../include -I../../include

OBJ_PATH = build
LIB_PATH = libs
INN_LIBS = -l server -l nettools
APP_SRV = target/srv

export CC FLAGS LIBS INCLUDE


all: check_path nettools server main
	$(CC) $(FLAGS) $(INCLUDE) -o $(APP_SRV) $(OBJ_PATH)/*.o -L $(LIB_PATH)/ $(INN_LIBS) $(LIBS)

main:
	$(CC) $(FLAGS) $(INCLUDE) -c -o $(OBJ_PATH)/main.o src/main.cpp

server:
	cd src/server; make PKG_NAME=libserver.a

nettools:
	cd src/nettools; make PKG_NAME=libnettools.a

check_path:
	test -d target || mkdir target
	test -d build || mkdir build
	test -d libs || mkdir libs


run:
	$(APP_SRV)

clean:
	rm -f $(APP) $(OBJ_PATH)/* $(LIB_PATH)/*
	cd src/server; make clean PKG_NAME=libserver.a
	cd src/nettools; make clean PKG_NAME=libnettools.a
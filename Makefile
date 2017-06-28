CC = g++
FLAGS = -g -Wall -std=c++11
LIBS = -pthread
INCLUDE = -Iinclude -I../include -I../../include

OBJ_PATH = build
LIB_PATH = libs
INNER_LIBS = -l server -l nettools -l utils -l plugins
APP_SRV = target/srv

export CC FLAGS LIBS INCLUDE


all: check_path plugins utils nettools server main
	$(CC) $(FLAGS) $(INCLUDE) -o $(APP_SRV) $(OBJ_PATH)/*.o -L $(LIB_PATH)/ $(INNER_LIBS) $(LIBS)

main:
	$(CC) $(FLAGS) $(INCLUDE) -c -o $(OBJ_PATH)/main.o src/main.cpp

server:
	cd src/server; make PKG_NAME=libserver.a

nettools:
	cd src/nettools; make PKG_NAME=libnettools.a

utils:
	cd src/utils; make PKG_NAME=libutils.a

plugins:
	cd src/plugins; make PKG_NAME=libplugins.a


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
	cd src/utils; make clean PKG_NAME=libutils.a
	cd src/plugins; make clean PKG_NAME=libplugins.a
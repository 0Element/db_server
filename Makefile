CC = g++
FLAGS = -g -Wall -std=c++11
LIBS = -pthread
INCLUDE = -Iinclude -I../include -I../../include -I../../../include

OBJ_PATH = build
LIB_PATH = libs
INNER_LIBS = -lserver -lnettools -lproto -lutils -lfile_cl -lpostgres_cl
OUT_LIBS = -lpq
APP_SRV = target/srv

export CC FLAGS LIBS INCLUDE


all: check_path plugins utils nettools server main proto
	$(CC) $(FLAGS) $(INCLUDE) -o $(APP_SRV) $(OBJ_PATH)/*.o -L $(LIB_PATH)/ $(INNER_LIBS) $(OUT_LIBS) $(LIBS)

main:
	$(CC) $(FLAGS) $(INCLUDE) -c -o $(OBJ_PATH)/main.o src/main.cpp

server:
	cd src/server; make PKG_NAME=libserver.a

nettools:
	cd src/nettools; make PKG_NAME=libnettools.a

utils:
	cd src/utils; make PKG_NAME=libutils.a

proto:
	cd src/proto; make PKG_NAME=libproto.a

plugins: postgres_cl file_cl

postgres_cl:
	cd src/plugins/postgres_cl; make PKG_NAME=libpostgres_cl.a

file_cl:
	cd src/plugins/file_cl; make PKG_NAME=libfile_cl.a


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
	cd src/proto; make clean PKG_NAME=libproto.a
	cd src/plugins/postgres_cl; make clean PKG_NAME=libpostgres_cl.a
	cd src/plugins/file_cl; make clean PKG_NAME=libfile_cl.a
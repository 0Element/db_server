CC=g++
STD=-std=c++11
FLAGS=-g -Wall -pthread
LIBS=-lleveldb -lsnappy
APP=ldb

all:
	$(CC) $(STD) $(FLAGS) -o $(APP) src/*.cpp $(LIBS)

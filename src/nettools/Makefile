OBJ_PATH = ../../build
LIB_PATH = ../../libs

BUILDDIR:=build
sources:=$(wildcard *.cpp)
objects:=$(addprefix $(BUILDDIR)/, $(patsubst %.cpp, %.o, $(sources)))


$(BUILDDIR)/%.o: %.cpp
	$(CC) $(FLAGS) $(INCLUDE) -c $^ -o $@ $(LIBS)

all: check_path $(objects) $(PKG_NAME)

check_path:
	test -d build || mkdir build

$(PKG_NAME):
	ar -cr $(LIB_PATH)/$(PKG_NAME) $(objects)

clean:
	rm -r -f $(BUILDDIR)/*
	rm -f $(OBJ_PATH)/$(PKG_NAME)

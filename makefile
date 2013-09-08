UNAME := $(shell uname -s)

ifeq ($(UNAME), Darwin) # Mac OS
	LDLIBS_GLUT = -framework OpenGL -framework glut
else
	LDLIBS_GLUT = -lGL -lGLU -lglut
endif

all: blesship

OBJ = main.o blesship.o wrapfunc.o objects/project.o objects/psitexture.o

blesship: CC = g++
blesship: $(OBJ)
blesship: LDLIBS = $(LDLIBS_GLUT)

main.o: main.cpp blesship.h
blesship.o: blesship.cpp blesship.h defines.h wrapfunc.h objects/project.h objects/psitexture.h
wrapfunc.o: wrapfunc.cpp wrapfunc.h blesship.h
objects/project.o: objects/project.cpp objects/project.h defines.h
objects/psitexture.o: objects/psitexture.cpp objects/psitexture.h

clean:
	rm -rf $(OBJ)

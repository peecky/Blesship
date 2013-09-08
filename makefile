UNAME := $(shell uname -s)

ifeq ($(UNAME), Darwin) # Mac OS
	LDFLAGS_GLUT = -framework OpenGL -framework glut
else
	LDFLAGS_GLUT = -lGL -lGLU -lglut
endif

all: blesship

OBJ = main.o blesship.o wrapfunc.o objects/project.o objects/psitexture.o

blesship: CC = g++
blesship: $(OBJ)
blesship: LDFLAGS = $(LDFLAGS_GLUT)

main.o: main.cpp blesship.h
blesship.o: blesship.cpp blesship.h defines.h wrapfunc.h objects/project.h objects/psitexture.h
wrapfunc.o: wrapfunc.cpp wrapfunc.h blesship.h
objects/project.o: objects/project.cpp objects/project.h defines.h
objects/psitexture.o: objects/psitexture.cpp objects/psitexture.h

clean:
	rm -rf $(OBJ)

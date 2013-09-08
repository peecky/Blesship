OBJ = main.o blesship.o wrapfunc.o project.o psitexture.o


blesship : $(OBJ)
	g++ -o blesship $(OBJ) -lglut
main.o : main.cpp blesship.h
	g++ -c main.cpp
blesship.o : blesship.cpp blesship.h defines.h wrapfunc.h ./objects/project.h ./objects/psitexture.h
	g++ -c blesship.cpp
wrapfunc.o : wrapfunc.cpp wrapfunc.h blesship.h
	g++ -c wrapfunc.cpp
project.o : ./objects/project.cpp ./objects/project.h defines.h
	g++ -c ./objects/project.cpp -lm
psitexture.o : ./objects/psitexture.cpp ./objects/psitexture.h
	g++ -c ./objects/psitexture.cpp

clean :
	rm -rf $(OBJ)

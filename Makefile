# Project: cs140 assembler
# Instructor: John Magee
# Students: Han Choi & Enbang Wu
#To run, put this file together with assembler.c
#in the same directory. Run "make". Then the executable
#is "assembler" .

assembler: assemblerHelper.o map.o pass.o assembler.o
	gcc -o assembler assemblerHelper.o map.o pass.o assembler.o

assembler.o: assembler.c 
	gcc -c assembler.c 
	
assemblerHelper.o: assemblerHelper.c
	gcc -c assemblerHelper.c

map.o: map.c
	gcc -c map.c

pass.o: pass.c
	gcc -c pass.c

clean:
	rm -f *~ *.o *.a *.exe



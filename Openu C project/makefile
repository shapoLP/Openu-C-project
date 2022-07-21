assembler: assembler_main.o assembler1.o assembler2.o assembler.h
     gcc: -g  assembler_main.o assembler1.o assembler2.o -o assembler -lm

assembler_main.o:assembler_main.c assembler.h
     gcc: -c -ansi -Wall -pedantic assembler_main.c -o assembler_main.o

assembler1.o:assembler1.c assembler.h
     gcc: -c -ansi -Wall -pedantic assembler1.c -o assembler1.o
     
assembler2.o:assembler2.c assembler.h
     gcc: -c -ansi -Wall -pedantic assembler2.c -o assembler2.o

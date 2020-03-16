OBJS	=	main.o	list_lib.o	data_io.o   hashTable.o redBlackTree.o
SOURCE	=	main.c	list_lib.c	data_io.c   hashTable.c redBlackTree.c
HEADER	=	list_lib.h	data_io.h   hashTable.h redBlackTree.h structs.h
OUT	=	myexe
CC	=	gcc
FLAGS   =	-Wall	-g	-c	-std=c99

$(OUT):	$(OBJS)
	$(CC)	-g	$(OBJS)	-o	$@

main.o: main.c
	$(CC)	$(FLAGS)	main.c

list_lib.o:   list_lib.c
	$(CC)	$(FLAGS)	list_lib.c

data_io.o:    data_io.c
	$(CC)	$(FLAGS)	data_io.c

hashTable.o:    hashTable.c
	$(CC)	$(FLAGS)	hashTable.c

redBlackTree.o:    redBlackTree.c
	$(CC)	$(FLAGS)	redBlackTree.c

clean:
	rm	-f	$(OBJS)	$(OUT)

count:
	wc	$(SOURCE)	$(HEADER)
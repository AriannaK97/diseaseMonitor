OBJS	=	main.o	list_lib.o	data_io.o   hashTable.o
SOURCE	=	main.c	list_lib.c	data_io.c   hashTable.c
HEADER	=	list_lib.h	data_io.h   hashTable.h
OUT	=	myexe
CC	=	gcc
FLAGS   =	-Wall	-g	-c	-std=c99

$(OUT):	$(OBJS)
	$(CC)	-g	$(OBJS)	-o	$@

main.o: main.c
	$(CC)	$(FLAGS)	main.c

myList.o:   list_lib.c
	$(CC)	$(FLAGS)	myList.c

myStructs.o:    data_io.c
	$(CC)	$(FLAGS)	myStructs.c

myStructs.o:    hashTable.c
	$(CC)	$(FLAGS)	hashTable.c

clean:
	rm	-f	$(OBJS)	$(OUT)

count:
	wc	$(SOURCE)	$(HEADER)
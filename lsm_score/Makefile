ALL : lsmet_scoring lsmet clean_o
CC = g++
CFLAG = -I.

lsmet: lsmet.o
	${CC} lsmet.o -o lsmet ${CFLAG}

lsmet_scoring: lsmet_scoring.o
	${CC} lsmet_scoring.o -o lsmet_scoring ${CFLAG}

.PHONY: all clean
clean:
	rm -f *.o lsmet lsmet_scoring
clean_o:
	rm -f *.o 
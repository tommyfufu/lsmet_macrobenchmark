ALL : lsmet_lsm_score LSMET clean
CC = g++
CFLAG = -I.

LSMET: LSMET.o
	${CC} LSMET.o -o LSMET ${CFLAG}

lsmet_lsm_score: lsmet_lsm_score.o
	${CC} lsmet_lsm_score.o -o lsmet_lsm_score ${CFLAG}

.PHONY: all clean
clean:
	rm -f *.o *.exe

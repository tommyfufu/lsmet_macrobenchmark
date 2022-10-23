ALL : lsfet_lsm_score LSFET clean
CC = g++
CFLAG = -I.

LSFET: LSFET.o
	${CC} LSFET.o -o LSFET ${CFLAG}

lsfet_lsm_score: lsfet_lsm_score.o
	${CC} lsfet_lsm_score.o -o lsfet_lsm_score ${CFLAG}

.PHONY: all clean
clean:
	rm -f *.o *.exe

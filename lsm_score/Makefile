ALL:lsfet_lsm_score clean
CC = g++
CFLAG = -I.
.PHONY: all clean

lsfet_lsm_score: lsfet_lsm_score.o
	${CC} lsfet_lsm_score.o -o lsfet_lsm_score ${CFLAG}
clean:
	rm -f *.o *.exe

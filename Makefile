.PHONY: clean

CC = $(CXX)

darts: darts.o score_solver.o segment.o

darts.o: darts.cc segment.h score_solver.h

score_solver.o: score_solver.cc score_solver.h segment.h

segment.o: segment.cc segment.h

clean:
	rm -f darts *.o

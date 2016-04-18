EXE = write.x read.x
CC = mpicc
BENCH_FLAGS = '--oar'

all: $(EXE)

FLAGS = -O3 -Wall -Wextra

%.o: %.c tool.h
	$(CC) -c $(FLAGS) $<

read.x: read.o tool.o
	$(CC) $(FLAGS) -o $@ $^ -lparabench

write.x: write.o tool.o
	$(CC) $(FLAGS) -o $@ $^ -lparabench

bench: $(EXE)
	parabench bench $(BENCH_FLAGS) -s 10000 -o csv/ write.x
	parabench bench $(BENCH_FLAGS) -s 10000 -o csv/ read.x

plot:
	parabench plot csv/times-write-*.csv -o times-write.png
	parabench plot csv/times-read-*.csv -o times-read.png

clean:
	rm -rf *.o $(EXE) data.bin csv/ times*.png

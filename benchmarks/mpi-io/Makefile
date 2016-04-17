EXE = write.x read.x
CC = mpicc

all: $(EXE)

FLAGS = -O3 -Wall -Wextra

%.o: %.c tool.h
	$(CC) -c $(FLAGS) $<

read.x: read.o tool.o
	$(CC) $(FLAGS) -o $@ $^ -lparabench

write.x: write.o tool.o
	$(CC) $(FLAGS) -o $@ $^ -lparabench

bench: $(EXE)
	parabench bench -s 10000 -o csv/ write.x
	parabench bench -s 10000 -o csv/ read.x
	parabench plot csv/times-write-*.csv -o times-write.png
	parabench plot csv/times-read-*.csv -o times-read.png

clean:
	rm -rf *.o $(EXE) data.bin csv/ times*.png

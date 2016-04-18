EXE = sleep.x
CC = mpicc

all: $(EXE)

FLAGS = -g -O3 -Wall -Wextra

%.o: %.c
	$(CC) $(FLAGS) -c $<

$(EXE): sleep.o
	$(CC) $(FLAGS) -o $@ $^ -lparabench

bench: $(EXE)
	parabench bench -s 1000 -o csv/ $(EXE)
	parabench plot csv/*.csv

oar: $(EXE)
	parabench bench --oar -s 1000 -o csv/ $(EXE)

plot:
	parabench plot csv/*.csv

clean:
	rm -rf $(EXE) *.o csv/ times.png

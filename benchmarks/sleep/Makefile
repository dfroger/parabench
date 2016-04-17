EXE = sleep.x
INCLUDES = -I$(CONDA_ENV_PATH)/include
LDFLAGS = -L$(CONDA_ENV_PATH)/lib -lparabench

%.o: %.c
	gcc -g -c $< $(INCLUDES)

$(EXE): sleep.o
	mpicc -g -o $@ $^ $(LDFLAGS) 

bench: $(EXE)
	parabench bench -s 1000 -o csv $(EXE)
	parabench plot csv/*.csv

clean:
	rm -rf $(EXE) *.o csv/ times.png

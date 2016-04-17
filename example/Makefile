EXE = example.x
INCLUDES = -I$(CONDA_ENV_PATH)/include
LDFLAGS = -L$(CONDA_ENV_PATH)/lib -lparabench

%.o: %.c
	gcc -g -c $< $(INCLUDES)

$(EXE): example.o
	mpicc -g -o $@ $^ $(LDFLAGS) 

clean:
	rm -f $(EXE) *.o time*.txt

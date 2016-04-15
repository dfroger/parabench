EXE = example.x
INCLUDES = -I$(CONDA_ENV_PATH)/include
LDFLAGS = -L$(CONDA_ENV_PATH)/lib -lparabench

%.o: %.c
	gcc -c $< $(INCLUDES)

$(EXE): example.o
	mpicc -o $@ $^ $(LDFLAGS) 

clean:
	rm -f $(EXE) *.o time*.txt

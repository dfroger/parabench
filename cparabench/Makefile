INCLUDES = $(PREFIX)/include

%.o: %.c
	gcc -g -I$(INCLUDES) -c $<

libparabench.a: parabench.o
	ar rcs $@ $<

install:libparabench.a parabench.h
	mkdir -p $(PREFIX)/include
	mkdir -p $(PREFIX)/lib
	cp parabench.h $(PREFIX)/include
	cp libparabench.a $(PREFIX)/lib

clean:
	rm -f $(EXE) *.o *.a

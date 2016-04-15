%.o: %.c
	gcc -c $<

libparabench.a: parabench.o
	ar rcs $@ $<

install:
	mkdir -p $(PREFIX)/include
	mkdir -p $(PREFIX)/lib
	cp parabench.h $(PREFIX)/include
	cp libparabench.a $(PREFIX)/lib

clean:
	rm -f $(EXE) *.o *.a

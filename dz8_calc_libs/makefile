static: main.o libcalc.a
	gcc -o main main.o -L. -lcalc

dinamic: main.o libcalc.so
	gcc -o main main.o -L. -lcalc -Wl,-rpath,.

main.o: main.c
	gcc -c main.c

libcalc.a: calc_plus.o calc_minus.o calc_mult.o calc_delen.o
	ar cr libcalc.a calc_plus.o calc_minus.o calc_mult.o calc_delen.o

libcalc.so: calc_plus.o calc_minus.o calc_mult.o calc_delen.o
	gcc -shared -o libcalc.so calc_plus.o calc_minus.o calc_mult.o calc_delen.o

calc_plus.o: calc_plus.c
	gcc -c calc_plus.c

calc_minus.o: calc_minus.c
	gcc -c calc_minus.c

calc_mult.o: calc_mult.c
	gcc -c calc_mult.c

calc_delen.o: calc_delen.c
	gcc -c calc_delen.c

clean:
	rm -f *.o *.a *.so main

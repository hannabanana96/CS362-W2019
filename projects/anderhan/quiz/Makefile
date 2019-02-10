CFLAGS= -Wall -fpic -coverage -lm -std=c99

all: testme.c
	echo "Test 1: Testing that program reaches its end with random inputs\n" &> test_results.out
	echo "        (without modification)" >> test_results.out
	gcc -o testme testme.c $(CFLAGS)
	./testme 0 >> test_results.out
	gcov -b testme.c >> test_results.out
	echo >> test_results.out
	echo "Test 2: Test that an incorrect state iteration can be caught\n" >> test_results.out
	echo "        (breaking the code)"	
	gcc -o testme testme.c $(CFLAGS)
	./testme 1 >> test_results.out
	gcov -b testme.c >> test_results.out

clean:
	rm -f *.o testme testme.exe *gcov *gcda *gcno *out


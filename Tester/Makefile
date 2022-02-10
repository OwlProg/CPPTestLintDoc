all: build

build: tests
	g++ Tester.o tests.o -o runtest
	@echo "BUILT SUCCESSFULLY, run <make run>"

tests:
	g++ -c Tester.cpp
	g++ -c *.test.cpp -o tests.o

clean:
	rm -rf *.o runtest

run:
	./runtest
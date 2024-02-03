test.out: test.c
	cc test.c -o test.out

test1: test.out
	./test.out 1 & ./test.out 1 & ./test.out 1 & ./test.out 1

test2: test.out
	./test.out 2 140 4 & ./test.out 2 120 4 & ./test.out 2 110 4 & ./test.out 2 150 4

test3: test.out
	./test.out 3 80 2 & ./test.out 3 120 4 & ./test.out 3 130 5

testall: test.out
	./test.out 3 100 & ./test.out 2 150 & ./test.out 1

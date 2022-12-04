run:
	clang -Wall GameOfLife.c -o GameOfLife
	./gameOfLife < input.txt

clean:
	rm -rf GameOfLife

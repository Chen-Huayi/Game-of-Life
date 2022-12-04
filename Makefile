run:
	clang -Wall gameOfLife.c -o gameOfLife
	./gameOfLife < input.txt

clean:
	rm -rf gameOfLife

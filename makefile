build:
	gcc -O3 -Wall -Wextra -std=gnu99 -o life-seq life-seq.c

test: build
	./life-seq

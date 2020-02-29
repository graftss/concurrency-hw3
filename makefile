build-seq:
	gcc -O3 -Wall -Wextra -std=gnu99 -o life-seq life.c timing.c life-seq.c

test-seq: build-seq
	./life-seq

build-conc:
	gcc -pthread -O0 -Wall -Wextra -std=gnu99 -o life-conc timing.c life.c life-conc-redo.c

test-conc: build-conc
	./life-conc

test: test-seq test-conc

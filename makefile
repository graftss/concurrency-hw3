build-seq:
	gcc -O3 -Wall -Wextra -std=gnu99 -o life-seq life.c life-seq.c

test-seq: build-seq
	./life-seq

build-conc:
	gcc -pthread -O3 -Wall -Wextra -std=gnu99 -o life-conc life.c life-conc.c

test-conc: build-conc
	./life-conc

build:
	gcc -O3 -Wall -Wextra -std=gnu99 -o life timing.c life.c life-conc.c life-seq.c main.c

test: build
	./life

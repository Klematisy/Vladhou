build:
	mkdir -p out
	gcc-13 src/*.c -o out/Vladhou -I/usr/local/include/ -L/usr/local/lib/ -lraylib
run:
	./out/Vladhou
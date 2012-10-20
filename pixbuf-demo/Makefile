pixbuf-demo:pixbuf-demo.o
	gcc -o pixbuf-demo  pixbuf-demo.o `pkg-config --libs gtk+-3.0` -lm
pixbuf-demo.o:
	gcc -c -o pixbuf-demo.o  pixbuf-demo.c `pkg-config --cflags gtk+-3.0`

clean:
	rm -f pixbuf-demo pixbuf-demo.o

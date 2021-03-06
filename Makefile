libstub-pixbuf-pixbuf.so:stub-pixbuf.o 
	gcc -shared -pthread -Wl,-z,noexecstack -Wl,-soname,libstub-pixbuf.so -o libstub-pixbuf.so -Wl,--start-group stub-pixbuf.o ../../out/Default/obj.target/libppapi_cpp.a ../../out/Default/obj.target/libppapi_cpp_objects.a -Wl,--end-group  -I /home/bxl/ -g -lcairo -lm -lpthread
#`pkg-config --libs gtk+-3.0` -lm

stub-pixbuf.o:
	gcc -I /home/bxl/ppapi-new -pthread -g -c -o stub-pixbuf.o `pkg-config --cflags cairo ` stub-pixbuf.c

#stub-gtk.o:
#	gcc -I /home/bxl/ppapi-new -pthread -g -c -o stub-gtk.o `pkg-config --cflags cairo` stub-gtk.c
#`pkg-config --cflags gtk+-3.0 ` stub-gtk.c

clean:
	rm stub-pixbuf.o stub-gtk.o libstub-pixbuf.so

run:
	google-chrome --no-sandbox --register-pepper-plugins="/home/bxl/ppapi-new/ppapi/examples/stub-pixbuf/libstub-pixbuf.so;application/x-ppapi-example" --ppapi-out-of-process  file:///home/bxl/ppapi-new/ppapi/examples/stub-pixbuf/example.html 

debug:
	google-chrome --no-sandbox --register-pepper-plugins="/home/bxl/ppapi-new/ppapi/examples/stub-pixbuf/libstub-pixbuf.so;application/x-ppapi-example" --ppapi-out-of-process  file:///home/bxl/ppapi-new/ppapi/examples/stub-pixbuf/example.html --ppapi-plugin-launcher='xterm -title plugin -e gdb --eval-command=run --args' 

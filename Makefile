libstub-pixbuf.so:stub-pixbuf.o
	g++ -shared -pthread -Wl,-z,noexecstack -Wl,-soname,libstub-pixbuf.so -o libstub-pixbuf.so -Wl,--start-group stub-pixbuf.o ../../out/Default/obj.target/libppapi_cpp.a ../../out/Default/obj.target/libppapi_cpp_objects.a -Wl,--end-group  -I /home/bxl/ -g `pkg-config --libs gtk+-3.0` -lm

stub-pixbuf.o:
	g++ -fpermissive  '-DNO_HEAPCHECKER' '-DCHROMIUM_BUILD' '-DENABLE_REMOTING=1' '-DENABLE_GPU=1' '-D__STDC_FORMAT_MACROS' '-DDYNAMIC_ANNOTATIONS_ENABLED=1' '-D_DEBUG' -I /home/bxl/ppapi-new -pthread -Wno-unused-parameter -Wno-missing-field-initializers -D_FILE_OFFSET_BITS=64 -fvisibility=hidden -pipe -fPIC -fno-strict-aliasing -fPIC -fvisibility=hidden -fvisibility=hidden -O0 -fno-rtti -fno-threadsafe-statics -fvisibility-inlines-hidden -MMD -MF stub-pixbuf.o.d.raw -g -c -o stub-pixbuf.o `pkg-config --cflags gtk+-3.0 ` stub-pixbuf.c
	#g++  -fPIC -g -Wall -c stub.cc -o stub.o -I /home/bxl/

clean:
	rm stub-pixbuf.o.d.raw stub-pixbuf.o libstub-pixbuf.so

run:
	google-chrome --no-sandbox --register-pepper-plugins="/home/bxl/ppapi-new/ppapi/examples/stub/libstub-pixbuf.so;application/x-ppapi-example" --ppapi-out-of-process  file:///home/bxl/ppapi-new/ppapi/examples/stub/example.html 
#--ppapi-plugin-launcher='xterm -title plugin -e gdb --eval-command=run --args' 

CC=/usr/bin/gcc

LIBS=-lm -lpthread -lOpenCL -I resources/ 

FLAGS= -Wno-deprecated-declarations  -Wall  -pthread  -O3 -mtune=native	

install:: clean rDaemon

clean::
	rm -f  rDaemon

rDaemon:: *.c *.h
	$(CC) $(FLAGS) -o rDaemon daemon.c common.c network.c storage.c $(LIBS)

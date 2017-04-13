objects = pthreadpool.o server.o transmitmeg.o

SWFM.o:$(objects)
	g++ -o SWFM.o $(objects) -lpthread
pthreadpool.o: pthreadpool.c lib/pthreadpool.h
	g++ -c pthreadpool.c
transmitmeg.o: transmitmeg.c lib/transmitmeg.h
	g++ -c transmitmeg.c
server.o:server.c lib/pthreadpool.h
	g++ -c server.c

.PHONY:clean
clean:
	-rm SWFM.o $(objects)


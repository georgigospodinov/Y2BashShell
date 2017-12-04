ecmds: ecmds.o eredirect.o esp.o piping.o
	gcc ecmds.o eredirect.o esp.o piping.o -o ecmds -g

piping.o: piping.c esp.h eredirect.h
	gcc piping.c -c -o piping.o -g

ecmds.o: ecmds.c esp.h eredirect.h
	gcc ecmds.c -c -o ecmds.o -g

eredirect.o: eredirect.c eredirect.h
	gcc eredirect.c -c -o eredirect.o -g

esp.o: esp.c esp.h
	gcc esp.c -c -o esp.o -g

clean:
	rm -f *.o
	rm -f ecmds
	rm -rf t

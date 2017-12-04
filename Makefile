# Run parallel
runparallelcmds: bin/runparallelcmds.o bin/sp.o bin/runcommons.o
	gcc bin/runparallelcmds.o bin/sp.o bin/runcommons.o -o runparallelcmds -O3

bin/runparallelcmds.o: src/runparallelcmds.c src/runcommons.h
	gcc src/runparallelcmds.c -c -o bin/runparallelcmds.o -O3

# Run consecutive
runcmds: bin/runcmds.o bin/sp.o bin/runcommons.o
	gcc bin/runcmds.o bin/sp.o bin/runcommons.o -o runcmds -O3

bin/runcmds.o: src/runcmds.c src/sp.h src/runcommons.h
	gcc src/runcmds.c -c -o bin/runcmds.o -O3

# Run commons
bin/runcommons.o: src/runcommons.c src/runcommons.h
	gcc src/runcommons.c -c -o bin/runcommons.o -O3

# Shellsplit
shellsplit: bin/shellsplit.o bin/sp.o
	gcc bin/shellsplit.o bin/sp.o -o shellsplit -O3

bin/shellsplit.o: src/shellsplit.c src/sp.h
	gcc src/shellsplit.c -c -o bin/shellsplit.o -O3

# Split and Parse
bin/sp.o: src/sp.c src/sp.h
	gcc src/sp.c -c -o bin/sp.o -O3

# Clean
clean:
	rm -f bin/*
	rm -f shellsplit
	rm -f runcmds
	rm -rf t
	rm -f runparallelcmds
	rm -rf temp-files


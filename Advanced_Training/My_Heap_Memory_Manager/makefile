static: libhmm.a

dynamic: libhmm.so

libhmm.a: hmm.o 
	ar -rs libhmm.a hmm.o 

hmm.o: hmm.c
	gcc -c hmm.c -g

libhmm.so: hmmdyn.o 
	gcc -shared -o libhmm.so hmmdyn.o -g  

hmmdyn.o: hmm.c
	gcc -c -fPIC hmm.c -o hmmdyn.o -g

ls : dynamic
		ulimit -c unlimited
		sudo systemctl restart apport
		LD_PRELOAD=/home/moaz/hardtest/libhmm.so ls
	
clean:
	rm -f hmm.o  hmmdyn.o

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
	
bash:dynamic
	ulimit -c unlimited
	  sudo systemctl restart apport
	  LD_PRELOAD=/home/moaz/hardtest/libhmm.so bash

bash_check:
	 rm -r ./bash_dump
	 apport-unpack /var/crash/_usr_bin_bash.1000.crash ./bash_dump
	 gdb bash ./bash_dump/CoreDump

clean:
	rm -f hmm.o  hmmdyn.o

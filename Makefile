memsim: memsim.o fifo.o lru.o
		g++ -o memsim memsim.o

memsim.o: memsim.cpp policies.h dataStructures.h
		g++ -c memsim.cpp

fifo.o: fifo.cpp policies.h dataStructures.h
		g++ -c fifo.cpp

lru.o: lru.cpp policies.h dataStructures.h
		g++ -c lru.cpp


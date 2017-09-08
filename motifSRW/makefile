motifMiner-srw.out :  StringTokenizer.o random.o randomMiner.cpp  randomMining.h graph.o 
	g++ -O2 -g -o motifMiner-srw.out graph.o StringTokenizer.o  random.o   randomMiner.cpp randomMining.cpp dfs.cpp canonicalcode.cpp 
graph.o	:   graph.h graph.cpp
	g++ -O2 -g -c graph.cpp

StringTokenizer.o : StringTokenizer.cpp StringTokenizer.h
	g++  -O2 -c -g StringTokenizer.cpp StringTokenizer.h
	
random.o: random.cpp random.h
	g++  -O2 -c -g random.cpp random.h



clean:
	rm -f *.o
	rm -f *.out
	rm -f *.gch
	rm -f *.dSYM


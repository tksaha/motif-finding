/*
 * File:   randomMining.h
 * Author: Tanay Kumar Saha
 *
 * Created on October 15, 2012, 7:33 PM
 */

#ifndef RANDOMMINING_H
#define	RANDOMMINING_H

#include <string>
#include<fstream>
#include "graph.h"
#include<iostream>
#include<vector>
#include<string>
#include<bitset>
#include "StringTokenizer.h"
#include "random.h"
#include <sys/time.h>
#include <unistd.h>
#include <boost/lexical_cast.hpp>
#include<algorithm>
using namespace std;
using namespace boost;

//one assumption on node label and edge label exists.

class randommining
{

private:
	std::vector<GraphClassPtr> myGraphSet;
	char *datafile;
	bool isundir;
	int sizeOfSubgrah,supportValue,temp[10],Qsize,fillerlength;
	int graphIndex,gxpopcount,gypopcount;
	bool tempbool[10],ifprint;
	double tempdouble[10],supportx;
	unsigned int noofIteration,noofGraphs;

/* Variables Common to all */
	GraphClassPtr rawGraph;
	myobjectpool *pool;

/* Variable related to random Graph Sampler */
	vector<set<Vertex*> > gxpopulation;
	vector<set<Vertex*> > gypopulation;
	set<Vertex*>mysetTemp;
	set<Vertex*>setIntersectionTemp;
	int dgx,dgy;
	int gTot,nIter,totsup;

/* Variables related to  canonical code stuff */
	std::vector<canonicalcodeStruc*> code;
	std::vector<canonicalcodeStruc*>newlevelcode;
	string Lastestcancode;
	int i,j,k,x,y, i2,j2,i1,j1,status, _lij, rhs_lij;
	bool is_fwd,rhs_is_fwd;
	int iCodeStage,cindex;    
/* Dfs stuffs */
	int time;
	vector<set<Vertex*> >allcomponents;
	set<Vertex*>componentlist;
	graph* dfsGraph;

public:
    	randommining();
    	~randommining();

/*Methods related to Parser */
   	string  makeFillerForString(string mystring);  
    	void printSupportList();
    	void printUsage(char *progname) {
		std::cout<<"Usage: "<<progname<<" -d data-file -i iteration -s sizeOfSubgrah -q (queueSize,0 by default) -undir 1/0"<<endl;
    	}
    	void ProcessInputFile();
    	void parseArgs(int argc, char* argv[]);

/*Methods related to Random Graph Sampler */
	void gettwoVertexGraph();
   	void populateEdges();
    	void getRandomInitialSubgraph();
    	void populateNeighbor(graph* g, int forwhom);
    	bool getNeighborGraph(graph* g); 
	void popNeiAndGetOne(graph *g);
    	void insertIntoHeap();
	void calculateConcentration(); 
    	void printHeap();
    	int  randomMiner();
	void printNeighborList(graph *g, bool gxorgy);

/*Methods corresponding to Canonical Code*/
	int  absmine(int a,int b);	
    	void freeCodex(canonicalcodeStruc* codex);
    	bool generateAnEdge(Vertex* endvertex, canonicalcodeStruc *it);
    	bool comparecodesAndInsert (canonicalcodeStruc *newcode);
    	void getfirstlabelCanonicalCode();
    	void printCanonicalCode() ;
    	void populateNextLevels();
    	void insert(bool how, Vertex* st, Vertex* end, int  edgelabel);
    	void getcanonicalCode();

/*Methods related to DFS algorithm */
	set<int> getCutVertices();
	bool isGraphConnected();
    	void DFSVISIT(Vertex* u);
	void doDFS();

};
#include "randomMining.inl"
		     
#endif	/* RANDOMMINING_H */


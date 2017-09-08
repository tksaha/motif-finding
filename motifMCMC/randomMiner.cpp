//============================================================================
// Name        : Motif Miner
// Author      : Tanay Kumar Saha (tksaha@cs.iupui.edu)
// Version     : 1.0
// Copyright   : Your copyright notice
// Description :
//============================================================================

#include <iostream>

#include "randomMining.h"

int main(int argc, char *argv[]) 
{
		randommining *rm;
		rm=new randommining();
		rm->parseArgs(argc,argv);
		rm->ProcessInputFile();
		rm->randomMiner();
		cout<<"Program successfully terminated"<<endl;
		return 0;
}

inline randommining::randommining()
{
      	sizeOfSubgrah=0;
      	fillerlength=15;
      	noofGraphs=1;
	isundir=1;
      	ifprint=true; 
	noofIteration=0;
      	Qsize=totsup=0;
      	for( i=0;i<10;i++)
	{
		temp[i]=0; 
		tempbool[i]=false;
		tempdouble[i]=0.0;
	}
	pool=new myobjectpool();
	myGraphSet.clear();
	iCodeStage=cindex=0;
	insertHeapTime=0.0;sampleTime=0.0;codeGenTime=0.0;
	microsec=1000000.0;
}
inline randommining:: ~randommining()
{
	myGraphSet.clear();
}

inline void randommining::populateNeighbor(graph* g,  int forwhom)
{
 	if(forwhom){ gypopulation.clear();gypopcount=0;} 
        else {gxpopulation.clear();gxpopcount=0;} 
	
/*if when returning gy, we have populated the neighbor, then just copy gy to gx*/
	if(!forwhom && gypopcount!=0)
	{
		for(vector<set<Vertex*> >::iterator pit=gypopulation.begin();pit!=gypopulation.end();++pit )
		{
			mysetTemp.clear();
			for(set<Vertex*>::iterator vpit=(*pit).begin(); vpit!=(*pit).end(); ++vpit)
			{
				mysetTemp.insert(*vpit);	
			}
			gxpopulation.push_back(mysetTemp);	
			gxpopcount=gxpopcount+mysetTemp.size();	
		}
		return ;	
	}

	
/* first find out the cut vertices */
	dfsGraph=g;
	doDFS();
	set<int> cutvertices=getCutVertices();
	//cout<<cutvertices.size()<<endl;

/* if there is any cut vertice is there then find the connected components of the of the graph and then using set intersection find out the neighbors */ 

        for(ChangeableVertexIterator  itx=g->myverts.begin();itx!=g->myverts.end();++itx)
        {
		  mysetTemp.clear();
                  if((*itx)->list.size()==0) {}
		  else if(!(cutvertices.find((*itx)->label)==cutvertices.end()))
		  {
		  	 /* One of the cut vertices ; find out the connected component and get the set intersection*/ 
			 (*itx)->shouldConsider=false;
			 dfsGraph=g;
			 doDFS();
			 bool firstcomponent=true;
			 /* do intersection and insert into list */
			 for(vector<set<Vertex*> >::iterator compoiter=allcomponents.begin();compoiter!=allcomponents.end();++compoiter)
			 {
			 	setIntersectionTemp.clear();
				for(set<Vertex*> ::iterator comVer=(*compoiter).begin(); comVer!=(*compoiter).end();++comVer)
				{
					for(ConstVertexIterator  ita=(*comVer)->list.begin();ita!=(*comVer)->list.end();++ita)
					{
						if(g->vertexmap.find((*ita)->label)==g->vertexmap.end())
						{
							setIntersectionTemp.insert((*ita));
						}
					}
				}
				if(firstcomponent)
				{
					for(set<Vertex*>::iterator its=setIntersectionTemp.begin();its!=setIntersectionTemp.end();++its)
					{
						mysetTemp.insert(*its);
					}
					firstcomponent=false;
				}
				else {
					set<Vertex*>::iterator its=mysetTemp.begin();
					for(;its!=mysetTemp.end();)
					{
						if( setIntersectionTemp.find(*its) == setIntersectionTemp.end())
						{
							mysetTemp.erase(*its);
							its=mysetTemp.begin();
							continue;
						}
						its++;
					}
				}
				setIntersectionTemp.clear();
			 }
			 (*itx)->shouldConsider=true;

		  }
                  else {
		
		    bool flag=true;
	  
		    if(flag){
			    for(ConstVertexIterator ity=g->myverts.begin();ity!=g->myverts.end();++ity)
			    {
				 if((*itx)->label==(*ity)->label) continue;
				 for(ConstVertexIterator  ita=(*ity)->list.begin();ita!=(*ity)->list.end();++ita)
				 {
					if(g->vertexmap.find((*ita)->label)==g->vertexmap.end())
					{
						mysetTemp.insert((*ita));
					}
				 }
			    }
		    }

                  }
                  if(forwhom){
                         gypopulation.push_back(mysetTemp);
		         gypopcount=gypopcount+mysetTemp.size();

                  }else{
                         gxpopulation.push_back(mysetTemp);
			 gxpopcount=gxpopcount+mysetTemp.size();
                  }
	}
}
inline void randommining::printNeighborList(graph *g, bool gxorgy)
{
	/* true for gx and false for gy */
	ChangeableVertexIterator  itx=g->myverts.begin();
if(gxorgy){
	for(vector<set<Vertex*> >::iterator pit=gxpopulation.begin();pit!=gxpopulation.end();++pit )
	{
		cout<<"<"<<(*itx)->label<<"->";
		++itx;
		for(set<Vertex*>::iterator vpit=(*pit).begin(); vpit!=(*pit).end(); ++vpit)
		{
			cout<<(*vpit)->label<<",";
		}
		cout<<">";
	}
	cout<<endl;
}
else{
	for(vector<set<Vertex*> >::iterator pit=gypopulation.begin();pit!=gypopulation.end();++pit )
	{
		cout<<"<"<<(*itx)->label<<"->";
		++itx;
		for(set<Vertex*>::iterator vpit=(*pit).begin(); vpit!=(*pit).end(); ++vpit)
		{
			cout<<(*vpit)->label<<",";
		}
		cout<<">";
	}
	cout<<endl;
}

}
inline bool randommining::getNeighborGraph(graph* g)
{
	int low=1; int high,rnd;
	Vertex *labelToAdd,*labelToDelete;
       	high=gxpopcount;
	if(high+1 >low){
        	rnd=boost_get_a_random_number(low,high+1);
	}else{
		cout<<"problem in rnd getneighbor"<<endl;
	}
	vector<set<Vertex*> >::iterator itl;
        itl=gxpopulation.begin();
        int count=0;
        bool flag=false;
	ConstVertexIterator cvi= g->myverts.begin();
        for(;  ;++itl)
        {
            if(flag) break;
            if(itl==gxpopulation.end()) break; 
            if(count+(*itl).size()>=rnd)
            {
                for(set<Vertex*>::iterator itp=(*itl).begin();itp!=(*itl).end();++itp)
                {
                        count++;
                        if(count==rnd) {
                                labelToAdd=(*itp);
                                labelToDelete=(*cvi);
				flag=true;
                                break;
                        }
		}
            }
            else count =count+(*itl).size();
	    cvi++;
	}

	rawGraph=pool->getGResource();
/* form the new graph*/
	for(cvi=g->myverts.begin();cvi!=g->myverts.end();++cvi)
	{
		if((*cvi)->label!=labelToDelete->label) {
			
			rawGraph->addVertex((myGraphSet[graphIndex-1]->biggraphvertex[(*cvi)->label]));
		}
	}
	rawGraph->addVertex((myGraphSet[graphIndex-1]->biggraphvertex[(labelToAdd)->label]));


/* keep all the edges that is does hold the deleting vertex*/
	for(ConstEdgeSetIterator cst=g->myedges.begin();cst!=g->myedges.end();++cst)
	{
		if((*cst)->st->label== labelToDelete->label || (*cst)->end->label ==labelToDelete->label)
		{
			continue;
		}
		else {
			rawGraph->addEdge(*(cst));		
		}
	}
/*add all edges corresponding to the new vertex */
	for(ConstVertexIterator ity=rawGraph->myverts.begin(); ity!=rawGraph->myverts.end();++ity)
	{
			if((*ity)->label==labelToAdd->label) continue;
			Edge *E=pool->getEResource();
			E->st=labelToAdd ; E->end=(*ity);
			vector<Edge*>::iterator itE=std::lower_bound(myGraphSet[graphIndex-1]->ev.begin(),myGraphSet[graphIndex-1]->ev.end(),E,compareE());	
			if( itE!=myGraphSet[graphIndex-1]->ev.end())
			{
				if((E->st->label == (*itE)->st->label)&&(E->end->label == (*itE)->end->label))	
					rawGraph->addEdge((*itE));
			}
			pool->returnEResource(E);
			E=pool->getEResource();
			E->st=(*ity) ; E->end=labelToAdd;
			itE=std::lower_bound(myGraphSet[graphIndex-1]->ev.begin(),myGraphSet[graphIndex-1]->ev.end(),E,compareE());	
			if( itE!=myGraphSet[graphIndex-1]->ev.end())
			{
				if((E->st->label == (*itE)->st->label)&&(E->end->label == (*itE)->end->label))	
					rawGraph->addEdge((*itE));
			}
			pool->returnEResource(E);

	}
/*End of populating */

	return true;

}  

inline void randommining::popNeiAndGetOne(graph *g)
{
	double acc_prob, accvalue,rnd;
	populateNeighbor(g, 0);
	
	dgx=gxpopcount;
	if(dgx==0){ 
		g->printAdjacencyList();
		cout<<"Error Populating Neighbor that was lastly sampled"<<endl; 
		exit(0);	
	}
	while(true){
		rawGraph =pool->getGResource();
		getNeighborGraph(g);
		populateNeighbor(rawGraph, 1);
		dgy=gypopcount; //dgy
		if(dgy==0){ 
			cout<<"Error Populating Neighbor of newly formed graph"<<endl; 
			exit(0);	
		} 
		accvalue=dgx/(double)dgy;
		if(accvalue>1.0) acc_prob=1.0;
		else acc_prob=accvalue;
		rnd=random_uni01();
		if(rnd<=acc_prob) break;
		else{
			double st;
			noofIteration=noofIteration-1;
			gettimeofday(&start_time, (struct timezone *)0);
			st = start_time.tv_sec + (start_time.tv_usec/microsec);
			insertIntoHeap();
			gettimeofday(&stop_time, (struct timezone *)0);
			insertHeapTime+=stop_time.tv_sec + (stop_time.tv_usec/microsec) -st;
		}
	}
}
inline int randommining:: randomMiner()
{
	int rnd,cnt;
	noofGraphs=myGraphSet.size();
	bool count[noofGraphs+1],toContinue;
	int low=1; int  high=noofGraphs;
	//cout<<noofGraphs<<endl;

	for(i=1;i<=noofGraphs;i++)
	{
		count[i]=false;
	}
	if(noofGraphs >=1)
	{
		vector<GraphClassPtr>recentGraphs(noofGraphs+1);
		double st;
		size_t i=0;
		string noIter=boost::lexical_cast<string>(noofIteration);
		nIter=noIter.size();
		string totalG=boost::lexical_cast<string>(myGraphSet.size());
		gTot=totalG.size();
//		(*myGraphSet.begin())->printAdjacencyList();
		while(i++<noofIteration+1)
		{
			graphIndex =1;	
			gettimeofday(&start_time, (struct timezone *)0);
			st = start_time.tv_sec + (start_time.tv_usec/microsec);	
			//cout<<"iterating:"<<i<<endl;	
			if(count[graphIndex]==false) {	
				getRandomInitialSubgraph();
				recentGraphs[graphIndex]=rawGraph;
				cout<<"printing sampled graph"<<endl;
	//	                rawGraph->printAdjacencyList();
				count[graphIndex]=true;
			}else{
				if(myGraphSet[graphIndex-1]->myverts.size()==sizeOfSubgrah)
				{
					continue;
				}
				//cout<<"Going to Populate Neighbor"<<endl;
				popNeiAndGetOne(recentGraphs[graphIndex]);
				pool->returnGResource(recentGraphs[graphIndex]);
				recentGraphs[graphIndex]=rawGraph;
				//rawGraph->printAdjacencyList();
				if(rawGraph->myedges.size()<sizeOfSubgrah-1)
				{
					cout<<"Subgraph has not been sampled Properly"<<endl;	
					recentGraphs[graphIndex]->printAdjacencyList();
				}
			}
			gettimeofday(&stop_time, (struct timezone *)0);
			if(i>1) sampleTime+=stop_time.tv_sec + (stop_time.tv_usec/microsec) -st;
			
			/* check to code gen  */
			gettimeofday(&start_time, (struct timezone *)0);
			st = start_time.tv_sec + (start_time.tv_usec/microsec);	
			if(i>1) getcanonicalCode();
			gettimeofday(&stop_time, (struct timezone *)0);
			if(i>1)  codeGenTime+=stop_time.tv_sec + (stop_time.tv_usec/microsec) -st;
			
			/*check to insert*/
			gettimeofday(&start_time, (struct timezone *)0);
			st = start_time.tv_sec + (start_time.tv_usec/microsec);
			if(i>1) insertIntoHeap();
			gettimeofday(&stop_time, (struct timezone *)0);
			if(i>1) insertHeapTime+=stop_time.tv_sec + (stop_time.tv_usec/microsec) -st;
		}
		
	}else{
		cout<<"no graph to Mine"<<endl;
	}
	calculateConcentration();
	printHeap();
	cout<<"Total Sample Time"<<sampleTime<<"  Total code Gen Time:"<<codeGenTime<<"  Total insert Time:"<<insertHeapTime<<endl;
	cout<<"|"<<sampleTime<<"|"<<codeGenTime<<"|"<<insertHeapTime<<"|"<<endl;	
	
	return 1;
}





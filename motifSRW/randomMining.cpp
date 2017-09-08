#include "randomMining.h"
#include "codeset.h"

void randommining::ProcessInputFile()
{
	std::string line;
	ifstream infile;
	infile.open(datafile, ios::in);
	int count=0;
	int st,end,elab,temp;
	GraphClassPtr g;
	g=pool->getGResource();
	long int countline=0;
	set<int>vertices;
	vector<EdgePair>edgelist;
	struct timeval start_time;
	struct timeval stop_time;
	double  microsec= 1000000.0;double stx;
	double filereadtime;
	gettimeofday(&start_time, (struct timezone *)0);
	stx= start_time.tv_sec + (start_time.tv_usec/microsec);	
	while(true)								
	{
//		cout<< "reading line"<<countline++<<endl;
		std::getline(infile,line);
	   	if (line.length() < 1){
			g->numV=g->myverts.size();
			g->numE=g->myedges.size();
			break;
	    	}
	   	StringTokenizer strtok = StringTokenizer(line,"\t");
	   	st=strtok.nextIntToken();
		end=strtok.nextIntToken();
		elab=1;
		vertices.insert(st);
		vertices.insert(end);
		edgelist.push_back(EdgePair(st,end));
	}	
	int nverts=vertices.size();	
	g->biggraphvertex.reserve(nverts+1);
	for(int i=0;i<=nverts;i++)
	{
		Vertex *vx=pool->getVResource();
                vx->label=i;
                vx->globallabel=1;
		g->biggraphvertex[i]=vx;
	}
	//cout<<edgelist.size();

	for(vector<EdgePair>::iterator epair=edgelist.begin(); epair!=edgelist.end();++epair)
	{
	     	Edge *E=pool->getEResource();
				
		E->st=g->biggraphvertex[(*epair).st]; 
		E->end=g->biggraphvertex[(*epair).end]; 
		E->edgelabel=elab;
		g->ev.push_back(E);
		g->updateAdj(g->biggraphvertex[(*epair).st],g->biggraphvertex[(*epair).end],isundir);
	}
	std::sort(g->ev.begin(),g->ev.end(),compareE());
	myGraphSet.push_back(g);
	gettimeofday(&stop_time, (struct timezone *)0);
	filereadtime=stop_time.tv_sec + (stop_time.tv_usec/microsec) -stx;
	cout<<"File reading time: "<<filereadtime<<" sec"<<endl;	
	infile.close();
}
string randommining:: makeFillerForString(string mystring)
{

        for (int len=mystring.length();len<=fillerlength;len++)
        {
               mystring=mystring+" ";
        }
        return mystring;

}

void randommining::parseArgs(int argc, char* argv[]) {
      if(argc<9) {
	      printUsage(argv[0]);
	      exit(0);
      }
      if(((string)(argv[1]))!="-d" || ((string)(argv[3]))!="-i" || ((string) (argv[5]))!="-s" || ((string) (argv[7]))!="-q"|| ((string) (argv[9]))!="-undir"){
	      printUsage(argv[0]);
	      exit(0);
      }
      for ( i=1; i < argc; i++){
		      if ((string)(argv[i])== "-d"){
				      datafile=argv[++i];
				      if(ifprint)  cout<<makeFillerForString("Data File:")<<datafile<<endl;
		      }
		      else if((string)(argv[i])== "-i"){
				      noofIteration=atof(argv[++i]);
				      if(ifprint) cout<<makeFillerForString("noofIteration:")<<noofIteration<<endl;
		      }
		      else if((string)(argv[i])== "-s"){
				      sizeOfSubgrah=atof(argv[++i]);
				      if(ifprint) cout<<makeFillerForString("sizeOfSubgrah:")<<sizeOfSubgrah<<endl;
		      }
		      else if( ((string)(argv[i])).compare("-q")==0){
				      Qsize=boost::lexical_cast<int>(argv[++i]);
		      }
		      else if( ((string)(argv[i])).compare("-undir")==0){
				      int val=boost::lexical_cast<int>(argv[++i]);
				      if(val==1) isundir=true;
				      else if(val==0) isundir=false;
				      else {
				      		cout<<"Unwanted VAlue for Directedness"<<endl;
						exit(0);
				      }
		      }

	}
}
  

void randommining::printHeap()
{
	
    	print_out_by<graphidsx>(mycodeset);   	

}
void randommining::gettwoVertexGraph()
{
		
	temp[0]=1; temp[1]=myGraphSet[graphIndex-1]->ev.size();
	int rnd=boost_get_a_random_number(temp[0], temp[1]+1);
	vector<Edge*>::iterator itx=myGraphSet[graphIndex-1]->ev.begin()+rnd-1;
	rawGraph->addVertex((*itx)->st) ;
	rawGraph->addVertex((*itx)->end);
}
void randommining::populateEdges()
{
	for(ConstVertexIterator itx=rawGraph->myverts.begin();itx!=rawGraph->myverts.end();++itx)
	{
		for(ConstVertexIterator ity=rawGraph->myverts.begin(); ity!=rawGraph->myverts.end();++ity)
		{
			if((*itx)->label==(*ity)->label) continue;
		//	cout<<(*itx)->label<<"  "<<(*ity)->label<<endl;
			/* Edge can be both way around*/
			Edge *E=pool->getEResource();
			E->st=(*itx) ; E->end=(*ity);
			vector<Edge*>::iterator itE=std::lower_bound(myGraphSet[graphIndex-1]->ev.begin(), 
					myGraphSet[graphIndex-1]->ev.end(),E,compareE());	
			if( itE!=myGraphSet[graphIndex-1]->ev.end())
			{
				//cout<<(*itE)->st->label<<"-----"<<(*itE)->end->label<<endl;
				if(((*itx)->label==(*itE)->st->label) && ((*ity)->label== (*itE)->end->label))
				{
					rawGraph->addEdge((*itE));
				}
			}
			pool->returnEResource(E);
		}
	 }
}
void randommining::getRandomInitialSubgraph()
{
	if(sizeOfSubgrah<2) { cout<<"Size must be at least 2"<<endl; exit(1);}
	else {	
		rawGraph =pool->getGResource();
		gettwoVertexGraph();
		tempbool[0]=false;
		
		while(rawGraph->myverts.size()!=sizeOfSubgrah)
		{
			temp[0]=rawGraph->myverts.size();
			for(ConstVertexIterator itx=rawGraph->myverts.begin();itx!=rawGraph->myverts.end();++itx)
			{
				for(ChangeableVertexIterator  itz=(*itx)->list.begin();itz!=(*itx)->list.end();++itz)
				{
					if(rawGraph->vertexmap.find((*itz)->label)==rawGraph->vertexmap.end()) {
							rawGraph->addVertex((*itz));
							itx=rawGraph->myverts.begin();
							itz=(*itx)->list.begin();
					}

					if(rawGraph->myverts.size()==sizeOfSubgrah) {
						tempbool[0]=true;
						break;
					}
				}
				if(tempbool[0]==true) break;
			}
			if(tempbool[0]==true) break;
			if(rawGraph->myverts.size()==temp[0]){
				 rawGraph->reset();
				 gettwoVertexGraph();
			}
		}
		populateEdges(); 
	 }
}
void randommining:: getcanonicalCode(){
	iCodeStage=0;
	newlevelcode.clear();
	code.clear();
	getfirstlabelCanonicalCode();
	if(rawGraph->myedges.size()>1) populateNextLevels();
	printCanonicalCode();
}
int randommining::absmine(int a,int b)
{
	if((a-b)<0) return -(a-b);
	else return (a-b);
}

void randommining::calculateConcentration()
{
	
	typedef code_set::index<codex>::type code_set_by_code;
	code_set_by_code& code_index=mycodeset.get<codex>();

	int totalcount=0;
	double totaldegreecount=0.0;

	for (code_set_by_code::iterator it=code_index.begin();it!=code_index.end();++it)
	{
		totalcount=totalcount+(*it).count;
		totaldegreecount=totaldegreecount+(*it).cumdfrac;	
	}

	for (code_set_by_code::iterator it=code_index.begin();it!=code_index.end();++it)
	{	
		heapData hd=*it;
		hd.countconcentration=hd.count/(double) (totalcount);
		hd.degreeconcentration=hd.cumdfrac/totaldegreecount;
		code_index.replace(it,hd);
	}	

} 
void randommining::insertIntoHeap()
{

	typedef code_set::index<codex>::type code_set_by_code;
	code_set_by_code& code_index=mycodeset.get<codex>();

	code_set_by_code::iterator it=code_index.find(Lastestcancode);
	if(it!=code_index.end())
	{
		heapData hd=*it;
		hd.count=hd.count+1;
		hd.cumdfrac=hd.cumdfrac+(1/(double)dgy);
		code_index.replace(it,hd);
	}
	else {
		mycodeset.insert(heapData(Lastestcancode,1/(double)(dgy)));
	}

} 

#include "graph.h"
#include "randomMining.h"
#include<algorithm>


bool randommining:: comparecodesAndInsert (canonicalcodeStruc *newcode)
{
	std::vector<canonicalcodeStruc*>::const_iterator it=newlevelcode.begin();
	status=0;//0 means equal, 1 means to insert it and delete others, otherwise do nothing
	if(it!=newlevelcode.end())
	{
		std::vector<fiveTuple*>::const_iterator itb=(*it)->codetuples.begin();
		std::vector<fiveTuple*>::const_iterator ita=newcode->codetuples.begin();
		
		if(newcode->codetuples.size()==(*it)->codetuples.size())
		{
			cindex=0;	
			for( ; itb!=(*it)->codetuples.end();++ita,++itb)
			{
				if(iCodeStage>cindex) {cindex++; continue;}
				else cindex++;
			
				i=(*ita)->dfsst; j=(*ita)->dfsend;
				x=(*itb)->dfsst; y=(*itb)->dfsend;
			        _lij, rhs_lij;
                                _lij=((*ita)->edgelabel); rhs_lij=((*itb)->edgelabel);


                                 i2=(*itb)->stlabel;
                                 j2=(*itb)->endlabel;
                                 i1=(*ita)->stlabel;
                                 j1=(*ita)->endlabel;
                                 is_fwd=(i<j);
                                 rhs_is_fwd=(x<y);

                                if(!is_fwd && rhs_is_fwd) {status=1; break;}

                                if(!is_fwd && !rhs_is_fwd && j<y) {status=1; break;}

                                if(!is_fwd && !rhs_is_fwd && j==y && _lij<rhs_lij)
                                        {status=1;break;}

                                if(is_fwd && rhs_is_fwd && i>x)
                                        {status=1;break;}

                                if(is_fwd && rhs_is_fwd && i==x && i1<i2)
                                        {status=1;break;}

                                if(is_fwd && rhs_is_fwd && i==x && i1==i2 && _lij<rhs_lij)
                                        {status=1;break;}

                                if(is_fwd && rhs_is_fwd && i==x && i1==i2 && _lij==rhs_lij && j1<j2)
                                        {status=1;break;}
                                if((i==x) && (j==y)&& (i1==i2) && (j1==j2) && (_lij==rhs_lij))
                                {}
                                else {
                                        status=2; break;
                                        //std::cout<<"Error in edge"<<endl;
                                }	 
			}
		}
		else{
			 cout<<"Error in Size"<<"  I am from canonical code compare function"<<endl;
			 return false;
		}
	}else{
	       // cout<<"no other code exists in new level code, so insert"<<endl;	
		newlevelcode.push_back(newcode);
		return true;		
	}

	if(status==0) {
		//same code so insert
		newlevelcode.push_back(newcode);
	}
	else if(status==1){
		//only keep new code and discard others
		for(std::vector<canonicalcodeStruc*>::iterator itc=newlevelcode.begin(); itc!=newlevelcode.end();++itc)
		{
			freeCodex (*itc);
			
		}
		newlevelcode.clear();
		newlevelcode.push_back(newcode);
	}else if(status==2)
	{
		return false;
	}
	return true;
}
void randommining::freeCodex(canonicalcodeStruc* codex)
{
	 
	for(std::vector<fiveTuple*>::iterator itf=(codex)->codetuples.begin();
			itf!=(codex)->codetuples.end();++itf)
	{
		pool->returnFiveTupleResource((*itf));
	}
	for(std::vector<Edge*>::iterator itE=(codex)->edges.begin();itE!=(codex)->edges.end();++itE)
	{
		pool->returnEResource((*itE));
	}
	pool->returncancodeResource(codex);
	 

}
void randommining::insert(bool how, Vertex* st, Vertex* end, int edgelabel)//0 for st->end and 1 for end->st
{
	canonicalcodeStruc *codex=pool->getcancodeResource();
        codex->maxcode=0;
	Edge *E=pool->getEResource();
	E->edgelabel=edgelabel;
        if (how) E->st=st;
	else E->st=end;
	codex->dfscodemap[E->st->label]=codex->maxcode;
	int dfsst=codex->maxcode;
	codex->maxcode++;
	if(how) E->end=end;
	else E->end=st;
	int dfsend=codex->maxcode;
	codex->dfscodemap[E->end->label]=codex->maxcode++;
	codex->edges.push_back(E);
	fiveTuple* tuple=pool->getFiveTupleResource();
	if(how) {
		 tuple->dfsst=dfsst;tuple->dfsend=dfsend;
		 tuple->stlabel= st->globallabel;
		 tuple->edgelabel=edgelabel;
		 tuple->endlabel=end->globallabel;
	}
	else{
		 tuple->dfsst=dfsst;tuple->dfsend=dfsend;
                 tuple->stlabel= end->globallabel;
                 tuple->edgelabel=edgelabel;
                 tuple->endlabel=st->globallabel;
		 
	}
	codex->codetuples.push_back(tuple);
	if(!comparecodesAndInsert(codex)) {
		freeCodex(codex);	 
	}
	
}
void randommining:: getfirstlabelCanonicalCode()
{
	//we will have as many lavels as the number of edges.
	//if anyone wants to insert a canonical code then it must the same or less than current cannonical code.
	//if smaller canonical code is found then we will discard prvious one and insert present one.
	//for each edgeS
	for(ConstEdgeSetIterator it=rawGraph->myedges.begin();it!=rawGraph->myedges.end();++it)
	{
		insert(true, (*it)->st, (*it)->end, (*it)->edgelabel);
		insert(false,(*it)->st, (*it)->end, (*it)->edgelabel);
	}
 	code.clear();
	for(std::vector<canonicalcodeStruc*>::iterator itc=newlevelcode.begin();itc!=newlevelcode.end();++itc)
	{
		code.push_back(*itc);	
	}		
	newlevelcode.clear();
}

void randommining:: printCanonicalCode()
{
        int count=0;
	Lastestcancode="";
	
	for(std::vector<canonicalcodeStruc*>::const_iterator it=code.begin(); it!=code.end();++it)
	{
		//cout<<"Starting printing canonical code:"<<endl;	
		if(count>0) break; //print only the first one;
                else count++;
		for(std::vector<fiveTuple*>::const_iterator itx=(*it)->codetuples.begin();itx!=(*it)->codetuples.end();++itx)
		{
				
			//cout<<"("<<i2<<"["<<(*itx)->st<<"]"<<","<<j2<<"["<<(*itx)->end<<"]"<<")";
			Lastestcancode+="("+boost::lexical_cast<string>((*itx)->dfsst)+","+boost::lexical_cast<string>((*itx)->dfsend)+","+boost::lexical_cast<string>((*itx)->stlabel)+","+boost::lexical_cast<string>((*itx)->edgelabel)+","
				+boost::lexical_cast<string>((*itx)->endlabel)+")";
		}
		//printing dfs code
		/*cout<<endl<<"Printing DFS code"<<endl;
		for(std::map<int, int>::const_iterator ity=(*it)->dfscodemap.begin();ity!=(*it)->dfscodemap.end();++ity)
		{
			cout<<(*ity).first<<" ->"<<(*ity).second<<endl;	
		}
		cout<<endl<<"End of Printing canonical code"<<endl;*/
	}

	for(std::vector<canonicalcodeStruc*>::iterator itc=code.begin(); itc!=code.end();++itc)
	{
		freeCodex(*itc);
	}
}

bool randommining::generateAnEdge(Vertex* endvertex, canonicalcodeStruc *it)
{
        int count=0;
        for(boost::container::flat_set<Vertex*>::const_iterator itp=endvertex->list.begin();itp!=endvertex->list.end();++itp)
        {
		if(rawGraph->vertexmap.find((*itp)->label) == rawGraph->vertexmap.end()) continue;

                map<int,int>::iterator itd=(it)->dfscodemap.find(endvertex->label);
                map<int,int>::iterator ite=(it)->dfscodemap.find((*itp)->label);
                int flag=1;
                if(itd!=(it)->dfscodemap.end() && ite!=(it)->dfscodemap.end())
                {
                        int first=(*ite).second;
                        int second=(*itd).second;
                        flag=2;
                        if(absmine(first,second)==1){
                                flag=0;
                        }
                        else{
                                for(std::vector<Edge*>::iterator ity=(it)->edges.end()-1;ity!=(it)->edges.begin();--ity)
                                {
                                        if(((*ity)->st->label==endvertex->label && (*ity)->end->label==(*itp)->label) 
							|| ((*ity)->end->label==endvertex->label && (*ity)->st->label
							==(*itp)->label))
                                        {
                                                flag=0; break;
                                        }
                                }
			}
		}
                if(flag==1 || flag==2){
                        count++;
                        canonicalcodeStruc *cs=pool->getcancodeResource();
			for(std::vector<Edge*>::iterator itn=(it)->edges.begin();itn!=(it)->edges.end();++itn)
			{
				Edge *ex=pool->getEResource();
				ex->st=(*itn)->st;ex->end=(*itn)->end;ex->edgelabel=(*itn)->edgelabel;
				cs->edges.push_back(ex);
			}
			for(std::vector<fiveTuple*>::iterator itft=it->codetuples.begin();itft!=it->codetuples.end();
					++itft)
			{
				fiveTuple* five=pool->getFiveTupleResource();
				five->dfsst=(*itft)->dfsst; five->dfsend=(*itft)->dfsend;
				five->stlabel=(*itft)->stlabel; five->endlabel=(*itft)->endlabel;
				five->edgelabel=(*itft)->edgelabel;
				cs->codetuples.push_back(five);
			}
			for(std::map<int,int>::iterator itmapx=it->dfscodemap.begin();itmapx!=it->dfscodemap.end();++itmapx)
			{
				cs->dfscodemap[(*itmapx).first]=(*itmapx).second;
			}
			
                        Edge *e=pool->getEResource();
                        e->st=endvertex;
                        e->end=(*itp);
                        int dfsst, dfsend;
                        dfsst=(*itd).second;
                        string stlabel, endlabel, edgelabel;
			
			Edge *et=pool->getEResource();
			et->st=endvertex;
			et->end=(*itp);

                        boost::container::flat_set<Edge*,compareE>::iterator itE=rawGraph->myedges.find(et);
			if(itE==rawGraph->myedges.end()) {
					et->end=endvertex;
					et->st=(*itp);
					itE=rawGraph->myedges.find(et);
					if(itE==rawGraph->myedges.end())
					{
						cout<<"Error finding edge from canonical code generation"<<endl;
						exit(0);
					}
			}
			pool->returnEResource(et);

			cs->maxcode=(it)->maxcode;
			e->edgelabel=(*itE)->edgelabel;
                        if(flag==1) {
                                 dfsend=cs->maxcode;
                                 cs->dfscodemap[(*itp)->label]=cs->maxcode++;
                        }else{
                                 dfsend=(*ite).second;
                        }
                        cs->edges.push_back(e);
                        fiveTuple *tuple=pool->getFiveTupleResource();
			tuple->dfsst=dfsst; tuple->dfsend=dfsend; tuple->stlabel=e->st->globallabel;
			tuple->endlabel=e->end->globallabel; tuple->edgelabel=e->edgelabel;

                        cs->codetuples.push_back(tuple);
                        if(!comparecodesAndInsert(cs)) {
				 freeCodex(cs);
			}
                }

        }

        if(count) return true;
        else return false;

}


void randommining::populateNextLevels()
{


	for(int i=1;i<rawGraph->myedges.size();i++)
        {
		iCodeStage=i;
                for(std::vector<canonicalcodeStruc*>::iterator it=code.begin(); it!=code.end();++it)
                {
                        std::vector<Edge*>::iterator itx=(*it)->edges.end();
                        bool flag=false;
                        Vertex* endvertex;
                        do{
                                if(itx==(*it)->edges.begin()) break;
                                itx=itx-1;
                                while(true)
                                {

                                        map<int,int>::const_iterator itd=(*it)->dfscodemap.find((*itx)->end->label);
                                        map<int,int>::const_iterator ite=(*it)->dfscodemap.find((*itx)->st->label);
                                        int fwd=(*ite).second;
                                        int bck=(*itd).second;
                                        //cout<<"fwd: "<<fwd<<"bck: "<<bck<<endl;                       
                                        if(bck<fwd) itx=itx-1;
                                        else break;
                                }
                                endvertex=(*itx)->end;
                                if(generateAnEdge(endvertex,(*it)))  flag=true;
                                if(flag==false) {
                                        if(generateAnEdge((*itx)->st , (*it))){flag=true;}
				  }
                        }while(!flag);

                }
                for(std::vector<canonicalcodeStruc*>::iterator itc=code.begin();itc!=code.end();++itc)
                {
                        freeCodex((*itc));

                }
                code.clear();

                for(std::vector<canonicalcodeStruc*>::iterator itc=newlevelcode.begin();itc!=newlevelcode.end();++itc)
                {
                        code.push_back((*itc));

                }
		newlevelcode.clear();
        }
}


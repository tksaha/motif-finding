#include "graph.h"
#include "randomMining.h"

void randommining::doDFS()
{
	//color 0 for white 1 for gray 2 for black parent=-1 for NIL
	allcomponents.clear();
	componentlist.clear();
	time=0;
	for(ChangeableVertexIterator it=dfsGraph->myverts.begin();it!=dfsGraph->myverts.end();++it)
	{
		(*it)->color=0;
		(*it)->parent=-1;
		//cout<<" shldconsider"<<(*it)->shouldConsider<<endl;
	}
	for( ChangeableVertexIterator it=dfsGraph->myverts.begin();it!=dfsGraph->myverts.end();++it)
	{
		if((*it)->shouldConsider==false) continue;
		if((*it)->color==0) {
			if(componentlist.size()==0) {}
			else {
				allcomponents.push_back(componentlist);
				componentlist.clear();
			}
			DFSVISIT((*it));
		}
	}
	allcomponents.push_back(componentlist);
	
}

void randommining::DFSVISIT(Vertex* u)
{
	u->color=1;
	componentlist.insert(u);
	time=time+1;
	u->startTime=time;
	u->lowv=time;
	u->childList.clear();
	for(ChangeableVertexIterator itx=u->list.begin();itx!=u->list.end();++itx)
	{
		if(dfsGraph->vertexmap.find((*itx)->label) !=dfsGraph->vertexmap.end())
		{
			if((*itx)->shouldConsider==false) continue;
			if((*itx)->color==0){
				(*itx)->parent=(u)->label;
				u->childList.push_back(*itx);
				DFSVISIT((*itx));
			}
			else{
				if(u->lowv>(*itx)->startTime)
				{
					u->lowv=(*itx)->startTime;
				}
			}
		}
	}
	u->color=2;
}
set<int> randommining::getCutVertices()
{
	set<int> cutlist;
	cutlist.clear();
	for( ChangeableVertexIterator it=dfsGraph->myverts.begin();it!=dfsGraph->myverts.end();++it)
	{
		//cout<<(*it)->startTime<<" "<<(*it)->lowv<<" "<<(*it)->label<<" "<<(*it)->parent<<endl;
		if((*it)->startTime==1) {
			if((*it)->childList.size()>1) cutlist.insert((*it)->label);
		}
		else{
			for(vector<Vertex*>::iterator itv=(*it)->childList.begin();itv!=(*it)->childList.end();++itv)
			{
				if((*itv)->lowv >= (*it)->lowv) {	cutlist.insert((*it)->label);}
			}
		}
	}
	return cutlist;
}

bool randommining::isGraphConnected()
{
	if(allcomponents.size()>1) return false;	
	else return true;
}

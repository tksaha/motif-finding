/*
 * graph.cpp
 *
 * Created on: Oct 15, 2012
 * Author: Tanay Kumar Saha
 */


#include "graph.h"

using namespace std;

//no vertex with numbering -1 is allowed, no self loop, 
graph::graph()
{
	myverts.clear();
	myedges.clear();
	vertexmap.clear();	
}
graph::~graph()
{
	myverts.clear();
	myedges.clear();
	vertexmap.clear();
}
	
void graph::addVertex(Vertex* v)
{
        myverts.insert(v);
        vertexmap[v->label]=v->globallabel;
}

void graph::addEdge(Edge* E)
{
	myedges.insert(E);
}
/* undir true means undirected , false means directed */
void graph:: updateAdj(Vertex* s, Vertex* t, bool undir)
{	
	s->list.insert(t);
	if(undir) t->list.insert(s);
}
void graph:: printAdjacencyList()
{
	std::cout<<"printing Edge Pairs:"<<endl;
	
	for(ConstEdgeSetIterator itx=myedges.begin();itx!=myedges.end();++itx)
	{
	   std::cout<<(*itx)->st->label<<":"<<(*itx)->end->label<<endl;
	}
	std::cout<<endl<<endl;
        
	for(ConstVertexIterator it=myverts.begin();it!=myverts.end();++it)
	{
   	    std::cout<<"label"<<(*it)->label<<"("<<(*it)->globallabel<<")"<<(*it)->parent<<":";
	    for(ConstVertexIterator  it1=(*it)->list.begin();it1!=(*it)->list.end();++it1)
	    {
	      std::cout<<"->"<<(*it1)->label;
	    }
	    std::cout<<endl;
	}
}
	

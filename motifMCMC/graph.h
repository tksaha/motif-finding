#ifndef GRAPH_H_
#define GRAPH_H_
#include<set>
#include<vector>
#include<string>
#include<map>
#include<list>
#include<bitset>
#include<boost/interprocess/containers/flat_set.hpp>
#include<iostream>
#include<boost/unordered_map.hpp>
#include "boost/smart_ptr.hpp"
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/global_fun.hpp>
#define  NODELABEL 30
#define  EDGELABEL 20

using boost::multi_index_container;
using namespace boost::multi_index;


using namespace boost;
using namespace std;


typedef boost::container::flat_set<int> BoostIntSet;
typedef	std::map<int,int> ::const_iterator ConstMapIterator;


class Vertex{
public:
        int label;
	int  globallabel;
        boost::container::flat_set<Vertex*>list;
	vector<Vertex*>childList;
	int color;
	int parent;
	int startTime;
	int lowv;/* for examining the cut edge */
	bool shouldConsider;/* for doing constrained dfs */
	Vertex(){
		shouldConsider=true;
		list.clear();
		childList.clear();
		label=-1;
	}
        Vertex(int lab)
        {
	  	label=lab;
	  	list.clear();
	  	shouldConsider=true; /* In general we will always consider the vertex */
        }
	Vertex(int lab, int gbl)
	{
	        label=lab;
	  	list.clear();
	  	shouldConsider=true; /* In general we will always consider the vertex */
		globallabel=gbl;
	}
        void reset()
	{
	 	list.clear();
		childList.clear();
	}
        ~Vertex()
        {
	  	list.clear();
        } 
};

struct EdgePair{
	int st;
	int end;
	
	EdgePair(int stx,int endx)
	{
		st=stx;
		end=endx;
	}
};

class Edge{
public:
        Vertex  *st;
        Vertex  *end;
	int edgelabel;
	Edge(){}
	Edge( Vertex* start,  Vertex* ed, int  label)
	{
		st=start;
		end=ed;
		edgelabel=label;
	}
	int reset()
	{}
        ~Edge()
        { }	
};


class compareV { 
   public:
     inline bool operator()(const Vertex *x,const Vertex  *y)const{ 
	 	 return x->label<y->label;
      } 
};

class compareE { 
   public:
      inline bool operator()(const Edge   *x,const Edge *y)const{ 
	 if(x->st->label !=y->st->label) return x->st->label<y->st->label;
	 else return x->end->label<y->end->label;
      } 
 
};


typedef	boost::container::flat_set<Vertex*,compareV> VertexSet;
typedef	vector<Vertex*> VertexVector;
typedef boost::container::flat_set<Edge*,compareE> EdgeSet;
typedef	vector<Edge*> EdgeVector;


typedef EdgeSet::const_iterator ConstEdgeSetIterator;
typedef EdgeSet::iterator ChangeableEdgeSetIterator;


typedef VertexSet::const_iterator ConstVertexIterator;
typedef VertexSet::iterator ChangeableVertexIterator;

class graph{

public:
	VertexVector biggraphvertex;
	EdgeVector ev;
	VertexSet myverts;
	EdgeSet myedges;
	int myid;
	map<int,int> vertexmap;
public:
	int numV;
	int numE;
	graph();
	~graph();
	void reset()
	{
		myverts.clear();	
		myedges.clear();
		vertexmap.clear();
		biggraphvertex.clear();
	}
 	void addVertex(Vertex* v);
	void addEdge(Edge* E);
	void updateAdj(Vertex* s,  Vertex *t,bool undir);
	void printAdjacencyList();
};
typedef graph* GraphClassPtr;


class fiveTuple
{

public: int dfsst, dfsend;
        int stlabel,edgelabel,endlabel;
	fiveTuple(){}
        fiveTuple(int dfsSt,int dfsEnd,int stLabel, int edgeLabel, int endLabel)
        {
                dfsst=dfsSt;dfsend=dfsEnd;
                stlabel=stLabel;
                edgelabel=edgeLabel;
                endlabel=endLabel;
        }
};

class canonicalcodeStruc{
public:
        int maxcode;
        canonicalcodeStruc(){
                edges.clear();
                codetuples.clear();
                dfscodemap.clear();
                maxcode=0;
        }
        canonicalcodeStruc(canonicalcodeStruc* code)
        {}
	void reset()
	{
		edges.clear();	
		codetuples.clear();
		dfscodemap.clear();
		maxcode=0;
	}
	~canonicalcodeStruc(){}
        std::vector<Edge*> edges;
        std::vector<fiveTuple*>codetuples; //newly added
        map<int,int>dfscodemap;
};

             
class heapData{
public:
        heapData(){
		count=0;
		cumdfrac =0.0;
		countconcentration=0.0;
		degreeconcentration=0.0;
	}
	heapData(string codex,double dfrac):code(codex),cumdfrac(dfrac){
		count=1;
		countconcentration=0.0;
		degreeconcentration=0.0;
	}
        ~heapData(){
                code.assign("");
        }
	void reset()
	{
		code.assign("");
	}
	int count;
        string code;
	double cumdfrac;
	double countconcentration;
	double degreeconcentration;
        bool operator==(const heapData* other)const{
                if(code.compare(other->code)==0){
                        return true;
                }
                else return false;
        }
	friend std::ostream& operator<<(std::ostream& os,const heapData& e)
  	{
    		cout<<"#"<<e.code<<"#"<<e.count<<"|"<<e.countconcentration<<"|"<<endl;
    		return os;
  	}
};

class codex{};
class graphidsx{};


class myobjectpool{
private:
	std::list<Vertex*> vertexResources;
	std::list<Edge*> edgeResources;
	std::list<graph*> graphResources;
	std::list<fiveTuple*> fiveTupleResources;
	std::list<canonicalcodeStruc*> canonicalcodeResources;
	std::list<heapData*> heapDataResources;
public:	myobjectpool() {}


	Vertex* getVResource()
	{
		if (vertexResources.empty())
		{
			for(int i=0;i<150;i++)
			{
				Vertex *v =new Vertex();
				vertexResources.push_back(v);
			}
		}
		
		Vertex* resource = vertexResources.front();
		vertexResources.pop_front();
		return resource;
	}
	
	void returnVResource(Vertex* v)
	{
		v->reset();
		vertexResources.push_back(v);
	}
	Edge* getEResource()
	{
		if (edgeResources.empty())
		{
			for(int i=0;i<150;i++)
			{
				Edge *E=new Edge();
				edgeResources.push_back(E);
			}
		}
		
		Edge* resource = edgeResources.front();
		edgeResources.pop_front();
		return resource;
	}
	
	void returnEResource(Edge* E)
	{
		E->reset();
		edgeResources.push_back(E);
	}
	graph* getGResource()
	{
		if(graphResources.empty())
		{
			for(int i=0;i<1;i++)
			{
				graph* g=new graph();
				graphResources.push_back(g);
			}
		}
		graph* resource=graphResources.front();
		graphResources.pop_front();
		return resource;
	}
	void returnGResource(graph* g)
	{
		g->reset();
		graphResources.push_back(g);
	}
	fiveTuple* getFiveTupleResource()
	{
		if(fiveTupleResources.empty())
		{
			for(int i=0;i<40;i++)
			{
				fiveTupleResources.push_back(new fiveTuple());
			}
	
		}
		fiveTuple* resource =fiveTupleResources.front();
		fiveTupleResources.pop_front();
		return resource;
	}
	void returnFiveTupleResource(fiveTuple* five)
	{
		fiveTupleResources.push_back(five);
	}
	canonicalcodeStruc* getcancodeResource()
	{
		if(canonicalcodeResources.empty())
		{
			for(int i=0;i<40;i++)
			{
				canonicalcodeResources.push_back(new canonicalcodeStruc());
			}
		}
		canonicalcodeStruc* resource=canonicalcodeResources.front();
		canonicalcodeResources.pop_front();
		return resource;
	}
	void returncancodeResource(canonicalcodeStruc* code)
	{
		code->reset();
		canonicalcodeResources.push_back(code);
	}
	heapData* getheapDataResources()
	{
		if(heapDataResources.empty())
		{	
			for(int i=0;i<40;i++)
			{
				heapDataResources.push_back(new heapData());
			}
		}
		heapData* resource=heapDataResources.front();
		heapDataResources.pop_front();
		return resource;
	}
	void returnheapDataResources(heapData* h)
	{
		h->reset();
		heapDataResources.push_back(h);
	}
	
};
#endif

/* GRAPH_H_ */


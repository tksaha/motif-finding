#!usr/bin/python
#input graph must be in edge list. No duplicate edge. No self edge.
import sys
import queue
import operator

def BFS():
    while vc.empty()==False:
        u=vc.get_nowait()
        #print('out:')
        #print(u)
                    
        for y in adj[u]:
            if v[y]==0:
                v[y]=1
                con.add(y)
                vc.put(y)
                #print('in:')
                #print(y)
        v[u]=2    
            

if len(sys.argv) != 2: #program name and infile
    sys.exit("usage: check_undirected in")
    
f_in=open(sys.argv[1],'r')
f_out=open(sys.argv[1]+'BFSCON','w')
            
v={}
adj={}
vc=queue.Queue()

con=set()

t=0
for line in f_in:
    t+=1
    #if operator.mod(t,100000) == 0:
    #    print(t)
         
    splits=line.split()
    if (len(splits)==0):
        continue;
    if splits[0] not in v:
        v[splits[0]]=0
        adj[splits[0]]=set()
    if splits[1] not in v:
        v[splits[1]]=0
        adj[splits[1]]=set()
    adj[splits[0]].add(splits[1])
    adj[splits[1]].add(splits[0])

print('graph loaded')

for x in v:
    if v[x]==1:
        print('error')
    if v[x]==2:
        #vc=set()
        continue
    if v[x]==0:
        con.clear()
        con.add(x)
        vc=queue.Queue()
        vc.put(x)
        v[x]=1
        BFS()
    print(len(con))
    print(len(v))    
    if len(con) > len(v)/2:
        break
f_in.close()
f_in=open(sys.argv[1],'r')

for line in f_in:
    splits=line.split()
    if (len(splits)==0):
        continue;
    if splits[0] not in con:
        continue
    if splits[1] not in con:
        continue
    f_out.write(line)
    #print(len(con))

print('connect largest ready')

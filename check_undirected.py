'''

@author: mmrahman,tksaha
'''
import sys
if len(sys.argv) != 2: #program name and 1 arg
    sys.exit("usage: check_undirected in")
    
f_in=open(sys.argv[1]+'.txt','r')
f_out=open(sys.argv[1]+'UND.txt','w')
#f_dup=open(sys.argv[1]+'DUP.txt','w')

edges=set()
for line in f_in:
    splits=line.split()
    #print(line)
    if len(splits)==0:
        continue
    v1=int(splits[0])
    v2=int(splits[1])
    if v1<v2:
        e=str(v1)+'\t'+str(v2)
    elif v2<v1:
        e=str(v2)+'\t'+str(v1)
    else:
        print(str(v1)+' '+str(v2))
        continue   
    if e not in edges:
        edges.add(e)
        f_out.write(e)
        f_out.write('\n')
    #else:
    #    f_dup.write(e)
    #    f_dup.write('\n')
    



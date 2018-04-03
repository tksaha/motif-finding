import os;
import sys;

# @author: Tanay Kumar Saha
# @email:  nataycse@gmail.com


# Parameter 1: input file (graph file)
# Each line contains a pair (st tab end);
inputfile = open (sys.argv[1]);

#Output file:  
output_file = open (sys.argv[1]+"-mcmc-format","w");
input_mapping_file = open (sys.argv[1]+"-mappings","w");

node_list=[];

# read the file for the first pass
for	line	in	inputfile:
	lineelems = line.strip().split("\t");
	node_list.append (int(lineelems[0]));
	node_list.append (int(lineelems[1]));


uniq_nodeid_list = list (sorted(set (node_list)));


# read the input file for the second pass, build the correct input file
# write the mapping

inputfile = open (sys.argv[1]);
inputfile_mappings ={};

#populate the mapping

for	pos	in	range (0,len(uniq_nodeid_list)):
	inputfile_mappings [ uniq_nodeid_list[pos]]= pos;
	input_mapping_file.write (str(uniq_nodeid_list[pos])+"\t"+str(pos)+os.linesep);



for	line	in	inputfile:
	lineelems = line.strip().split("\t");
	st = inputfile_mappings[int(lineelems[0])];
	end = inputfile_mappings [int (lineelems [1])];
	output_file.write (str(st)+"\t"+str(end)+"\t"+"1"+"\t"+"1"+"\t"+"1"+os.linesep);



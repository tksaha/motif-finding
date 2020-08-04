# motif-finding
Scientists have shown that network motifs are key building block of various biological networks.
Most of the existing exact methods for finding network motifs are inefficient simply due 
to the inherent complexity of this task. In recent years, researchers are considering 
approximate methods that save computation by sacrificing exact counting of the frequency 
of potential motifs. However, these methods are also slow when one considers the motifs of larger size. 
In this work, we propose two methods for approximate motif finding, namely SRW-rw, and MHRW 
based on Markov Chain Monte Carlo (MCMC) sampling. Both the methods are significantly faster 
than the best of the existing methods, with comparable or better accuracy. Further, as the motif 
size grows the complexity of the proposed methods grows linearly.

# Input File Format
Node ids should be between 1 to max number of nodes in the network. Otherwise you will get segmentation fault.
So, please format your network file accordingly before your run the script. Please see the **yeast-mod** file inside
motifMCMC for reference.

I have provided three script for making proper file format for the executable to run. To make the right format please execute the script in the following sequence:

```
python  check_undirected.py CA-GrQc
```
Please note that there is no .txt at the end of the file. the script will generate a file with **UND** appended at the end of the file name.

```
python make_connected_BFS_2.py  CA-GrQcUND.txt
```
The script will create a file with BFSCON appened. Basically, it will create a connected network if the original input 
graph is not connected. 

```
python     make_proper_input_file_for_mcmc_sample.py  CA-GrQcUND.txtBFSCON
```

The above script will rearrage the node ids from 1 to maximum node id.

```
./motifMiner-mcmc.out  -d CA-GrQcUND.txtBFSCON-mcmc-format   -i 10000 -s 5 -q 10000 -undir 1
```



# Installation
To make executable, please run the following command under individual folder.

```
make
```

You can run the code in following way (In this version -undir should always be 1): 

```
./executablename -d [fileName] -i 100000 [iteration no] -s 4 [size] -q 1000 [queue size] -undir 1 [always expect undir 1]
```


# Reference
If you are using the code for research purposes, please consider citing the following paper: 

```
@inproceedings{saha.hasan:15*2,
  title={Finding Network Motifs Using MCMC Sampling.},
  author={Saha, Tanay Kumar and Al Hasan, Mohammad},
  booktitle={CompleNet},
  pages={13--24},
  year={2015}
}
```

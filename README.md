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
So, please format your network file accordingly before your run the script.


# Installation
To make executable, please run the following command under individual folder.

```
make
```

You can run the code in following way: 

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

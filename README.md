# morph-opening-transform
Implementation of morphological opening transform in few versions: sequantial, basic parallel and optimized for Xeon Phi.

## Variants
Sequential - is executed on a single thread.  
Basic parallel - parallelized using OpenMP.  
FSA parallel - False Sharing Avoidance, data split into 64B batches to avoid overhead for data synchronization between threads.  

## Building
Makefile_phi - use to compile with icc. You will be able to run application on Xeon Phi coprocessor in the native mode. Remember to import variables for compiler, eg. `export /opt/icc_xe2013/bin/compilervars.sh intel64`  
  
Makefile - use for normal build with gcc. You will be able to test application using your processor.  

## Dependencies
* QDBMP - BMP library  

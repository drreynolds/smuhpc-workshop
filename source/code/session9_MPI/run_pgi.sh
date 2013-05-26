#!/bin/bash

# desired number of MPI processes
NPROCS_=8

# executable name
EXENAME_=driver.exe

# executable arguments
ARGS_="input1 input2 input3"

/grid/software/mpich2-1.3.2/bin/mpiexec -n $NPROCS_ $EXENAME_ $ARGS_

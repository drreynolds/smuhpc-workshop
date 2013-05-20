###################################################################
# Makefile for SMU HPC Workshop, session 5 (Advanced programming)
#
# Daniel R. Reynolds
# SMU Mathematics
# 19 May 2013
###################################################################

FC  = gfortran
OPT = -O0

all : driver1.exe driver2.exe

driver1.exe : driver1.f90 vectors.f90 get_time.f90 Makefile
	$(FC) $(OPT) driver1.f90 vectors.f90 get_time.f90 -o driver1.exe 

driver2.exe : driver2.f90 tridiag_matvec.f90 Makefile
	$(FC) $(OPT) driver2.f90 tridiag_matvec.f90 -o driver2.exe 


####### End of Makefile #######

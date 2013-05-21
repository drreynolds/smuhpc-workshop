###################################################################
# Makefile for SMU HPC Workshop, session 5 (Advanced programming)
#
# Daniel R. Reynolds
# SMU HPC Workshop
# 21 May 2013
###################################################################

CC  = gcc
OPT = -O0

all : driver1.exe driver2.exe

driver1.exe : driver1.c vectors.c Makefile
	$(CC) $(OPT) driver1.c vectors.c -o driver1.exe -lm

driver2.exe : driver2.c tridiag_matvec.c Makefile
	$(CC) $(OPT) driver2.c tridiag_matvec.c -o driver2.exe -lm


####### End of Makefile #######

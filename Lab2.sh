#!/bin/bash
#PBS -l nodes=2:ppn=2,walltime=00:05:00
#PBS -N Marchenko_Lab2
cd $PBS_O_WORKDIR
subThread=0
thread=0
subRootFolder=""
if [ -z "$num" ]; then
  exit
fi
if [ $num -le 0 ]; then
  exit
fi
num=`expr $num - 1`
qsub -v num=$num manager.pbs
subThread=`expr $num % 10`
thread=`expr $num / 10`
subRootFolder="threadFolder${thread}"
mkdir -p "${subRootFolder}"
ml icc
ml openmpi
mpirun ./mpi >> "${subRootFolder}/subThread${subThread}.txt"
numOfFiles=$( ls ${subRootFolder} | wc -l )
if [ $subThread -eq 0 ]
then
while [ $numOfFiles -lt 10 ]
do
numOfFiles=$( ls ${subRootFolder} | wc -l )
done
cat ${subRootFolder}/* > "statFile${thread}.txt"
echo "${thread}" >> test2.txt
if [ $thread -eq 0]
then
echo passed >> test2.txt
numOfReports=$( ls statFile* | wc -l )
while [ $numOfReports -lt 2 ]
do
numOfReports=$( ls statFile* | wc -l )
done
cat statFile* > finalReport.txt
fi
echo "${numOfFiles}" >> test.txt

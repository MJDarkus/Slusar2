#!/bin/bash
#PBS -l walltime=00:03:00
#PBS -l nodes=2
#PBS -l ncpus=2
#PBS -N Marchenko_lab2
#PBS -t 1-100
#PBS –W stagein=/mnt/work/tb382/mpi@plus7.cluster.univ.kiev.ua:/home/grid/testbed/tb382/Slusar2/lab2/mpt

#PBS -W stageout=/mnt/work/tb382/output.task*@plus7.cluster.univ.kiev.ua:/home/grid/testbed/tb382/Slusar2/lab2/final

//cd ~/5kurs/lab2/final

c=9
ml icc
ml openmpi

if [ $PBS_ARRAYID == 1 ]; then
  while [ $(ls $(seq -f 'results.task%g' 10 10 100) | wc -l) -ne 10 ]; do
    sleep 5
  done 
  echo > total
  for task_no in $(seq 10 10 100); do
    if [ $task_no -eq 10 ]; then 
      c=8; 
    else 
      c=9; 
    fi
    echo "Stats for tasks "$(($task_no-$c))" - "$(($task_no-1))":" >> total
    cat results.task$task_no | awk '{ print $0; }' >> total
    echo "" >> total
  done
elif [ $(($PBS_ARRAYID%10)) -eq 0 ]; then
  if [ $PBS_ARRAYID -eq 10 ]; then 
    c=8; 
  else 
    c=9; 
  fi
  while [ $(ls $(seq -f 'output.task%g' $(($PBS_ARRAYID-$c)) $(($PBS_ARRAYID-1))) | wc -l) -ne $c ]; do
    sleep 3
  done 
  cat $(seq -f 'output.task%g' $(($PBS_ARRAYID-$c)) $(($PBS_ARRAYID-1))) |\
  awk '{ split($0,a,""); split(a[5],b,","); print b[1]; }' |\
  uniq -c > results.task$PBS_ARRAYID
else
  /mnt/work/tb382/mpi > /mnt/work/tb382/output.task$PBS_ARRAYID
fi

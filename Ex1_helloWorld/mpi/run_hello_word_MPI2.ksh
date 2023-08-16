#!/bin/ksh

if [ $# -ne 1 ]
then
    echo ""
    echo "usage: ${0} [NP]"
    echo ""
    echo "where:"
    echo ""
    echo "NP: total_ranks_MPI ::   total number of MPI ranks/processes"
    echo ""
    exit
fi

np=${1}
codname="hello_world_MPI2"


if [ ! -s ${codname}.x ] 
then
    echo "Executável nao existe. ${codname}.x"
    exit
fi

mpirun -n ${np} ${codname}.x


#!/bin/ksh

codname="hello_world_MPI2"

rm -f ${codname}.x

echo "Compilando..."
    mpicc ${codname}.c -o ${codname}.x
echo "Compilado!  ${codname}.x"



if [ ! -s ${codname}.x ] 
then
    echo "Deu ruim..."
fi



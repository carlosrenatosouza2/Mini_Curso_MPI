#!/bin/ksh

codname="hello_world_seq"

rm -f ${codname}.x

echo "Compilando..."
    gcc ${codname}.c -o ${codname}.x
echo "Compilado!  ${codname}.x"



if [ ! -s ${codname}.x ] 
then
    echo "Deu ruim..."
fi



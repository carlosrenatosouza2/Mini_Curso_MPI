#!/bin/ksh

rm -f hello_world_seq.x

echo "Compilando..."
gcc hello_world_seq.c -o hello_world_seq.x

echo "Compilado!"

if [ ! -s hello_world_seq.x ] 
then
    echo "Deu ruim..."
fi



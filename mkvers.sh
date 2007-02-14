#! /bin/sh
hg tip --template='\nconst char * __version="$Id: @(#)calife revision: {node} $";\n'>version.c

#! /bin/sh
hg tip --template='const char * __version="$Id: @(#)calife revision: {node} $";'>>version.c

#!/bin/bash
dir=/tmp/readesm-$RANDOM
file=$(basename $1).html
mkdir $dir
readesm --infile $1 --outfile $dir/$file --format html
tar xjf PREFIX/share/readesm/images.tar.bz2 -C $dir
kfmclient openURL $dir/$file text/html
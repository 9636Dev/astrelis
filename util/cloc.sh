#!/bin/sh

cd $(dirname $0)

cmd=$(cat cloc.txt)
cd ../
$cmd


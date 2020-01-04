#!/bin/bash

src="./src"
graphFile="/tmp/aster-graph.dot"

cd $src
cpp-dependencies --graph $graphFile


xdot $graphFile


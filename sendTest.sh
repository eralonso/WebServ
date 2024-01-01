#! /bin/bash

function readQuestion()
{
	local input
	readInput input "$2"
	while [ "$input" != "y" ] && [ "$input" != "n" ]; do
		readInput input "Bad input, please introduce an option between \'y\' or \'n\': "
	echo "$input"
	done
	eval "$1=$input"
}

function readInput()
{
	local input
	while [ -z "$input" ]; do
		printf "$2 "
		read input
	done
	eval "$1=\"$input\""
}

echo "Starting Test"
readInput host "Introduce the host:"
readInput port "Introduce the port:"
readInput iterations "Introduce the number of request that you want to send:"
readQuestion background "Do you want to send all the requests at the same time? [ y / n ]"
echo "host: $host && port: $port && iterations: $iterations && background: $background"
echo "ALL OK"

#! /bin/bash

function readInput()
{
	local readInput
	while [ -z "$readInput" ]; do
		printf "$2 "
		read readInput
	done
	eval "$1=\"$readInput\""
}

function readNumber()
{
	local numberInput
	readInput numberInput "$2"
	while ! [[ "$numberInput" =~ ^[0-9]+$ ]]; do
		readInput numberInput "Bad input, please introduce an integer: "
	done
	eval "$1=$numberInput"
}

function readQuestion()
{
	local questionInput
	readInput questionInput "$2"
	while [ "$questionInput" != "y" ] && [ "$questionInput" != "n" ]; do
		readInput questionInput "Bad input, please introduce an option between \'y\' or \'n\': "
	done
	eval "$1=$questionInput"
}

function sendRequest()
{
	curl -v "$1:$2" >> outputLog 2>> errorLog
}

function doTest()
{
	echo -e "Request [$1] host [$3] port [$4]\n" >> outputLog
	echo -e "Request [$1] host [$3] port [$4]\n" >> errorLog
	sendRequest "$3" "$4"
	if [ "$1" -lt "$2" ]; then
		echo -e "----------------------------------------------------------\n\n" >> outputLog
		echo -e "----------------------------------------------------------\n\n" >> errorLog
	fi
}

function main()
{
	local host
	local port
	local iterations
	local background
	local iter=1

	readInput host "Introduce the host:"
	readNumber port "Introduce the port:"
	readNumber iterations "Introduce the number of request that you want to send:"
	readQuestion background "Do you want to send all the requests at the same time? [ y / n ]"

	echo "host: $host && port: $port && iterations: $iterations && background: $background"

	> outputLog > errorLog
	while [ "$iter" -le "$iterations" ]; do
		if [  "$background" = "y" ]; then
			( doTest "$iter" "$iterations" "$host" "$port" & )
		else
			doTest "$iter" "$iterations" "$host" "$port"
		fi
		((++iter))
	done
	echo "The Standard Output of requests is stored in the \"outputLog\" file and Standard Error in the \"errorLog\" file"
}

main

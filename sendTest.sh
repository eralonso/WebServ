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
	curl -m 2 -v "$1:$2"
}

function doTest()
{
	local testOutputLog="$outputLog$1"
	local testErrorLog="$errorLog$1"

	> "$testOutputLog" > "$testErrorLog"
	echo -e "Request [$1] host [$3] port [$4]\n" >> "$testOutputLog"
	echo -e "Request [$1] host [$3] port [$4]\n" >> "$testErrorLog"
	( sendRequest "$3" "$4" ) >> "$testOutputLog" 2>> "$testErrorLog"
	if [ "$1" -lt "$2" ]; then
		echo -n "-------------------------------------------------" >> "$testOutputLog"
		echo -e "-------------------------------------------------\n\n" >> "$testOutputLog"
		echo -n "-------------------------------------------------" >> "$testErrorLog"
		echo -e "-------------------------------------------------\n\n" >> "$testErrorLog"
	fi
}

function launchTests()
{
	local iter=1

	while [ "$iter" -le "$3" ]; do
		if [  "$4" = "y" ]; then
			( doTest "$iter" "$3" "$1" "$2" & )
		else
			doTest "$iter" "$3" "$1" "$2"
		fi
		((++iter))
	done
}

function waitBackgroundProccess()
{
	while [[ "$( ps aux | awk -v PROGRAM=$0 '{ if ( $12 == PROGRAM ) { print "y" } }' | wc -l )" -gt 2 ]]; do
		sleep 0.0001;
		echo "Waiting..."
	done
}

function joinFiles()
{
	local iter=1

	while [ "$iter" -le "$1" ]; do
		cat "$outputLog$iter" >> $outputLog
		cat "$errorLog$iter" >> $errorLog
		rm -f "$outputLog$iter" "$errorLog$iter"
		((++iter))
	done
}

function main()
{
	local host
	local port
	local iterations
	local background

	readInput host "Introduce the host:"
	readNumber port "Introduce the port:"
	readNumber iterations "Introduce the number of request that you want to send:"
	readQuestion background "Do you want to send all the requests at the same time? [ y / n ]"

	> "$outputLog" > "$errorLog"

	echo "Sending Requests..."
	launchTests "$host" "$port" "$iterations" "$background"
	waitBackgroundProccess
	echo "Merging files..."
	joinFiles "$iterations"

	echo -en "\nThe Standard Output of requests is stored in the \"$outputLog\""
	echo " file and Standard Error in the \"$errorLog\" file"
}

outputLog="output.log"
errorLog="error.log"

main

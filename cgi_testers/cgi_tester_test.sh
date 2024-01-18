		export SERVER_SOFTWARE="webserv"
		export SERVER_NAME="localhost"
		export GATEWAY_INTERFACE="CGI/1.0"
		export SERVER_PROTOCOL="HTTP/1.1"
		export SERVER_PORT="8000"
		export REQUEST_METHOD="GET"
		#export SCRIPT_NAME="tester_meal"
		export QUERY_STRING="value=5"
		export REMOTE_HOST="localhost"
		export REMOTE_ADDRESS="localhost"
		export AUTH_TYPE="none"
		export REMOTE_USER="user"
		export REMOTE_IDENT="user"
		export CONTENT_TYPE="Content-Type: text/html"
		export CONTENT_LENGTH="0"
		export HTTP_ACCEPT="Accept: text/html"
		export USER_AGENT="User-Agent: cgi_tester_test"
		#export FILEPATH_INFO="./tester_meal"
		#export PATH_INFO="./tester_meal"
		export PATH_INFO="./"
		#export PATH_TRANSLATED="./tester_meal"
        echo "tester_meal content: "
        cat tester_meal
        echo "calling... cat tester_meal | ./ubuntu_cgi_tester;"
        cat tester_meal | ./ubuntu_cgi_tester tester_meal;
        # echo "calling... ./ubuntu_cgi_tester;"
        # ./ubuntu_cgi_tester;
        echo ""
        echo ""
        echo "Environment state"
		echo SERVER_SOFTWARE $SERVER_SOFTWARE
		echo SERVER_NAME $SERVER_NAME
		echo GATEWAY_INTERFACE $GATEWAY_INTERFACE
		echo SERVER_PROTOCOL $SERVER_PROTOCOL
		echo SERVER_PORT $SERVER_PORT
		echo REQUEST_METHOD $REQUEST_METHOD
		echo FILEPATH_INFO $FILEPATH_INFO
		echo PATH_INFO $PATH_INFO
		echo PATH_TRANSLATED $PATH_TRANSLATED
		echo SCRIPT_NAME $SCRIPT_NAME
		echo QUERY_STRING $QUERY_STRING
		echo REMOTE_HOST $REMOTE_HOST
		echo REMOTE_ADDRESS $REMOTE_ADDRESS
		echo AUTH_TYPE $AUTH_TYPE
		echo REMOTE_USER $REMOTE_USER
		echo REMOTE_IDENT $REMOTE_IDENT
		echo CONTENT_TYPE $CONTENT_TYPE
		echo CONTENT_LENGTH $CONTENT_LENGTH
		echo HTTP_ACCEPT $HTTP_ACCEPT
		echo USER_AGENT $USER_AGENT

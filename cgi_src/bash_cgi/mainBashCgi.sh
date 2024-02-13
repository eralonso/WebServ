# echo "Start BASH script -->" 1>&2;
LINEEND=$'\r\n';

format_env_line()
{
	LINEVAL=${1#*=};  # Remove everything up to and including first =
	LINEKEY+=${1%%=*};
	echo "$LINEKEY: $LINEVAL";
}

format_env()
{
	envContent=$(env | while read line ; do (echo "<p>"$(format_env_line $line)"</p>";) done);
}

format_body()
{
	BODY="<body>"$LINEEND;
	BODY+="<h1 style=\"color: #0FF00F;\">Made in Bash though CGI</h1>"$LINEEND;
	BODY+="<h3 style=\"color: #000F0F;\">This is the environment</h3>"$LINEEND;
	BODY+="<details close>"$LINEEND;
	BODY+="<summary>Environment Variables</summary>"$LINEEND;
	BODY+="<div style="background-color:AA9988">"$LINEEND;
	format_env;
	BODY+="$envContent";
	BODY+="</div>"$LINEEND;
	BODY+="</details>"$LINEEND;
	userCookie=$HTTP_COOKIE;
	userCookie=$(echo  $userCookie | awk -F '[][]' '{for(i=2;i<=NF;i+=2) print $i}');
	userCookie=$(echo  $userCookie | awk -F '&' '{print $1}');
	userCookie=$(echo  $userCookie | awk -F '=' '{print $2}');
	if [ "$userCookie" != "" ]; then
		BODY+="<h3>Current User: $userCookie</h3>$LINEEND";
		BODY+="<button onclick=\"document.cookie = 'sessionId=; Path=/; Expires=Thu, 01 Jan 1970 00:00:01 GMT;';  location.reload();\">LOGOUT</button>"$LINEEND;
	else
		BODY+="<form method=\"POST\" action=\"/cgi_src/bash_cgi/loginAction.sh\" style=\"display: flex; flex-direction: column; max-width:20em; margin: auto; padding: 1em; border: solid black 1px\">"$LINEEND;
		BODY+="<h3>Login Form</h3>"$LINEEND;
		BODY+="<div style=\"max-width:18em; margin: auto; padding: 1em;\">"$LINEEND;
		BODY+="<label for=\"user\">user</label>"$LINEEND;
		BODY+="<input type=\"user\" id=\"user\" name=\"user\"\>"$LINEEND;
		BODY+="</div>"$LINEEND;
		BODY+="<div style=\"max-width:18em; margin: auto; padding: 1em;\">"$LINEEND;
		BODY+="<label for=\"pass\">password</label>"$LINEEND;
		BODY+="<input type=\"password\" id=\"pass\" name=\"pass\"\>"$LINEEND;
		BODY+="</div>"$LINEEND;
		BODY+="<input type=\"submit\" style=\"width:16em; self-justify: center; margin: auto; padding: 1em;\" id=\"submit\"\>"$LINEEND;
		BODY+="</form>"$LINEEND;
	fi
	BODY+="</body>"$LINEEND;
}

ACUM="Status: 200 OK "$LINEEND;
ACUM+="Content-Type: text/html"$LINEEND;
ACUM+="Location: $SERVER_NAME:$SERVER_PORT"$LINEEND;
ACUM+=$LINEEND;
format_body;
ACUM+=$BODY;
echo -e "$ACUM";
# echo -e "Written to server..." 1>&2;
# echo -e "$ACUM" 1>&2;
# echo "Finish BASH script <--" 1>&2;
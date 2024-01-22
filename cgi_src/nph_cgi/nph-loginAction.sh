echo "Start BASH script -->" 1>&2;
LINEEND=$'\r\n';

format_env ()
{
	envContent=$(env | awk -F '=' '{print "<p>"$1": "$2"</p>"}');
}

format_body()
{
	BODY="<body>"$LINEEND;
	BODY+="<h1>BASH Login Response</h1>"$LINEEND;
	BODY+="<h3>This is the environment</h3>"$LINEEND;
	BODY+="<details close>"$LINEEND;
	BODY+="<summary>Environment Variables</summary>"$LINEEND;
	BODY+="<div style="background-color:AA9988">"$LINEEND;
	format_env;
	BODY+="$envContent";
	BODY+="</div>"$LINEEND;
	BODY+="</details>"$LINEEND;
	BODY+="<script>window.location.assign(\"./nph-main.sh\")</script>"$LINEEND;
	BODY+="</body>"$LINEEND;
}

read -d $'\04' VALUE
echo -e "Read from webserv: " 1>&2;
echo -e "$VALUE" 1>&2;
echo -e "$LINEEND" 1>&2;
if [ "$REQUEST_METHOD" = "POST" ]; then
	format_body;
	ACUM=$SERVER_PROTOCOL" 200 OK "$LINEEND;
	ACUM+="Server: "$SERVER_NAME$LINEEND;
	ACUM+="Content-Type: text/html; charset=utf-8"$LINEEND;
	ACUM+="Location: $SERVER_NAME:$SERVER_PORT"$LINEEND;
	ACUM+="Content-Length: "${#BODY}$LINEEND;
	ACUM+="Set-Cookie: sessionId=bash[$VALUE]; Path=/; Max-Age=2592000"$LINEEND;
	ACUM+=$LINEEND;
	ACUM+=$BODY;
else
	format_body;
	ACUM=$SERVER_PROTOCOL" 405 NotAllowedMethod "$LINEEND;
	ACUM+="Server: "$SERVER_NAME$LINEEND;
	ACUM+="Content-Type: text/html; charset=utf-8"$LINEEND;
	ACUM+="Location: $SERVER_NAME:$SERVER_PORT"$LINEEND;
	# ACUM+="Content-Length: "${#BODY}$LINEEND;
	# ACUM+=$LINEEND;
	# ACUM+=$BODY;
fi
echo -e "$ACUM";
echo -e "Written to server..." 1>&2;
echo -e "$ACUM" 1>&2;
echo "Finish BASH script <--" 1>&2;

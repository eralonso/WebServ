# echo "Start BASH script -->" 1>&2;
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
	BODY+="<script>window.location.assign(\"./mainBashCgi.sh\")</script>"$LINEEND;
	BODY+="</body>"$LINEEND;
}

read -d $'\04' VALUE;
# echo -e "Read from webserv: " 1>&2;
# echo -e "$VALUE" 1>&2;
# echo -e "$LINEEND" 1>&2;
if [ "$REQUEST_METHOD" = "POST" ]; then
	ACUM="Status: 200 OK "$LINEEND;
	ACUM+="Content-Type: text/html; charset=utf-8"$LINEEND;
	ACUM+="Location: $SERVER_NAME:$SERVER_PORT/cgi_src/bash_cgi/main.sh"$LINEEND;
	ACUM+="Set-Cookie: sessionId=bash[$VALUE]; Path=/; Max-Age=2592000"$LINEEND;
	ACUM+=$LINEEND;
	format_body;
	ACUM+=$BODY;
else
	ACUM="Status: 405 NotAllowedMethod"$LINEEND;
	ACUM+="Content-Type: text/html; charset=utf-8"$LINEEND;
	ACUM+=$LINEEND;
	format_body;
	ACUM+=$BODY;
fi
echo -e "$ACUM";
# echo -e "Written to server..." 1>&2;
# echo -e "$ACUM" 1>&2;
# echo "Finish BASH script <--" 1>&2;
echo "Start BASH script -->" 1>&2
LINEEND="\r\n"
BODY="<h1>Made in Bash though CGI</h1>"$LINEEND
BODY+="<h3>This is the environment</h3>"$LINEEND
BODY+="<div style="background-color:AA9988">"$LINEEND
BODY+="<p>SERVER_SOFTWARE:"$SERVER_SOFTWARE"</p>"$LINEEND
BODY+="<p>SERVER_NAME:"$SERVER_NAME"</p>"$LINEEND
BODY+="<p>GATEWAY_INTERFACE:"$GATEWAY_INTERFACE"</p>"$LINEEND
BODY+="<p>SERVER_PROTOCOL:"$SERVER_PROTOCOL"</p>"$LINEEND
BODY+="<p>SERVER_PORT:"$SERVER_PORT"</p>"$LINEEND
BODY+="<p>REQUEST_METHOD:"$REQUEST_METHOD"</p>"$LINEEND
BODY+="<p>PATH_INFO:"$PATH_INFO"</p>"$LINEEND
BODY+="<p>PATH_TRANSLATED:"$PATH_TRANSLATED"</p>"$LINEEND
BODY+="<p>SCRIPT_NAME:"$SCRIPT_NAME"</p>"$LINEEND
BODY+="<p>QUERY_STRING:"$QUERY_STRING"</p>"$LINEEND
BODY+="<p>REMOTE_HOST:"$REMOTE_HOST"</p>"$LINEEND
BODY+="<p>REMOTE_ADDRESS:"$REMOTE_ADDRESS"</p>"$LINEEND
BODY+="<p>AUTH_TYPE:"$AUTH_TYPE"</p>"$LINEEND
BODY+="<p>REMOTE_USER:"$REMOTE_USER"</p>"$LINEEND
BODY+="<p>REMOTE_IDENT:"$REMOTE_IDENT"</p>"$LINEEND
BODY+="<p>HTTP_COOKIE:"$HTTP_COOKIE"</p>"$LINEEND
BODY+="<p>CONTENT_LENGTH:"$CONTENT_LENGTH"</p>"$LINEEND
BODY+="<p>HTTP_ACCEPT:"$HTTP_ACCEPT"</p>"$LINEEND
BODY+="<p>USER_AGENT:"$USER_AGENT"</p>"$LINEEND
BODY+="<p>__CF_USER_TEXT_ENCODING:"$__CF_USER_TEXT_ENCODING"</p>"$LINEEND
BODY+="<p>LC_CTYPE:"$LC_CTYPE"</p>"$LINEEND
BODY+="</div>"$LINEEND
BODY+="<button onclick=\"document.cookie = 'sessionId=; Path=/; Expires=Thu, 01 Jan 1970 00:00:01 GMT;';\">LOGOUT</button>\n"$LINEEND

echo "Status: 200 OK"
echo "Content-Type: text/html"
echo "Location: "$SERVER_NAME":"$SERVER_PORT
echo "Set-Cookie: sessionId=bash38afes7a8; Path=/; Max-Age=2592000\r";
echo ""
echo -e $BODY
echo "Finish BASH script <--" 1>&2
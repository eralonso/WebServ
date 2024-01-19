import sys
import os
sys.stderr.write("Executing python start->\n")

def getBody():
	env = ""
	for var in os.environ:
		env += "<p>" + var + ": " + os.environ[var] + "</p>\r\n"
	evalBody = ("<body>\r\n")
	evalBody += "<h1 style=\"color: #F00F0F;\">Python Login Response</h1>\r\n"
	evalBody += "<h3 style=\"color: #000F0F;\">This is the environment</h3>\r\n"
	evalBody += ("<div>\r\n")
	evalBody += "<details close>\r\n"
	evalBody += "<summary>Environment Variables</summary>\r\n"
	evalBody += env
	evalBody += ("</div>\r\n")
	evalBody += ("</details>\n")
	evalBody += ("<script>window.location.assign(\"./nph-main.py\")</script>\n")
	evalBody += ("</body>\r\n")
	return evalBody

value = sys.stdin.readline()
acum = ""
sys.stderr.write("Read from webserv: ")
sys.stderr.write(value)
sys.stderr.write("\n")
if (os.environ["REQUEST_METHOD"] == "POST"):
	acum += "Status: 200 OK\r\n"
	acum += "Content-Type: text/html; charset=utf-8\r\n"
	acum += "Location: " + os.environ["SERVER_NAME"] + ":" + os.environ["SERVER_PORT"] + "/cgi_src/python_cgi/main.py\r\n"
	#acum += "Set-Cookie: <cookie-name>=<cookie-value>; Expires=<date>\r\n"
	acum += "Set-Cookie: sessionId=python["+ value +"]; Path=/; Max-Age=2592000\r\n"
	#acum += "Set-Cookie: <cookie-name>=<cookie-value>; Domain=<domain-value>\r\n"
	#acum += "Set-Cookie: <cookie-name>=<cookie-value>; Path=<path-value>\r\n"
	acum += "\r\n"
	acum += getBody()
else:
	acum += "Status: 405 NotAllowedMethod\r\n"
	acum += "Content-Type: text/html; charset=utf-8\r\n"
	# acum += "Location: localhost:" + os.environ["SERVER_PORT"] + "/\r\n"
	acum += getBody()

sys.stdout.write(acum)
sys.stderr.write("Written to webserv: ")
sys.stderr.write(acum)
sys.stderr.write("Executing python finished-<\n")

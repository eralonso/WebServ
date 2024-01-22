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
	acum = ""
	body = getBody()
	acum += os.environ["SERVER_PROTOCOL"] + " 200 OK\r\n"
	acum += "Server: " + os.environ["SERVER_NAME"] + "\r\n"
	acum += "Content-Type: text/html; charset=utf-8\r\n"
	acum += "Content-Length: " + str(len(body)) + "\r\n"
	acum += "Location: " + os.environ["SERVER_NAME"] + ":" + os.environ["SERVER_PORT"] + "./nph-main.py\r\n"
	acum += "Set-Cookie: sessionId=python["+ value +"]; Path=/; Max-Age=2592000\r\n"
	acum += "\r\n"
	acum += body
else:
	acum = ""
	body = ""
	acum += os.environ["SERVER_PROTOCOL"] + " 405 NotAllowedMethod\r\n"
	acum += "Server: " + os.environ["SERVER_NAME"] + "\r\n"
	acum += "Content-Type: text/html; charset=utf-8\r\n"
	acum += "Content-Length: " + str(len(body)) + "\r\n"
	acum += "Location: " + os.environ["SERVER_NAME"] + ":" + os.environ["SERVER_PORT"] + "/error_pages/405.html\r\n"
	acum += "\r\n"
	# acum += body

sys.stdout.write(acum)
sys.stderr.write("Written to webserv: ")
sys.stderr.write(acum)
sys.stderr.write("Executing python finished-<\n")

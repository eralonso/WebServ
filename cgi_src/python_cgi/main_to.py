import sys
import os
sys.stderr.write("Executing python start->\n")
value = sys.stdin.readline()
acum = ""
body = ""
env = ""
for i in range(20000000):
#for i in range(5):
    cat = str(i)
for var in os.environ:
    env += "<p>" + var + ": " + os.environ[var] + "</p>\n"
sys.stderr.write("Read from webserv: ")
sys.stderr.write(value)
sys.stderr.write("\n")
sys.stderr.write("Environment variables: \n")
sys.stderr.write(env)
sys.stderr.write("\n")
acum += "Status: 200 OK\r\n"
acum += "Content-Type: text/html; charset=utf-8\r\n"
acum += "Location: localhost:8000/\r\n"
#acum += "Set-Cookie: <cookie-name>=<cookie-value>; Expires=<date>\r\n"
acum += "Set-Cookie: sessionId=python38afes7a8; Path=/; Max-Age=2592000\r\n"
#acum += "Set-Cookie: <cookie-name>=<cookie-value>; Domain=<domain-value>\r\n"
#acum += "Set-Cookie: <cookie-name>=<cookie-value>; Path=<path-value>\r\n"
acum += "\r\n"
body += ("<body>\n")
body += "<h1 style=\"color: #F00F0F;\">This is the body given by python</h1>\n"
body += ("<div>\n")
body += "<h3 style=\"color: #000F0F;\">This is the environment</h3>\n"
body += env
body += ("</div>\n")
body += ("</body>\n")
#acum += "HTTP/1.1 307 Temporary Redirect\r\n"
#acum += "HTTP/1.1 200 OK\r\n"
#acum += "Content-Type: text/html\r\n"
#acum += "Content-Length: " + str(len(body)) + "\r\n"
#acum += "Location: localhost:8000/\r\n"
#acum += "\r\n"
acum += body
sys.stdout.write(acum)
sys.stderr.write("Written to webserv: ")
sys.stderr.write(acum)
sys.stderr.write("Executing python finished-<\n")

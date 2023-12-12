import sys
import os
sys.stderr.write("Executing python start->\n")
value = sys.stdin.readline()
#for i in range(20000000):
acum = ""
env = ""
for i in range(5):
    cat = str(i)
for var in os.environ:
    env += "<p>" + var + ": " + os.environ[var] + "</p>\n"
sys.stderr.write("Read from webserv: ")
sys.stderr.write(value)
sys.stderr.write("\n")
sys.stderr.write("Environment variables: \n")
sys.stderr.write(env)
sys.stderr.write("\n")
acum += ("<body>\n")
acum += "<h1 style=\"color: #F00F0F;\">This is the body given by python</h1>\n"
acum += ("<div>\n")
acum += "<h3 style=\"color: #000F0F;\">This is the environment</h3>\n"
acum += env
acum += ("</div>\n")
acum += ("</body>\n")
sys.stdout.write(acum)
sys.stderr.write("Written to webserv: ")
sys.stderr.write(acum)
sys.stderr.write("Executing python finished-<\n")

import sys
sys.stderr.write("Executing python start->\n")
value = sys.stdin.readline()
#for i in range(20000000):
for i in range(5):
    cat = str(i)
sys.stderr.write("Read from webserv: ")
sys.stderr.write(value)
sys.stderr.write("\n")
sys.stdout.write ("<body><h1 style=\"color: #F00F0F;\">This is the body given by python</h1></body>\n")
sys.stderr.write("Written to webserv: ")
sys.stderr.write("<body><h1>This is the body given by python</h1></body>\n")
sys.stderr.write("Executing python finished-<\n")

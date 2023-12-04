import sys
value = sys.stdin.readline()
sys.stderr.write("Read from server: ")
sys.stderr.write(value)
sys.stderr.write("\n")
print ("<body><h1>This is the body given by python</h1></body>")
sys.stderr.write('Py error message\n')

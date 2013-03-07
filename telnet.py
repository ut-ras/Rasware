#!/usr/bin/python 

import telnetlib , sys

connection = telnetlib.Telnet("localhost","4444")
for command in sys.argv:
	connection.read_until(b">")
	connection.write(command.encode('ascii') + b"\n")
connection.close()

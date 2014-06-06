#!/usr/bin/env python
import json
import sys
import ast

with open('output.txt') as openfileobject:
    for line in openfileobject:
	jsonString = line.split('{', 1)[-1]
	if jsonString != '\n':
		jsonString = '{'+jsonString.split('\n',1)[0]
		jsonString = jsonString.split('\r',1)[0]
		sys.stderr.write(jsonString)
		sys.stderr.flush()
		data = json.loads(jsonString)
		#index = jsonString.find('SVLX')
		

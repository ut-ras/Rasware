import serial, json

import atexit

SERIAL_LINE = "COM14"
BAUD_RATE = 115200

def printError(s):
    print "ERROR: ", s

def onKill():
    print "closing connection"
    connection.close()

atexit.register(onKill)

# initialize connection

connection = serial.Serial(
    port = SERIAL_LINE, 
    baudrate = BAUD_RATE, 
    timeout = .2, 
    writeTimeout = 1
    )

# write to launchpad

connection.flush()
connection.write('{"right_motor":"200"}\n')
connection.flushOutput()

# read from launchpad and process data received

def processData(data):
    try:
        rightTicks = int(data["right_encoder"])
        leftTicks = int(data["left_encoder"])
    except:
        printError("couldn't recieve values for expected keys") 
        return
        
    print "right encoder: %d  left encoder: %d" % (rightTicks, leftTicks)

def parseLine(s):
    try :
        data = json.loads(s.strip())
    except:
        printError("problem while parsing JSON")
        return

    processData(data)

while True:
    connection.flushInput()
    line = connection.readline()

    if len(line) == 0:
        printError("ERROR: timeout")
    else:
        parseLine(line)

connection.close()

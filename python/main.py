#!/usr/bin/env python3
# -*- coding: utf-8 -*-
from http.server import HTTPServer, BaseHTTPRequestHandler
import json
import signal
import sys
import serial
import datetime
import time

__author__ = "Anthony Tournier, Bastian Schildknecht"
__version__ = "0.2.0"
__license__ = "MIT"

bombplanted = 0
livegame = 0
burnsendonce = 0
healthprev = 0

# Read configuration data from json file
print(str(datetime.datetime.now().strftime("%H:%M:%S.%f")[:-3]) + " Reading config...")
__config__ = {}
try:
    with open('config.json') as json_data_file:
        __config__ = json.load(json_data_file)
except Exception:
    print(str(datetime.datetime.now().strftime("%H:%M:%S.%f")[:-3]) + " Could not read config.json! Make sure it is correctly set up.")

# Connect to Arduino over specified serial port
print(str(datetime.datetime.now().strftime("%H:%M:%S.%f")[:-3]) + " Opening serial connection...")
try:
    __ser__ = serial.Serial(__config__['serial']['port'], __config__['serial']['baudrate'], timeout=1)
    print(str(datetime.datetime.now().strftime("%H:%M:%S.%f")[:-3]) + " Connected to Arduino on " + str(__config__['serial']['port']))
except Exception:
    print(" Could not connect! Check your config file and your Arduino connection.")

# Get game state value if available
def GetValue(obj, *args):
    argcount = len(args)
    if (argcount > 0):
        for arg in args:
            try:
                obj = obj[arg]
            except KeyError:
                return None
        return obj
    else:
        return None

# Signal handler to close serial port on interrupt
def signal_handler(signal, frame):
    serial_cmd = b"12;"
    try:
        __ser__.write(serial_cmd)
    except Exception:
        pass
    __ser__.close()
    sys.exit(0)

# Register signal handler for the signal SIGINT
signal.signal(signal.SIGINT, signal_handler)

# HTTP request handler for receiving game state updates over HTTP
class RequestHandler(BaseHTTPRequestHandler):
    _currentStatus = 0

# Send serial command
    def SendCmd(self, id):
        serial_cmd = (b"%d;") % (id)
        self.SerialSend(serial_cmd)
        # print(str(datetime.datetime.now().strftime("%H:%M:%S.%f")[:-3]) + " " + str(serial_cmd))

# Send serial command with an integer argument
    def SendCmdWithIntArg(self, id, arg):
        serial_cmd = (b"%d,%d;" % (id, arg))
        self.SerialSend(serial_cmd)
        # print(str(datetime.datetime.now().strftime("%H:%M:%S.%f")[:-3]) + " " + str(serial_cmd))

    def SerialSend(self, data):
        try:
            __ser__.write(data)
        except Exception:
            print(str(datetime.datetime.now().strftime("%H:%M:%S.%f")[:-3]) + " Could not send data to Arduino! Check your connection.")

# Player is in a menu
    def menu(self, serial_id = 1):
        self.SendCmd(serial_id)
        self._currentStatus = serial_id
        print(str(datetime.datetime.now().strftime("%H:%M:%S.%f")[:-3]) + " Player is in menu")

# Player is flashed
    def flashed(self, serial_id = 2, flashed = 0):
        self.SendCmdWithIntArg(serial_id, flashed)
        self._currentStatus = serial_id
        print(str(datetime.datetime.now().strftime("%H:%M:%S.%f")[:-3]) + " Player is flashed"  + str(flashed))
    
# Bomb has been planted
    def bomb(self, serial_id = 3):
        self.SendCmd(serial_id)
        self._currentStatus = serial_id
        bombplanted = 1
        print(str(datetime.datetime.now().strftime("%H:%M:%S.%f")[:-3]) + " Bomb has been planted")

# Bomb exploded
    def bombExploded(self, serial_id = 4):
        self.SendCmd(serial_id)
        self._currentStatus = serial_id
        print(str(datetime.datetime.now().strftime("%H:%M:%S.%f")[:-3]) + " Bomb exploded")
    
# Bomb was defused
    def bombDefused(self, serial_id = 5):
        self.SendCmd(serial_id)
        self._currentStatus = serial_id
        print(str(datetime.datetime.now().strftime("%H:%M:%S.%f")[:-3]) + " Bomb defused")

# Not known state, set default color
    def defaultColor(self, serial_id = 6):
        self.SendCmd(serial_id)
        self._currentStatus = serial_id
        print(str(datetime.datetime.now().strftime("%H:%M:%S.%f")[:-3]) + " Not known state")

    def tWins(self, serial_id = 7):
        self.SendCmd(serial_id)
        self._currentStatus = serial_id
        print(str(datetime.datetime.now().strftime("%H:%M:%S.%f")[:-3]) + " T Win")

    def ctWins(self, serial_id = 8):
        self.SendCmd(serial_id)
        self._currentStatus = serial_id
        print(str(datetime.datetime.now().strftime("%H:%M:%S.%f")[:-3]) + " CT Win")

    def burning(self, serial_id = 9, burning = 0):
        self.SendCmdWithIntArg(serial_id, burning)
        self._currentStatus = serial_id
        print(str(datetime.datetime.now().strftime("%H:%M:%S.%f")[:-3]) + " Player is burning")

    def freezetime(self, serial_id = 10):
        #logger.info('Freezetime')
        self.SendCmd(serial_id)
        self._currentStatus = serial_id
        bombplanted = 0
        print(str(datetime.datetime.now().strftime("%H:%M:%S.%f")[:-3]) + " Freeze time")

    def live(self, serial_id = 11):
        if (bombplanted == 0):
            #logger.info('Live')
            self.SendCmd(serial_id)
            self._currentStatus = serial_id
            print(str(datetime.datetime.now().strftime("%H:%M:%S.%f")[:-3]) + " Live")

# Player health update
    def health(self, serial_id = 13, health = 0):
        #logger.info('Health (0-100): %d' %(health))
        self.SendCmdWithIntArg(serial_id, health)
        self._currentStatus = serial_id
        print(str(datetime.datetime.now().strftime("%H:%M:%S.%f")[:-3]) + " Player health update " + str(health))

# Send HTTP response
    def sendResponse(self):
        response = bytes("RESPONSE", encoding="utf-8")
        self.send_response(200) 
        self.send_header("Content-Length", str(len(response)))
        self.end_headers()
        self.wfile.write(response)
    
# Receive game state updates over HTTP as JSON and process them
    def do_POST(self):
        # Get payload length
        length = int(self.headers['Content-Length'])
    
# Deserialize HTTP data in JSON format
        global bombplanted
        global livegame
        global burnsendonce
        global healthprev
        
        body = self.rfile.read(length).decode("utf-8")
        #logger.debug("Received data:\n" + body)
        # print(str(datetime.datetime.now().strftime("%H:%M:%S.%f")[:-3]) + " Received data:\n" + body)
        payload = json.loads(body)            
        
# Process data and send commands to Arduino over serial        
# Check if player is in menu or playing
        if GetValue(payload, 'player', 'activity') == 'menu':
            # Player is in menu
            self.menu()
        elif GetValue(payload, 'player', 'activity') == 'playing':
            # Player is playing
            # Check bomb state
            bomb = GetValue(payload, 'round', 'bomb')
            if (bomb == 'planted'):
                self.bomb()
                bombplanted = 1
            elif (bomb == 'exploded'):
                self.bombExploded()
            elif (bomb == 'defused'):
                self.bombDefused()
            else:
                # Bomb has not been planted yet
                # Check win conditions
                winTeam = GetValue(payload, 'round', 'win_team')
                if (winTeam == 'T'):
                    self.tWins()
                elif (winTeam == 'CT'):
                    self.ctWins()
                else:
                    # Check flashed state
                    flashed = GetValue(payload, 'player', 'state', 'flashed')
                    if (not flashed or flashed == 0):
                        # Player is not flashed
                        # Check burning state
                        burning = GetValue(payload, 'player', 'state', 'burning')
                        if (not burning or burning == 0):
                            # Player is not burning
                            if (burnsendonce == 0):
                                self.burning(burning=burning)
                                burnsendonce = 1
                        else:
                            # Player is burning
                            self.burning(burning=burning)
                            burnsendonce = 0
                    else:
                        # Player is flashed so override all colors with whiteout
                        self.flashed(flashed=flashed)
            phase = GetValue(payload, 'round', 'phase')
            if (phase == 'freezetime'):
                self.freezetime()
                bombplanted = 0
                livegame = 0
            elif (phase == 'live'):
                if (bombplanted == 0):
                    if (livegame == 0):
                        self.live()
                        livegame = 1
            health = GetValue(payload, 'player', 'state', 'health')
            if (health != healthprev):
                self.health(health=health)
                healthprev = health
        self.sendResponse()

    do_PUT = do_POST



# -------------------------
# Server Setup
# -------------------------
print(str(datetime.datetime.now().strftime("%H:%M:%S.%f")[:-3]) + " Setting up server...")

# Read port from config
port = int(__config__['csgogsi']['port'])

# Create HTTP request handler
handler = RequestHandler

# Create HTTP server on specified IP and port with the created request handler
server = HTTPServer((__config__['csgogsi']['ip'], port), handler)

# Start server
print(str(datetime.datetime.now().strftime("%H:%M:%S.%f")[:-3]) + " Starting server...")
print(str(datetime.datetime.now().strftime("%H:%M:%S.%f")[:-3]) + " Listening at " + str(__config__['csgogsi']['ip'] + ":" + str(port)) )
server.serve_forever()

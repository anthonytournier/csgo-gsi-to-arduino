#!/usr/bin/env python
# -*- coding: utf-8 -*-
from http.server import HTTPServer, BaseHTTPRequestHandler
import json
import signal
import sys
import serial
import logzero
import logging
from logzero import logger

__author__ = "Anthony Tournier, Bastian Schildknecht"
__version__ = "0.2.0"
__license__ = "MIT"

# Configure logger
# Set loglevel
"""
Levels are:
NOTSET:     0
DEBUG:      10
INFO:       20
WARNING:    30
ERROR:      40
CRITICAL:   50
"""
logzero.loglevel(20)

# Read configuration data from json file
logger.info("Reading config...")
__config__ = {}
try:
    with open('config.json') as json_data_file:
        __config__ = json.load(json_data_file)
except Exception:
    logger.error("Could not read config.json! Make sure it is correctly set up.")
    exit(1)

# Connect to Arduino over specified serial port
logger.info("Opening serial connection...")
try:
    __ser__ = serial.Serial(__config__['serial']['port'], __config__['serial']['baudrate'], timeout=1)
except Exception:
    logger.error("Could not connect! Check your config file and your Arduino connection.")
    exit(1)


# Get game state value if available
def GetValue(obj, *args):
    argcount = len(args)
    if (argcount > 0):
        for arg in args:
            try:
                obj = obj[arg]
            except KeyError:
                logger.debug("Key '" + arg + "' not found!")
                return None
        return obj
    else:
        return None

# Signal handler to close serial port on interrupt
def signal_handler(signal, frame):
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

    # Send serial command with an integer argument
    def SendCmdWithIntArg(self, id, arg):
        serial_cmd = (b"%d,%d;" % (id, arg))
        self.SerialSend(serial_cmd)

    def SerialSend(self, data):
        try:
            __ser__.write(data)
        except Exception:
            logger.error("Could not send data to Arduino! Check your connection.")

    # Player is in a menu
    def menu(self, serial_id = 1):
        logger.info('On menu')
        self.SendCmd(serial_id)
        self._currentStatus = serial_id

    # Playing/Spectating terrorists
    def teamT(self, serial_id = 2):
        logger.info('T team')
        self.SendCmd(serial_id)
        self._currentStatus = serial_id

    # Playing/Spectating counter-terrorism
    def teamCT(self, serial_id = 3):
        logger.info('CT team')
        self.SendCmd(serial_id)
        self._currentStatus = serial_id

    # Player is flashed
    def flashed(self, serial_id = 4, flashed = 0):
        logger.info('Flashed (0-255): %d' %(flashed))
        self.SendCmdWithIntArg(serial_id, flashed)
        self._currentStatus = serial_id
    
    # Bomb has been planted
    def bomb(self, serial_id = 5):
        logger.info('Bomb has been planted')
        self.SendCmd(serial_id)
        self._currentStatus = serial_id

    # Bomb exploded
    def bombExploded(self, serial_id = 6):
        logger.info('Bomb exploded')
        self.SendCmd(serial_id)
        self._currentStatus = serial_id
    
    # Bomb was defused
    def bombDefused(self, serial_id = 7):
        logger.info('Bomb defused')
        self.SendCmd(serial_id)
        self._currentStatus = serial_id

    # Not known state, set default color
    def defaultColor(self, serial_id = 8):
        logger.info('Default state')
        self.SendCmd(serial_id)
        self._currentStatus = serial_id

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
        body = self.rfile.read(length).decode("utf-8")
        logger.debug("Received data:\n" + body)
        payload = json.loads(body)

        # Process data and send commands to Arduino over serial
        # Check if player is in menu or playing
        if GetValue(payload, 'player', 'activity') == 'menu':
            # Player is in menu
            self.menu()
        elif GetValue(payload, 'player', 'activity') == 'playing':
            # Player is playing
            # Check flashed state
            flashed = GetValue(payload, 'player', 'state', 'flashed')
            if (not flashed or flashed == 0):
                # Player is not flashed
                # Check bomb state
                bomb = GetValue(payload, 'round', 'bomb')
                if (bomb == 'planted'):
                    # Bomb has been planted
                    self.bomb()
                elif (bomb == 'exploded'):
                    # Bomb exploded
                    self.bombExploded()
                elif (bomb == 'defused'):
                    # Bomb was defused
                    self.bombDefused()
                else:
                    # Bomb has not been planted yet
                    # Set team colors
                    team = GetValue(payload, 'player', 'team')
                    if (team == 'T'):
                        # Player plays terrorist
                        self.teamT()
                    elif (team == 'CT'):
                        # Player plays counter-terrorism
                        self.teamCT()
                    else:
                        # Not in a team currently so set menu color
                        self.menu()
            else:
                # Player is flashed so override all colors with whiteout
                self.flashed(flashed=flashed)
            
            
                
        self.sendResponse()

    do_PUT = do_POST



# -------------------------
# Server Setup
# -------------------------
logger.info("Setting up server...")

# Read port from config
port = int(__config__['csgogsi']['port'])

# Create HTTP request handler
handler = RequestHandler

# Create HTTP server on specified IP and port with the created request handler
server = HTTPServer((__config__['csgogsi']['ip'], port), handler)

# Start server
logger.info("Starting server...")
server.serve_forever()
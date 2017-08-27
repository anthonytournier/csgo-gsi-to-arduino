#!/usr/bin/env python
# -*- coding: utf-8 -*-
from BaseHTTPServer import HTTPServer, BaseHTTPRequestHandler
import json
import signal
import sys
import serial
from logzero import logger

__author__ = "Anthony Tournier"
__version__ = "0.1.0"
__license__ = "MIT"

__config__ = {}
with open('config.json') as json_data_file:
    __config__ = json.load(json_data_file)

__ser__ = serial.Serial(__config__['serial']['port'], __config__['serial']['baudrate'], timeout=1)

def isset(var):
    try:
        var
    except NameError:
        return False
    else:
        return True

def signal_handler(signal, frame):
    __ser__.close()
    sys.exit(0)

class RequestHandler(BaseHTTPRequestHandler):
    currentStatut = 0

    def menu(self, serial_id = 1):
        serial_cmd = ("%d;") % (serial_id)
        logger.info('On menu')
        __ser__.write(serial_cmd)
        self.currentStatut = serial_id
    def teamT(self, serial_id = 2):
        serial_cmd = ("%d;") % (serial_id)
        logger.info('T team')
        __ser__.write(serial_cmd)
        self.currentStatut = serial_id
    def teamCT(self, serial_id = 3):
        serial_cmd = ("%d;") % (serial_id)
        logger.info('CT team')
        __ser__.write(serial_cmd)
        self.currentStatut = serial_id
    def sendResponse(self):
        response = bytes("RESPONSE") 
        self.send_response(200) 
        self.send_header("Content-Length", str(len(response)))
        self.end_headers()
        self.wfile.write(response)
    def do_POST(self):
        request_headers = self.headers
        content_length = request_headers.getheaders('content-length')
        length = int(content_length[0]) if content_length else 0
        data = json.loads(self.rfile.read(length))
        logger.info(data)

        if data['player']['activity'] == 'menu':
            self.menu()
        if data['player']['activity'] == 'playing':
            if isset(data['player']['team']) and data['player']['team'] == 'T':
                self.teamT()
            else:
                self.teamCT()
                
        self.sendResponse()

    do_PUT = do_POST

Handler = RequestHandler

port = int(__config__['csgogsi']['port'])
signal.signal(signal.SIGINT, signal_handler)

server = HTTPServer((__config__['csgogsi']['ip'], port), Handler)

server.serve_forever()
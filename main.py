#!/usr/bin/env python
# -*- coding: utf-8 -*-
from BaseHTTPServer import HTTPServer, BaseHTTPRequestHandler
from logzero import logger
import json
import serial

__author__ = "Anthony Tournier"
__version__ = "0.1.0"
__license__ = "MIT"

__config__ = {}
with open('config.json') as json_data_file:
    __config__ = json.load(json_data_file)

__ser__ = serial.Serial(__config__['serial']['port'], __config__['serial']['baudrate'], timeout=1)

class RequestHandler(BaseHTTPRequestHandler):
    def do_POST(self):
        request_headers = self.headers
        content_length = request_headers.getheaders('content-length')
        length = int(content_length[0]) if content_length else 0
        data = self.rfile.read(length)
        logger.info('Payload received' + data)
        __ser__.write(data)
    do_PUT = do_POST

def main():
    logger.info('DATAS')
    port = int(__config__['csgogsi']['port'])
    logger.info('Listening on ' + __config__['csgogsi']['ip'] + ':' + __config__['csgogsi']['port'])
    server = HTTPServer((__config__['csgogsi']['ip'], port), RequestHandler)
    server.serve_forever()

if __name__ == "__main__":
    main()

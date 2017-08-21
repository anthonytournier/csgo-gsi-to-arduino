#!/usr/bin/env python
# -*- coding: utf-8 -*-
from BaseHTTPServer import HTTPServer, BaseHTTPRequestHandler
from logzero import logger
import json


__author__ = "Anthony Tournier"
__version__ = "0.1.0"
__license__ = "MIT"

config = {}
with open('config.json') as json_data_file:
    config = json.load(json_data_file)

class RequestHandler(BaseHTTPRequestHandler):

    def do_POST(self):        
        request_path = self.path
        request_headers = self.headers
        content_length = request_headers.getheaders('content-length')
        length = int(content_length[0]) if content_length else 0
        data = self.rfile.read(length)
        logger.info('Payload received' + data)
        # self.send_response(200)
    do_PUT = do_POST

def main():
    """ Maint input of app """
    logger.info('DATAS')
    port = int(config['csgogsi']['port'])
    logger.info('Listening on ' + config['csgogsi']['ip'] + ':' + config['csgogsi']['port'])
    server = HTTPServer((config['csgogsi']['ip'], port), RequestHandler)
    server.serve_forever()

if __name__ == "__main__":
    main()

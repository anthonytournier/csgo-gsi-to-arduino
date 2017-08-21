#!/usr/bin/env python
# -*- coding: utf-8 -*-
import json
from logzero import logger

__author__ = "Anthony Tournier"
__version__ = "0.1.0"
__license__ = "MIT"

__conf__ = {}
with open('config.json') as json_data_file:
    __conf__ = json.load(json_data_file)


def main():
    """ Maint input of app """
    logger.info("TODO")

if __name__ == "__main__":
    main()

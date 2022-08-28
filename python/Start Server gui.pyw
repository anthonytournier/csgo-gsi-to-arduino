#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import json
import sys
import serial
import datetime
import time
import os
from subprocess import Popen, PIPE, STDOUT
from threading import Thread
from tkinter import *
import tkinter.scrolledtext as scrolledtext
import tkinter as tk

data2 = ""

cmd = [sys.executable, "main.py"]

class StagelightGUI:
    _currentStatus = 0
    def __init__(self, root):
        """Start subprocess, make GUI widgets."""
        self.root = root

        # GUI BUttons
        self.b1 = tk.Button(root, text="Start Server", command=self.start, width=10, state=tk.DISABLED)
        self.b1.place(x=0, y=0)
        self.b2 = tk.Button(root, text="Stop Server", command=self.stop, width=10, state=tk.DISABLED)
        self.b2.place(x=80, y=0)
        self.b3 = tk.Button(root, text="Breath Red", command=self.red1, width=10, bg="red")
        self.b3.place(x=163, y=0)
        self.b4 = tk.Button(root, text="Breath Green", command=self.green1, width=10, bg="green")
        self.b4.place(x=243, y=0)
        self.b5 = tk.Button(root, text="Breath Blue", command=self.blue1, width=10, bg="blue", fg='white')
        self.b5.place(x=323, y=0)
        self.b6 = tk.Button(root, text="Exit", command=self.exit, width=10)
        self.b6.place(x=0, y=35)
        self.b7 = tk.Button(root, text="Multi Fade", command=self.multifade, width=10)
        self.b7.place(x=163, y=35)        
        self.b8 = tk.Button(root, text="All Off", command=self.alloff, width=10)
        self.b8.place(x=243, y=35)
        self.b9 = tk.Button(root, text="Disconnect", command=self.disconnect, width=10)
        self.b9.place(x=323, y=35)

        # Scrolling text area
        self.text = scrolledtext.ScrolledText(root, wrap=tk.WORD, width=44, height=12)
        self.text['font'] = ('consolas', '12')
        self.text['background'] = ('black')
        self.text['foreground'] = ('yellow')
        self.text.place(x=0, y=70)
    
    def disconnect(self):
        try:
            self.b7["state"] = "disabled"
            self.b8["state"] = "disabled"
            self.b9["state"] = "disabled"
            self.b1["state"] = "normal"
            self.b3["state"] = "disabled"
            self.b4["state"] = "disabled"
            self.b5["state"] = "disabled"
            __ser__.close()
            self.text.insert(tk.END,str(datetime.datetime.now().strftime("%H:%M:%S.%f")[:-3]) + " Serial Port Closed.\r\n")
            self.text.see(tk.END)   
        except Exception:
            self.text.insert(tk.END,str(datetime.datetime.now().strftime("%H:%M:%S.%f")[:-3]) + " Error closing Serial Port.\r\n")
            self.text.see(tk.END)               
    
    def SendCmd(self, id):
        serial_cmd = (b"%d;") % (id)
        self.SerialSend(serial_cmd)
    
    def SerialSend(self, data):
        try:
            __ser__.write(data)
        except Exception:
            notsent = str(datetime.datetime.now().strftime("%H:%M:%S.%f")[:-3]) + " Could not send data to Arduino! Check your connection."
            self.text.insert(tk.END,notsent +'\r\n', 'error')
            self.text.tag_config('error', foreground='red')
            self.text.see(tk.END)
    
    # Set RED breath color on
    def red1(self, serial_id = 20):
        self.text.insert(tk.END,str(datetime.datetime.now().strftime("%H:%M:%S.%f")[:-3]) + " Setting Red RGB\r\n")
        self.SendCmd(serial_id)
        self._currentStatus = serial_id
        self.text.see(tk.END)
    
    # Set GREEN breath color on
    def green1(self, serial_id = 21):
        self.text.insert(tk.END,str(datetime.datetime.now().strftime("%H:%M:%S.%f")[:-3]) + " Setting Green RGB\r\n")
        self.SendCmd(serial_id)
        self._currentStatus = serial_id
        self.text.see(tk.END)
    
    # Set Blue breath color on
    def blue1(self, serial_id = 22):
        self.text.insert(tk.END,str(datetime.datetime.now().strftime("%H:%M:%S.%f")[:-3]) + " Setting Blue RGB\r\n")
        self.SendCmd(serial_id)
        self._currentStatus = serial_id
        self.text.see(tk.END)
    
    # Set ALL LED OFF
    def alloff(self, serial_id = 23):
        self.text.insert(tk.END,str(datetime.datetime.now().strftime("%H:%M:%S.%f")[:-3]) + " All LED Off\r\n")
        self.SendCmd(serial_id)
        self._currentStatus = serial_id
        self.text.see(tk.END)
        
    # Set MultiColor fade
    def multifade(self, serial_id = 24):
        self.text.insert(tk.END,str(datetime.datetime.now().strftime("%H:%M:%S.%f")[:-3]) + " Set MultiColor fade\r\n")
        self.SendCmd(serial_id)
        self._currentStatus = serial_id
        self.text.see(tk.END)
            
    def read_output(self, pipe):
        """Read subprocess' output and store it in `self.stdout_data`."""
        while True:
            data = os.read(pipe.fileno(), 1 << 20)
            # Windows uses: "\r\n" instead of "\n" for new lines.
            # data = data.replace(b"\r\n", b"\n")
            if data:
                info("got: %r", data)
                self.text.insert(tk.END,str(data))
                self.text.see(tk.END)
            else:  # clean up
                info("eof")
                self.root.after(5000, self.stop) # stop in 5 seconds
                return None

    def start(self, starting=[]):
        self.b3["state"] = "disabled"
        self.b4["state"] = "disabled"
        self.b5["state"] = "disabled"
        self.b7["state"] = "disabled"
        self.b8["state"] = "disabled"
        self.b2["state"] = "normal"
        # start subprocess
        self.proc = Popen(cmd, stdout=PIPE, stderr=STDOUT, shell=False)
        # Create a buffer for the stdout
        self.stdout_data = ""
        # Create a new thread that will read stdout and write the data to
        # `self.stdout_buffer`
        thread = Thread(target=self.read_output, args=(self.proc.stdout, ))
        thread.start()
        self.b1["state"] = "disabled"

    def stop(self, stopping=[]):
        self.text.insert(tk.END,str(datetime.datetime.now().strftime("%H:%M:%S.%f")[:-3]) + " Stop command recieved\r\n")
        self.text.see(tk.END)
        """Stop subprocess."""
        if stopping:
            return # avoid killing subprocess more than once
        #stopping.append(True)
        info("stopping")
        self.text.insert(tk.END,str(datetime.datetime.now().strftime("%H:%M:%S.%f")[:-3]) + " Stopping\r\n")
        try:
            self.proc.terminate() # tell the subprocess to exit
        except Exception:
            pass
        # kill subprocess if it hasn't exited after a countdown
        def kill_after(countdown):
            try:
                if self.proc.poll() is None: # subprocess hasn't exited yet
                    countdown -= 3
                    if countdown < 0: # do kill
                        info("killing")
                        self.proc.kill() # more likely to kill on *nix
                    else:
                        self.root.after(1000, kill_after, countdown)
                        return # continue countdown in a second
                time.sleep(2) # waiting for subprocess serial to close propperly
            except Exception:
                pass
           # Read configuration data from json file, to reconnect to Arduino
           # Reconnecting causes a restart where all lights are set to off
           # print(str("Reading config..."))
            __config__ = {}
            try:
                with open('config.json') as json_data_file:
                    __config__ = json.load(json_data_file)
            except Exception:
                donothing = 1
                # print("Could not read config.json! Make sure it is correctly set up.")
                # exit(1)
            # Connect to Arduino over specified serial port
            self.text.insert(tk.END,str(datetime.datetime.now().strftime("%H:%M:%S.%f")[:-3]) + " Opening serial connection again to restart Arduino\r\n")
            try:
                __ser__ = serial.Serial(__config__['serial']['port'], __config__['serial']['baudrate'], timeout=1)
                self.text.insert(tk.END,str(datetime.datetime.now().strftime("%H:%M:%S.%f")[:-3]) + " Connected to Arduino via stop command.\r\n", 'error')
                self.text.tag_config('error', foreground='white')
                self.text.see(tk.END)
            except Exception:
                self.text.insert(tk.END,str(datetime.datetime.now().strftime("%H:%M:%S.%f")[:-3]) + " Could not connect! Check your config file and your Arduino connection.\r\n", 'error')
                self.text.tag_config('error', foreground='red')
                self.text.see(tk.END)
            time.sleep(2)
            try:
                self.proc.stdout.close()  # close fd
                self.proc.wait()          # wait for the subprocess' exit
            except Exception:
                pass
            try:
                __ser__.close()
            except Exception:
                donthing = 1
        self.b3["state"] = "normal"
        self.b4["state"] = "normal"
        self.b5["state"] = "normal"
        self.b7["state"] = "normal"
        self.b8["state"] = "normal"
        kill_after(countdown=5)
        
    def exit(self, exiting=[]):
        """Stop subprocess."""
        if exiting:
            return # avoid killing subprocess more than once
        exiting.append(True)
        self.text.insert(tk.END,str(datetime.datetime.now().strftime("%H:%M:%S.%f")[:-3]) + " Exiting.\r\n", 'error')
        self.text.tag_config('error', foreground='white')
        self.text.see(tk.END)
        try:
            self.proc.terminate() # tell the subprocess to exit
        except Exception:
            self.text.insert(tk.END,str(datetime.datetime.now().strftime("%H:%M:%S.%f")[:-3]) + " Sub process not found.\r\n", 'error')
            self.text.tag_config('error', foreground='red')
            self.text.see(tk.END)
            #pass
        # kill subprocess if it hasn't exited after a countdown
        def kill_after(countdown):
            try:
                if self.proc.poll() is None: # subprocess hasn't exited yet
                    countdown -= 3
                if countdown < 0: # do kill
                    info("killing")
                    self.proc.kill() # more likely to kill on *nix
                else:
                    self.root.after(1000, kill_after, countdown)
                    return # continue countdown in a second
            except Exception:
                pass
        time.sleep(2)  # waiting for subprocess serial to close propperly
                       # Read configuration data from json file, to reconnect to Arduino
                       # Reconnecting causes a restart where all lights are set to off
                       # print(str("Reading config..."))
        __config__ = {}
        try:
            with open('config.json') as json_data_file:
                __config__ = json.load(json_data_file)
        except Exception:
            self.text.insert(tk.END,str(datetime.datetime.now().strftime("%H:%M:%S.%f")[:-3]) + " Could not read config.json! Make sure it is correctly set up.\r\n", 'error')
            self.text.tag_config('error', foreground='red')
            self.text.see(tk.END)

        # Connect to Arduino over specified serial port
        # print("Opening serial connection...")
        self.text.insert(tk.END,str(datetime.datetime.now().strftime("%H:%M:%S.%f")[:-3]) + " Opening serial connection again to restart Arduino\r\n")
        self.text.see(tk.END)
        try:
            __ser__ = serial.Serial(__config__['serial']['port'], __config__['serial']['baudrate'], timeout=1)
            # print(" Connected to Arduino on ")
        except Exception:
            self.text.insert(tk.END,str(datetime.datetime.now().strftime("%H:%M:%S.%f")[:-3]) + " Could not connect! Check your config file and your Arduino connection.\r\n", 'error')
            self.text.tag_config('error', foreground='red')
            self.text.see(tk.END)
        
        time.sleep(2)
        try:
            self.proc.stdout.close()  # close fd
            self.proc.wait()          # wait for the subprocess' exit
        except Exception:
            pass
        
        try:
            __ser__.close()
        except Exception:
            pass

        self.root.destroy()       # exit GUI
            
root = tk.Tk()
root.iconbitmap('favicon.ico')
root.geometry('417x300')
root.title('CSGO Stage Light')
# root.configure(bg='#bababa')
root.configure(bg='black')
app = StagelightGUI(root)

a1 = (str(datetime.datetime.now().strftime("%H:%M:%S.%f")[:-3]) + " Reading config...")
app.text.insert(tk.END, a1 + "\r\n", 'error')
app.text.tag_config('error', foreground='white')
app.text.see(tk.END)
__config__ = {}
try:
    with open('config.json') as json_data_file:
        __config__ = json.load(json_data_file)
except Exception:
    a1 = str(datetime.datetime.now().strftime("%H:%M:%S.%f")[:-3]) + " Could not read config.json! Make sure it is correctly set up."
    app.text.insert(tk.END, a1 + "\r\n", 'error')
    app.text.tag_config('error', foreground='red')
    app.text.see(tk.END)

# Connect to Arduino over specified serial port
a1 = str(datetime.datetime.now().strftime("%H:%M:%S.%f")[:-3]) + " Opening serial connection..."
try:
    __ser__ = serial.Serial(__config__['serial']['port'], __config__['serial']['baudrate'], timeout=1)
    a1 = str(datetime.datetime.now().strftime("%H:%M:%S.%f")[:-3]) + " Connected to Arduino on " + str(__config__['serial']['port'])
    app.text.insert(tk.END, a1 + "\r\n", 'error')
    app.text.tag_config('error', foreground='white')
    app.text.see(tk.END)
except Exception:
    a1 = str(datetime.datetime.now().strftime("%H:%M:%S.%f")[:-3]) + " Could not connect! Check your config file and your Arduino connection."
    app.text.insert(tk.END, a1 + "\r\n", 'error')
    app.text.tag_config('error', foreground='red')
    app.text.see(tk.END)

root.protocol("WM_DELETE_WINDOW", app.stop) # exit subprocess if GUI is closed
root.mainloop()


import logging
import os
import sys
from subprocess import Popen, PIPE, STDOUT
from threading import Thread
from tkinter import *
import tkinter.scrolledtext as scrolledtext
import time
import json
import serial
import tkinter as tk

info = logging.getLogger(__name__).info
data2 = ""

# define dummy subprocess to generate some output
cmd = [sys.executable, "main.py"]

class ShowProcessOutputDemo:
    def __init__(self, root):
        """Start subprocess, make GUI widgets."""
        self.root = root

        # stop subprocess using a button
        tk.Button(root, text="Start Server", command=self.start).pack()
        tk.Button(root, text="Stop Server", command=self.stop).pack()
        #stopbtn.place(x=100, y=20)

        #self.text = tk.Text(root) # put subprocess output here

        self.text = scrolledtext.ScrolledText(root, undo=True)
        self.text['font'] = ('consolas', '10')
        self.text['background'] = ('green')
        self.text['foreground'] = ('yellow')
        self.text.pack(expand=True, fill='both')

    def read_output(self, pipe):
        global data2
        """Read subprocess' output and store it in `self.stdout_data`."""
        while True:
            data = os.read(pipe.fileno(), 1 << 20)
            # Windows uses: "\r\n" instead of "\n" for new lines.
            data = data.replace(b"\r\n", b"\n")
            if data:
                info("got: %r", data)
                #self.stdout_data += data.decode()
                data2 += data.decode()
                data3 = data2.replace('] "POST / HTTP/1.1" 200 -',"")
                data3 = data3.replace('127.0.0.1 - - [',"")
                self.text.insert(tk.END,data3)
                self.text.see(tk.END)
            else:  # clean up
                info("eof")
                self.root.after(5000, self.stop) # stop in 5 seconds
                return None

    def start(self, starting=[]):
        # start subprocess
        self.proc = Popen(cmd, stdout=PIPE, stderr=STDOUT, shell=False)
        # Create a buffer for the stdout
        self.stdout_data = ""
        # Create a new thread that will read stdout and write the data to
        # `self.stdout_buffer`
        thread = Thread(target=self.read_output, args=(self.proc.stdout, ))
        thread.start()

        # A tkinter loop that will show `self.stdout_data` on the screen
        # self.show_stdout()

    def stop(self, stopping=[]):
        """Stop subprocess and quit GUI."""
        self.text.insert(tk.END,"Stop COMMAND recieved.")
        if stopping:
            return # avoid killing subprocess more than once
        stopping.append(True)

        info("stopping")
        self.proc.terminate() # tell the subprocess to exit
    
        # kill subprocess if it hasn't exited after a countdown
        def kill_after(countdown):
            if self.proc.poll() is None: # subprocess hasn't exited yet
                countdown -= 3
                if countdown < 0: # do kill
                    info("killing")
                    self.proc.kill() # more likely to kill on *nix
                else:
                    self.root.after(1000, kill_after, countdown)
                    return # continue countdown in a second
            time.sleep(2) # waiting for subprocess serial to close propperly
           
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
            # print("Opening serial connection...")
            self.text.insert(tk.END,"Opening serial connection again to restart Arduino")
            try:
                __ser__ = serial.Serial(__config__['serial']['port'], __config__['serial']['baudrate'], timeout=1)
                # print(" Connected to Arduino on ")
            except Exception:
                donothing = 2
                # print(" Could not connect! Check your config file and your Arduino connection.")
                # exit(1)
            
            time.sleep(2)
            self.proc.stdout.close()  # close fd
            self.proc.wait()          # wait for the subprocess' exit
            self.root.destroy()       # exit GUI
            try:
                __ser__.close()
            except Exception:
                donthing = 1
        kill_after(countdown=5)

logging.basicConfig(level=logging.INFO, format="%(asctime)s %(message)s")
root = tk.Tk()
root.iconbitmap('favicon.ico')
root.geometry('650x200')
root.title('CSGO Stage Light')
app = ShowProcessOutputDemo(root)
root.protocol("WM_DELETE_WINDOW", app.stop) # exit subprocess if GUI is closed
root.mainloop()
info("exited")

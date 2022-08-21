# This is a fork
[Original](https://github.com/synyster0fa7x/csgo-gsi-to-arduino)

# CSGO: Game State Integration to Arduino

Thanks to http://embryonic.dk/wordpress/?p=867
for calculating the formula needed to sync the LED blink with the bomb beep.

"Shortly after Valve released the game state API, they turned down the accuracy of the bomb timer since it could have been used for cheating . You would have been replacing game sense (“how much time do I have left to defuse the bomb?”) with automation."

Not sure how true that statement is, but the bomb sync is out sometimes.

########################################################################


Send CSGO:GSI data from python to Arduino through serial communication

### Materials

* Arduino (Uno, Mega, Mini...)
* 3 resistors 100Ω
* 3 N-Channel MOSFETs / Transistors
* 12V LED Strip RGB 4 Pins
* I used 1K resistor with TIP41C transistor

## Installation

#### Windows installer

1. Download and install exe from https://github.com/Welsyntoffie/csgo-gsi-python-to-arduino/releases/tag/v1.0.0
2. Connect your Arduino to your PC via USB
3. Go to "Device Manager" and look under COM LPT for the COMport number of your Arduino
4. Navigate to `C:\Program Files(x86)\CSGO Stage light` and edit the `config.json` file at line 8 with the COM number you found from step 3
5. Navigate to `C:\Users\User\Documents\Arduino\CSGO_lighting_controller` and open `CSGO_lighting_controller.ino`
6. After the Arduino IDE loads, select your board from the Tools menu
7. Select your Port number from the tools menu (will be the same as in step 3)
8. Upload the sketch (check for any errors at the bottom)
9. Navigate to your dekstop. Open the file `CSGO Light Controller`

You should now see a CMD window with some text inside about starting server and connection status to Arduino. Check for any errors.

### Manual install

#### Prerequisites

```
Python 3
PIP
git
```

#### Arduino
My Arduino files are stored in tabs.

Upload the `arduino/CSGO_lighting_controller.ino` to your Arduino.

#### Python Server

##### Linux
Copy the repository URL using the green clone widget on the top right.
Then use the commands listed below to install the required software.
```
$ git clone <Copied repository URL>
$ cd csgo-gsi-to-arduino/python
$ python -m pip install -r requirements.txt
$ cp config.json.dist config.json
```

Fill `config.json` with your parameters:
Example:
```
{
    "csgogsi":{
        "ip":"localhost",
        "port":"61500",
        "secret":"MY_SECRET_PHRASE"
    },
    "serial":{
        "port": "/dev/ttyACM0",
        "baudrate": 9600
    }
}
```

Then launch the server with :
```
python ./main.py
```

The server is now listenning for POST request from CSGO and sends the data to your Arduino through serial COM.

#### Windows
Copy the repository URL using the green clone widget on the top right.
Then use the commands listed below to install the required software.
```
$ git clone <Copied repository URL>
$ cd csgo-gsi-to-arduino/python
$ python -m pip install -r requirements.txt
$ copy config.json.dist config.json
```

Fill `config.json` with your parameters:
Example:
```
{
    "csgogsi":{
        "ip":"localhost",
        "port":"61500",
        "secret":"MY_SECRET_PHRASE"
    },
    "serial":{
        "port": "COM3",
        "baudrate": 115200
    }
}
```

Then launch the server:
```
Navigate to where you have the python file, Hold left shift and right click on any white space, select open command prompt window here
In the CMD window type the below text...
python main.py
```

The server is now listenning for POST request from CSGO and sends the data to your Arduino through serial COM.


#### CS:GO Server

Copy the `.cfg` file into your CS:GO config's directory. This is mine for exemple : 

`C:\Program Files (x86)\Steam\steamapps\common\Counter-Strike Global Offensive\csgo\cfg\`

You can rename the last part of the config name to whatever you want to avoid conflicts with other GSI services.
Example name:
`gamestate_integration_stagelighting.cfg`

then restart CS:GO.

### Wiring

![csgo gsi led strip arduino](arduino/wiring.png)

## Built With

* [Python](https://www.python.org/)
* [CSGO:GSI](https://developer.valvesoftware.com/wiki/Counter-Strike:_Global_Offensive_Game_State_Integration) - CSGO: Game State Integration documentation

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details.

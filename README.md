# This is a fork
[Original](https://github.com/synyster0fa7x/csgo-gsi-to-arduino)

# CSGO: Game State Integration to Arduino

Thanks to http://embryonic.dk/wordpress/?p=867
for calculating the formula needed to sync the LED blink with the bomb beep

Send CSGO:GSI data from python to Arduino through serial communication

### Prerequisites

```
Python 3
PIP
git
```

### Materials

* Arduino (Uno, Mega, Mini...)
* 3 resistors 100Ω
* 3 N-Channel MOSFETs / Transistors
* 12V LED Strip RGB 4 Pins
* I used 1K resistor with TIP41C transistor

### Installation

#### Arduino

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
        "port":"3000",
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

The server is now listenning for POST request from CSGO and sends the data to your Arduino throught serial COM.

##### Windows
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
        "port":"3000",
        "secret":"MY_SECRET_PHRASE"
    },
    "serial":{
        "port": "COM3",
        "baudrate": 9600
    }
}
```

Then launch the server with:
```
Open CMD then type...
python main.py
```

The server is now listenning for POST request from CSGO and sends the data to your Arduino throught serial COM.


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

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
If all good your arduino will flash all the colors in sequence once. Red, Blue, Green (need to swap that)

### Manual install

#### Prerequisites

```
Python 3
PIP
git
```

#### Arduino
My Arduino files are stored in tabs.

Used libraries...
```
Arduino_JSON
CmdMessenger
Countimer-master
JLed
```
To install libraries, open Arduino IDE, select Sketch fromt the menu, Include Library , Manage Libraries.
In the search bar type the names as mentioned above and click install.

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

## Notes
The JLed library is used to create a breathing effect when in freezetime. If you want a solid color during freezetime then edit the arduino file in the main sketch, then upload the sketch again.

Set
> freezetime_breath = 0.

### Wiring

![csgo gsi led strip arduino](arduino/wiring.png)

# Features

* Round Live - default color Green (brightness will drop as your health drops)
* Freezetime - default color Blue with breathing effect (can be configured to be solid blue, see note above)
* Health at 0 - default color is Red (When you spectate a player, the strip will change to Green)
* Burning - default color is yellowish (Will keep this state while player is burning then switch back to live)
* Flashed - default color White (will fade away as the flash fades in game)
* Bomb planted - default color Red (flashing and gradually increasing in flash timing - Not perfect all the time though)
* Bomb exploded - default color rainbow fast flashing
* Bomb defused - default color Orange
* On Menu - default color Voilet
* T win - default color Yellow fading on and off
* CT Win - default color Blue fasing on and off

## Built With

* [Python](https://www.python.org/)
* [CSGO:GSI](https://developer.valvesoftware.com/wiki/Counter-Strike:_Global_Offensive_Game_State_Integration) - CSGO: Game State Integration documentation

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details.

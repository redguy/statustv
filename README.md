statustv
========


### Requirements

#### Arduino
* [IRLibNEC](https://github.com/redguy/IRLibNEC.git). If you have non-NEC compatible TV, you need [IRLib](https://github.com/cyborg5/IRLib.git) and some changes to the code. Note that the full version of the IRLib library does not fit together with my code into 16k Flash (ie. ATMega168 in Duemilanove).

##### OS X
* Perl modules `Device::SerialPort`, `Modern::Perl`, `Mac::AppleScript::Glue`. The last one required `-f` to install to ignore some failing tests on my box.
* `terminal-notifier`. Download binary from [GitHub repository](https://github.com/alloy/terminal-notifier/releases), unpack & copy to `/Applications`. 

#### Hardware
* IR LED 940nm, eg. LTE5208AC, connected from Arduino pin D3 to ground in series with appropriate resistor (~100 Ohm). Actually, I kinda forgot to connect the resistor on my prototype and it seemed to be working fine (probably as long as the LED is used only to send short bursts of 38kHz pulses).
* IR Receiver on Arduino pin D6, e.g. TSOP4838. Looking at the lenses, connect left pin directly to D6, middle one is GND and right one to +5V.





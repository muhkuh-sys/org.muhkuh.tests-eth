This project is based on the Hilscher Ethernet MAC. It can be downloaded here: https://kb.hilscher.com/display/HAL/Ethernet+MAC

TODO:

 * Deactivate the PHY/XC after the test.

 * Setup the PHY only once.
Currently the PHY is initialized for each interface. With dual PHYs on the netX90 and netX4000 the PHY is set up 2 times.

 * Implement the number of test packets for the echo client as a test parameter.
Currently this is hard coded in the function "echo_client_initialize".

 * Pass all parameters from the LUA test script.
Currently all parameters are set at compile time in "options_default.c".

 * netX90 needs a reset to work
The test does not run after a power-on. No packets are received. It works only if the software started once and reset is pressed.
It is not neccessary to establish a link. A simple reset without starting the software does not do the trick.

These are the example projects for EE-399

The folder contains LED blink projects for SHARC core 1 and SHARC core 2. The cache and parity has been disabled for the reasons stated in the EE-399.
The LED blink program reads the argument from the RSL, and blink the LED accordingly. It also contains code that loads two locations of a global buffer with 0xDEAD and 0xBEEF.  Besides verifying that the LEDs blink on the EZ-board, a user can load a “symbols only” session on CCES 
Arduino Code for Vernier Motion Detector
===============

These are a series of sketches to interface the Vernier Motion Detector directly 
through their BTD connector. This connector can be purchased directly through 
Vernier as Part Number BTD-ELV -- http://www.vernier.com/products/accessories/btd-elv/

SparkFun Electronics will be carrying a Vernier Sensor Shield soon. The release date is still
pending. In the meantime, this code can be tested and run using the BTD-ELV and an Arduino.


Repository Contents
-------------------

  * **/VernierMotionDetector/* - Arduino example code for interfacing the Vernier Motion Detector.
  This is code is kept as simple as possible. It uses a push button connected to digital Pin 12 to 
  start / stop the motion detector. If you're using the SparkFun Vernier Shield, this button is 
  already connected to Pin 12.  If you are using the BTD-ELV, you will need to connect a button to
  connect Digital Pin 12 to GND.  

  * **/VernierMotionDetectorSerialChart/* - Arduino example code to output a standard CSV style dataset
  to use with SerialChart. SerialChart is a simple open-source program that will graph data that is
  streamed to it through the Serial bus. You may have to modify the configuration file with the correct
  COM port for your arduino device.
  
  Go to https://code.google.com/p/serialchart/ to download SerialChart for your computer.
  
  * *arduino_sample.scc* -- This is the configuration file to use with Serial Chart.
  
* **/VernierMotionDetectorSDStore/* - Arduino example code for displaying AND saving data directly to 
  a micro SD Card using the SparkFun microSD Card Shield. 
  
  https://www.sparkfun.com/products/9802
  
  You will need to do a little soldering to put header pins on this, but it's super simple once you
  have the soldering done. Data is saved to a file called datalog.txt. This filename can be changed 
  in the code.

License Information
-------------------

All contents of this repository are released under [Creative Commons Share-alike 3.0](http://creativecommons.org/licenses/by-sa/3.0/).

Authors: Brian Huang @ SparkFun Electonics

/*
VernierMotionDetectorSDStore (v 2013.11)
 Takes data from a Vernier Motion Detector connected to BTD connector.
 
 This sketch measures the time taken for the ultrasound to return (in microseconds)
 and then calculates the corresponding distance (based on the speed of ultrasound
 in air) and displays the distance (in cm) on the Serial Monitor. 
 
 The data is displayed to the serial monitor and saved to a file called data_date.txt
 
 Here is how the Vernier Motion Detector works:
 - when pin 2 on BTD is pulled high, this triggers the ultrasound pulse
 - the program then starts timing but then delays 0.9 ms *(blanking time,
 0.9 seconds is the time it takes ultrasound to travel 15 cm twice (round trip))
 - the program then monitors pin 1 on the BTD, waiting for it to go high. 
 This happens when an echo is detected.
 
 Added a variable called dataRate. data Rate describes the # of samples per second.
 The delay at the end of the loop is equal to 1/dataRate and roughly controls the # 
 samples per second.
 
 See www.vernier.com/arduino for more information.
 */
#include <SD.h>      // includes the Arduino SD Library 
#define DELIMITER '\t'   // change this depending on your preferred output format

char * filename = "datalog.txt";  // sets the filename for data

int dataRate = 20;  // # of samples per second.

unsigned long time;     // clock reading in microseconds
unsigned long timeRef;  // reference for starting time
unsigned long Duration; // time it take echo to return
const float SpeedOfSound = 340; //in m/s
float Distance;// in centimeters
int val = 0;

unsigned int echoCounter = 0;

const int TriggerPin = 3; // trigger pin -- if Motion Detector is plugged into Digital 1
const int EchoPin = 2;    // echo pin --  -- if Motion Detector is plugged into Digital 1
/* If you plug the Motion Detector into Digital 2, then, TriggerPin = 7, EchoPin = 6 */

const int buttonPin = 12;
const int ledPin = 13;

/* Global Variable declarations for SD Card Shield */
const int chipSelect = 8;
File dataFile;

unsigned long timeInterval = 250000;
int mode = 0;
unsigned int ndx = 1;

void setup() 
{
  // initialize the Ping pin as an output:

  timeInterval = 1000000 / dataRate;

  pinMode(TriggerPin, OUTPUT);
  pinMode(EchoPin, INPUT); //this is the pin that goes high when an echo is received
  // initialize serial communication at 9600 bits per second:

  pinMode(buttonPin, INPUT_PULLUP);

  Serial.begin(9600);

  Serial.print("Initializing SD card...");
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:

  pinMode(chipSelect, OUTPUT);

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) 
  {
    Serial.println("Card failed, or not present.");
    // display message, but continue.
  }
  else
  {
     Serial.println("card initialized.");
  }
  
  Serial.println();
  Serial.println("*****************************************************");
  Serial.print("Data Collection Rate is set to: ");
  Serial.print(dataRate);
  Serial.println(" samples/sec.");
  Serial.println("Type any number in the Serial Monitor to change this.");

  Serial.println();
  Serial.println("Push button (D12) to start data collection.");
  Serial.println("Use reset button to reset / stop data collection.");
  Serial.println("*****************************************************");
}

void loop() 
{
  pinMode(12, INPUT_PULLUP);
  if (Serial.available() > 0)    // check for change in data rate
  {
    dataRate = Serial.parseInt();
    timeInterval = 1000000 / dataRate;

    mode = 0;
    ndx = 0;
    Serial.println("*****************************************************");
    Serial.print("Data Collection Rate is set to: ");
    Serial.print(dataRate);
    Serial.println(" samples/sec.");
    Serial.println("Type any number in the Serial Monitor to change this.");

    Serial.println();

    Serial.println("Push button (D12) to start data collection.");
    Serial.println("Use reset button to reset / stop data collection.");
    Serial.println("*****************************************************");   
  }

  int buttonPress = digitalRead(12);

  if (buttonPress == LOW)
  {
    if (mode == 0)
    {
      mode = 1;

      dataFile = SD.open(filename, FILE_WRITE);
      if (dataFile)
      {
        dataFile.println("Vernier Format 2");
        dataFile.println("Motion Detector Readings taken using Ardunio");
        dataFile.println("Data Set");
        dataFile.print("Time");
        dataFile.print("\t"); //tab character
        dataFile.println ("Distance"); //change to match sensor
        dataFile.print("seconds");
        dataFile.print("\t"); // tab character
        dataFile.println ("centimeters"); //change to match sensor        
        dataFile.close();
      }
      else  // if(datafile)
      {
        Serial.println("error opening file - continuing w/o SD card.");
        Serial.println();
      }
      
      Serial.println("Vernier Format 2");
      Serial.println("Motion Detector Readings taken using Ardunio");
      Serial.println("Data Set");
      Serial.print("Time");
      Serial.print("\t"); //tab character
      Serial.println ("Distance"); //change to match sensor
      Serial.print("seconds");
      Serial.print("\t"); // tab character
      Serial.println ("centimeters"); //change to match sensor
      timeRef = micros();  
      digitalWrite(ledPin, HIGH); // turn on LED 13 when taking data.

    }
  }  // if buttonPress = LOW

  if (mode == 1)
  {
    if ((micros()) >= ndx*timeInterval + timeRef)
    {
      ndx++;
      sendPulse();
      echoCounter = 0;

      while ((digitalRead(EchoPin) == LOW) && (echoCounter < 5000))  // waits for the echo or count to 20 for *time-out
      {
        echoCounter++;
      }

      Duration = micros() - time;
      /* The speed of sound is 340 m/s.
       The ultrasound travels out and back, so to find the distance of the
       object we take half of the distance traveled.*/

      Distance = Duration * SpeedOfSound / (2 * 10000) ;// note convert to cm
      dataFile = SD.open(filename, FILE_WRITE);
      // if the file is available, write to it:
      if (dataFile) 
      {
        dataFile.print((time - timeRef) / 1E6, 4);  // 4 decimal places
        dataFile.print("\t");
        dataFile.println(Distance, 3);
        dataFile.close();
      }  
      // if the file isn't open, pop up an error:
      else 
      {
        // if no file open
        
      } 
      Serial.print((time - timeRef) / 1E6, 2);  // 2 decimal places
      Serial.print(DELIMITER); // tab character
      Serial.println(Distance, 3);  // 3 decimal places
    }
  }
}

void sendPulse()
{
  digitalWrite(TriggerPin, LOW);
  delayMicroseconds(4000);
  digitalWrite(TriggerPin, HIGH); // start the ultrasound pulse
  time = micros(); //note time
  delayMicroseconds(900); //delay during the blanking time
}








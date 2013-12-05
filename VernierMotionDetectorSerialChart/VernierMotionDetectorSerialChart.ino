/*
VernierMotionDetectorSerialChart (v 2013.11)
 Takes data from a Vernier Motion Detector connected to BTD connector.
 
 This sketch measures the time taken for the ultrasound to return (in microseconds)
 and then calculates the corresponding distance (based on the speed of ultrasound
 in air) and displays the distance (in cm) on the Serial Monitor. 
 
 The data is displayed to the serial monitor as a CSV (Comma Separated Value) format
 for direct tie into SerialChart -- https://code.google.com/p/serialchart/
 
 SerialChart is an open-source graphiing program that pulls directly from a Serial COM port.
 
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

const char delimiter = ',';
int dataRate = 30;  // # of samples per second.

unsigned long time;     // clock reading in microseconds
unsigned long timeRef;  // reference for starting time
unsigned long Duration; // time it take echo to return
const float SpeedOfSound = 340; //in m/s
float Distance;// in centimeters
int val = 0;

unsigned int echoCounter = 0;

const int TriggerPin = 3; //trigger pin
const int EchoPin = 2;// echo pin
const int buttonPin = 12;
const int ledPin = 13;

unsigned long timeInterval = 250000;
int mode = 0;
unsigned long ndx = 1;

void setup() 
{
  // initialize the Ping pin as an output:

  timeInterval = 1000000 / dataRate;

  pinMode(TriggerPin, OUTPUT);
  pinMode(EchoPin, INPUT); //this is the pin that goes high when an echo is received
  // initialize serial communication at 9600 bits per second:

  pinMode(buttonPin, INPUT_PULLUP);

  Serial.begin(9600);
  Serial.println();
  Serial.println("**************************************************");
  Serial.println("Type 'go' in the Serial window to start collection");
  Serial.println("Use reset button to reset / stop data collection.");
  Serial.println("**************************************************");
}

void loop() 
{
  int numBytes = Serial.available();
//  Serial.print("bytes avail: ");
//  Serial.println(numBytes);
  if (numBytes > 0)    // check for change in data rate
  {
    char * dump;
    Serial.readBytes(dump, numBytes);  // clear the serial line.
    mode = 1;
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

      Serial.print((time - timeRef) / 1E6, 2);
      Serial.print(delimiter); // tab character
      Serial.println(Distance, 3);
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

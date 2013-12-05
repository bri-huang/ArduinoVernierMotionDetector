/*
VernierMotionDetector (v 2013.11)
 Takes data from a Vernier Motion Detector connected to BTD connector.
 
 This sketch measures the time taken for the ultrasound to return (in microseconds)
 and then calculates the corresponding distance (based on the speed of ultrasound
 in air) and displays the distance (in cm) on the Serial Monitor. 
 
 Here is how the Vernier Motion Detector works:
 - when pin 2 on BTD is pulled high, this triggers the ultrasound pulse
 - the program then starts timing but then delays 0.9 ms *(blanking time,
 0.9 milliseconds is the time it takes ultrasound to travel 15 cm twice (round trip))
 - the program then monitors pin 1 on the BTD, waiting for it to go high. 
 This happens when an echo is detected.
 
 As written, the reading will be displayed roughly every quarter of a second. 
 Change the delay at the end of the loop to change the rate.
 
 See www.vernier.com/arduino for more information.
 
 Modified by: B. Huang, SparkFun Electronics
 December 4, 2013
 
 Updated code to start only after the button is pressed. Unfortunately, pushing the 
 button again does not stop the data collection. 
 
 Elminated the use of delay() in the code and added a "time-out" feature in case an echo
 is not detected.
 
 */

const int TriggerPin = 3; // trigger pin -- if Motion Detector is plugged into Digital 1
const int EchoPin = 2;    // echo pin --  -- if Motion Detector is plugged into Digital 1
/* If you plug the Motion Detector into Digital 2, then, TriggerPin = 7, EchoPin = 6 */

int refreshRate = 10;  // # of samples per second -- change this to increase/decrease the rate
                       // of the motion detector

const int buttonPin = 12;
const int ledPin = 13;


unsigned long time;     // clock reading in microseconds
unsigned long timeRef;  // reference for starting time
unsigned long Duration; // time it take echo to return
const float SpeedOfSound = 340; //in m/s


unsigned long timeInterval;
float Distance;               // in centimeters
int val = 0;
int echoCounter;
int ndx;

int mode = 0;

void setup() 
{
  // initialize the Ping pin as an output:

  pinMode(TriggerPin, OUTPUT);
  pinMode(EchoPin, INPUT); //this is the pin that goes high when an echo is received
  // initialize serial communication at 9600 bits per second:

  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);

  Serial.begin(9600);
  Serial.println("*************************************************");
  Serial.println("Push button (D12) to start data collection.");
  Serial.println("Use reset button to reset / stop data collection.");
  Serial.println("*************************************************");

  timeInterval = 1000000 / refreshRate;

}
void loop() 
{
  int buttonPress = digitalRead(12);
  if (buttonPress == LOW)
  {
    if (mode == 0)
    {
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
      digitalWrite(ledPin, HIGH);
    }
    else 
    { 
      mode = 0; 
      digitalWrite(ledPin, LOW);  
    }  
  }
  while (digitalRead(12) == LOW); // hold until button is released.

  if (mode == 1)
  {

    if ((micros()) >= ndx*timeInterval + timeRef)
    {
      ndx++;
      sendPulse();
      echoCounter = 0;

      while ((digitalRead(EchoPin) == LOW) && (echoCounter < 5000))  // waits for the echo or count for *time-out
      {
        echoCounter++;
      }

      Duration = micros() - time;
      /* The speed of sound is 340 m/s.
       The ultrasound travels out and back, so to find the distance of the
       object we take half of the distance traveled.*/

      Distance = Duration * SpeedOfSound / 2 / 10000 ;// note convert to cm
      //  Serial.print(Duration);// print the time it took until the echo
      Serial.print((time - timeRef) / 1E6);
      Serial.print("\t"); // tab character
      Serial.println(Distance);
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








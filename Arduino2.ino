
#define trigPin 6                       //Define the HC-SE04 triger on pin 6 on the arduino
#define echoPin 5                       //Define the HC-SE04 echo on pin 5 on the arduino

#include<SoftwareSerial.h>
SoftwareSerial BT(10, 11); // Connect Tx to pin 10 and Rx to pin 11 of HC-05/HC-06
String readData;          // String for storing data send from the Bluetooth device
String temp;
int relay = 7;
bool listenToBluetooth = false;

int val2 = 0;


void setup()
{
  BT.begin(9600);
  Serial.begin(9600);
  pinMode(relay, OUTPUT);

  digitalWrite(relay, LOW);

  //Start the serial monitor
  pinMode(6, OUTPUT);           //set the trigpin to output
  pinMode(5, INPUT);            //set the echopin to input
}

void loop()

{

  int duration, distance;             //Define two intregers duration and distance to be used to save data
  digitalWrite(6, HIGH);        //write a digital high to the trigpin to send out the pulse
  delay(50);             //wait half a millisecond
  digitalWrite(6, LOW);         //turn off the trigpin
  duration = pulseIn(5, HIGH);  //measure the time using pulsein when the echo receives a signal set it to high
  distance = (duration / 2) / 29.1;   //distance is the duration divided by 2 becasue the signal traveled from the trigpin then back to the echo pin, then devide by 29.1 to convert to centimeters

  while (BT.available())
  {
    delay(50);
    char c = BT.read();
    readData += c;

    int idx = readData.indexOf("L");
    if (idx != -1) {
      readData = readData.substring(idx, 3);
    }
  }
  if (readData.length() > 0)
  {
    Serial.println(readData);
    if (readData == "H" || readData == "HIGH" || readData == "turn on" || readData == "switch on")
    {
      val2 = digitalRead(relay);
      if (val2 == 0)
      {
        digitalWrite(relay, HIGH);
        val2 = 1;
        Serial.println("I switched it on");
      }

      listenToBluetooth = true;
      delay(200);
    }
    else if (readData == "L" || readData ==  "LOW" ||  readData.equalsIgnoreCase("turn off") || readData == "switch off")
    {
      val2 = digitalRead(relay);
      if (val2 == 1)
      {
        digitalWrite(relay, LOW);
        val2 = 0;
        Serial.println("I switched it off");
      }
      //      else
      //      {
      //        digitalWrite(relay, HIGH);
      //        val2 = 0;
      //      }

      listenToBluetooth = true;
      delay(200);
    }
    readData = "";
  }

  Serial.println(distance);
  while (distance > 0 )
  {
    if (listenToBluetooth) {
      break;
    }
    if (distance < 200 )
    {
      digitalWrite(7, HIGH);
    }

    else
    {
      digitalWrite(7, LOW);
    }
    Serial.println(distance);             //Dispaly the distance on the serial monitor
    //      Serial.println(" CM");              //in centimeters
    delay(100);
    break;//execute the Light subroutine below

  }

  //  listenToBluetooth = false;

  //delay half a second
}




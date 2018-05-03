
/*
 * Snippets of code here have been taken from both the GSM Shield Examples and the PulseSensor Playground Examples. Both can be found at the links below.
 * https://github.com/WorldFamousElectronics/PulseSensorPlayground
 * https://github.com/arduino-libraries/GSM
 */

#include <PulseSensorPlayground.h>
#include <GSM.h>


volatile int BPM;
char c = 'H';
volatile boolean QS = false;
volatile int IBI = 600;
volatile int Signal;
volatile boolean Pulse = false;
int redPin = 10;
int greenPin = 9;
int pulsePin = 4;
int blinkPin = 11;
boolean numberEntered = false;
GSM gsmAccess;
GSM_SMS hrsms;


extern void interruptSetup();
extern void healthCheck();

void setup() {

Serial.begin(115200);
interruptSetup();
pinMode(redPin, OUTPUT);       
pinMode(greenPin, OUTPUT);

boolean notConnected = true;
while (notConnected) {
    if (gsmAccess.begin() == GSM_READY) {
      notConnected = false;
    } else {
      Serial.println("No Connection Made With GSM Shield");
      delay(1000);
    }
  }

  Serial.println("GSM initialized");
}

void loop(){
String firstPart = "Your Heart Rate Is: ";
firstPart += BPM;
char txtmsg[50];
firstPart.toCharArray(txtmsg, 50);

   if (QS == false) {
    Serial.println("No heartrate found");
    
   }
  if (QS == true) {
        healthCheck();
   
    Serial.print("Your Current Heart Rate Is: ");            
    Serial.println(BPM);
  
     Serial.print("Enter a mobile number: ");
  char mobNumber[20];  // telephone number to send sms
  readSerial(mobNumber);
  Serial.println(mobNumber);  
     hrsms.beginSMS(mobNumber);
         //Serial.println("got to phone number");
  hrsms.print(txtmsg);
      //Serial.println("sent the bpm");
      Serial.println(txtmsg);
  hrsms.endSMS();     
    QS = false;
  
   }
  

  delay(30000);
}
void healthCheck() {
if (BPM >= 100) {
      digitalWrite(10, HIGH);    
      digitalWrite(9, LOW);
      Serial.print("Trying to turn high");                     
    } else {
      if (BPM < 100) {
        digitalWrite(10, LOW);
        digitalWrite(9, HIGH);  
         Serial.print("Trying to turn low");       
      }
    }
}

/*
 * The below comes from the GSM Shield Examples and is used to read the users mobile number and make use of it.
 * It can be found at the link below.
 * https://github.com/arduino-libraries/GSM/blob/master/examples/SendSMS/SendSMS.ino
 */
int readSerial(char result[]) {
  int i = 0;
  while (1) {
    while (Serial.available() > 0) {
      char inChar = Serial.read();
      if (inChar == '\n') {
        result[i] = '\0';
        Serial.flush();
        return 0;
      }
      if (inChar != '\r') {
        result[i] = inChar;
        i++;
      }
    }
  }
}

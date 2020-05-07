 #include <ESP32Servo.h>
#include <HardwareSerial.h>
#include <WiFi.h>
#include "time.h"
#include <EEPROM.h>
#define EEPROM_SIZE 10 //0 is openHour // 1 is openMin // 2 is closeHour // 3 is closeMin // 4 is currentValue
#define RXD2 16 
#define TXD2 17
#define timeSeconds 5

////////////////////////////////          PIN DECLARATIONS           //////////////////////////////////
const int lightPin     = 15;                              // LDR Pin       10K-resistor
const int motionSensor = 34;                              // Motion Sensor Pin  (Connected to 3V pin)
const int motorPin     = 27;                              // Servo Motor Pin
///////////////////////////////            MOTOR CONTROL             /////////////////////////////////
Servo myservo;                                            // Create SERVO          Object
unsigned int stepValue = 5;                               // ~5     Degrees a      Step
unsigned int currentValue;                                // Value  Written to     motor
unsigned int stepCount = 0;                               // 0-10   Keep    blinds in  Turn Range
///////////////////////////////            BLUETOOTH INPUTS          /////////////////////////////////
char input;                                              // Bluetooth  mode input
char STEP;                                                // Manual mode    input  '+' and  '-'
char lightLevel;                                          // LightLevel mode input
/////////////////////////////////////////////////////////////////////////////////////////////////////
const char* ssid       = "Complete";                      // WiFi   Network Name
const char* password   = "samir2019";                     // WiFi   Network Password
const char* ntpServer = "north-america.pool.ntp.org";     // NTP    Server  URL
const long  gmtOffset_sec = -28800;                       // Value  for     Pacific Zone
const int   daylightOffset_sec = 3600;                    // Adjust Time    for DLS(+1hr)
unsigned int hour;                                        // Hour   from    NTP capture
unsigned int minute;                                      // Minute from    NTP capture
/////////////////////////////////////////////////////////////////////////////////////////////////////
int ind1;
int ind2;
int ind3;
int ind4;
String openTime;
String openHour;                                          // Open  Hour   Input 00-23
String openMin;                                           // Open  Minute Input 00-59
String closeTime;
String closeHour;                                         // Close Hour   Input 00-23
String closeMin;                                          // Close Minute Input 00-59
                                                          // Convert String to Integer for Comparison
unsigned int O_hourInt;                                   // Open  Hour   Integer 0-23
unsigned int O_minInt;                                    // Open  Minute Integer 0-59
unsigned int C_hourInt;                                   // Close Hour   Integer 0-23
unsigned int C_minInt;                                    // Close Minute Integer 0-59
/////////////////////////////////////////////////////////////////////////////////////////////////////
char openClose;                                           // MIGHT DELETE///////////////////////////////////////////////
bool minMatched = false;                                  // MIGHT DELETE///////////////////////////////////////////////
bool hourWrong = false;                                   // MIGHT DELETE///////////////////////////////////////////////
unsigned long hourTimeLoop;                               // MIGHT DELETE///////////////////////////////////////////////
unsigned long hourTimeMain;                               // MIGHT DELETE/////////////////////////////////////////////// 
float hourCount = 0;                                      // MIGHT DELETE///////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
                                                          // PiR    Interrupt   Variables
unsigned long now = millis();                             // Holds  RunTime of  ESP32
unsigned long lastTrigger = 0;                            // Holds  Time        when Motion Detected
boolean startTimer = false;                               // Start  Timer  Flag when Motion Detected
/////////////////////////////////////////////////////////////////////////////////////////////////////
                                                          // calValue() smoothSetup() calSetup()
int  j,k = 0;                                             // Motor Move Lower and Upper Bounds
int increment = 0;                                        // For   motorMove() for LDR
                                                          /////////////////// LDR Smoothing Variables
const int numReadings = 10;                               // Number    of    LDR readings
int readings[numReadings];                                // Array[10] from  sensorValue
int readIndex = 0;                                        // Index     of    the   current reading
int total = 0;                                            // Static    total of    10 LDR Readings
int average = 0;                                          // Average   LDR   value of 10 Readings
                                                          /////////////////// LDR Calibrate Variables
int sensorValue = 0;                                      // The Sensor Value on lightPin(15)
int sensorMin = 0;                                        // Temporary Minimum LDR value
int sensorMax = 2300;                                     // Temporary Max     LDR value
/////////////////////////////////// END OF DECLARATIONS //////////////////////////////////////////////

int threshold = 40;
bool touch1detected = false;
bool touch2detected = false;

/////////////////////// Motion was detected -> Closes Blinds -> Starts Timer ///////////////////////
/*
void IRAM_ATTR detectsMovement() {
                                                          // digitalWrite(led,HIGH);
  //Serial2.println("Blinds close");                      // Notify User of Detection
  myservo.write(0);                                       // Close Blinds
  startTimer = true;                                      // Start Detection Timer Flag
  lastTrigger = millis();                                 // Start Detection Timer  
  return;                                                 // Return to prior function
                                 }                        // End of PiR interrupt   
                                 */
///////////////////////////         END OF PiR INTERRUPT         ////////////////////////////////////

/////////////////////// Get Initial Time from NTP during Setup() ////////////////////////////////////
void getInitTime(){
  struct tm timeinfo;
    if(!getLocalTime(&timeinfo)){                         // If NTP could Not be Retrieved
                      return;   }                         // end of NTP failure
     hour = timeinfo.tm_hour;                             // Store Hour from Time Structure via NTP
     minute = timeinfo.tm_min;                            // Store Minute from Time Structure via NTP
  return;                                                 // Return to SetUp()
                  }                                       // End of Initial Time function
/////////////////////// Check if User Open/Close Time matches NTP Time //////////////////////////////
void scheduling(){
  struct tm timeinfo;
    if(!getLocalTime(&timeinfo)){                         // If NTP could Not be Retrieved
                      return;   }                         // End of NTP failure
  hour = timeinfo.tm_hour;                                // Store NTP Hour   to hour   variable
  minute = timeinfo.tm_min;                               // Store NTP Minute to minute variable
  //
  if(O_hourInt == hour){                                  // User Open Hour Matches NTP hour
    if(O_minInt == minute){                               // User Open Minute Matches NTP minute
      currentValue = 30;                                  // Set currentValue to 30 for Blind Open
      servoSweep(currentValue);                           // Write currentValue(30) to Motor
      EEPROM.write(4,currentValue);                       // Update Motor Position to Flash Memory
      EEPROM.commit(); 
      stepCount = 6;                                      // Update Manual Mode Counter 
      O_minInt = O_minInt-1;                              // Break out of loop 
                        return;                           // Return to Main                                                 
                          }                               // End of Open Minute check
                        }                                 // End of Open Hour  check
  if(C_hourInt == hour){                                  // User Close Hour Matches NTP hour
     if(C_minInt == minute){                              // User Close Minute Matches NTP minute
      currentValue = 0;                                   // Set currentValue to 0 for Blind Close
      servoSweep(currentValue);                           // Write currentValue(0) to Motor
      EEPROM.write(4,currentValue);                       // Update Motor Position to Flash Memory
      EEPROM.commit(); 
      stepCount = 0;                                      // Update Manual Mode Counter
      C_minInt = C_minInt-1;                              // Break out of loop 
                        return;                           // Return to Main
                            }                             // End of Close Minute Check
                        }                                 // End of Close Hour   Check
   else{ return;}                                         // Return Main if Hours Not Matching
                }                                         // End of Scheduling function
////////////////////////////         END OF SCHEDULING        ///////////////////////////////////////

void gotTouch1(){
 touch1detected = true;
}

void gotTouch2(){
 touch2detected = true;
}

/////////////////////////////////             SETUP            //////////////////////////////////////
void setup(){
  smoothSetup();                                          // Run Smoothing LDR function
  calSetup();                                             // Run Calibrate LDR function
  Serial.begin(115200);                                   // Serial monitor baud rate
                                                          // Bluetooth init
  Serial2.begin(9600,SERIAL_8N1,RXD2,TXD2);             // Set HM-10 to 115200 baud, 8Bits, 
                                                          // No Parity, 1 Stop on Pin 16(RX),17(TX)
  
  EEPROM.begin(EEPROM_SIZE);                              // 10 bytes of storage
  currentValue = EEPROM.read(4);                          // Read from EEPROM for previous currentValue  
  O_hourInt = EEPROM.read(0);  
  O_minInt = EEPROM.read(1); 
  C_hourInt = EEPROM.read(2);
  C_minInt = EEPROM.read(3);    
                                                          // Motor Init 
  myservo.attach(motorPin);                               // Attach servo on pin 27 to SERVO object
  //servoSweep(0);                                        // Initialize servo at 0(closed)
  servoSweep(currentValue);                               // Init servo at previous value b4 power off
                                                          // WiFi Init
  WiFi.begin(ssid, password);                             // Init WiFi with SSID and Password
  while (WiFi.status() != WL_CONNECTED) {
  delay(500);  }// Serial2.println(" CONNECTED"); // Can remove once APP done
                                                          // Config time Variables and get Time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  getInitTime();                                          // If Scheduling() Called -> ERRORS
                                                          
  WiFi.disconnect(true); WiFi.mode(WIFI_OFF);             // Disconnect WiFi -> no longer needed
                                                          // PiR Interrupt Set-Up
 //pinMode(motionSensor, INPUT_PULLUP);                 
 //attachInterrupt(digitalPinToInterrupt(motionSensor), detectsMovement, RISING);  //LOW to HIGH
                                                          // LDR init

  
  touchAttachInterrupt(T0, gotTouch1, threshold);         
  touchAttachInterrupt(T6, gotTouch2, threshold);
  
  
 
           }                                              // End of Setup()
////////////////////////////////      END OF SETUP            ///////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////             MAIN FUNCTION                  ///////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
void loop() {
  //now = millis();                                         // Updated with current time every loop run
  
  if(touch1detected && (currentValue > 0)){
   currentValue = currentValue-1;
   delay(50);
    if(currentValue> 0 || currentValue<=100){
        stepCount = currentValue/5;
        //Serial2.print("STEPCOUNT: ");
        //Serial2.println(stepCount);
       // Serial2.print(" Motor AT : ");
        //Serial2.println(currentValue);
        myservo.write(currentValue);    
                      }
    if(currentValue<=0){
      currentValue = 0;
      //Serial2.println("Min Down");
                      }
   touch1detected = false;
                              }
                              
if(touch2detected && (currentValue <= 100)){
    currentValue = currentValue + 1;
    delay(50);
    if(currentValue <= 100){
      stepCount = currentValue/5;
      //Serial2.print("STEPCOUNT: ");
      //Serial2.println(stepCount);
      //Serial2.print(" Motor AT : ");
      //Serial2.println(currentValue);
      myservo.write(currentValue);    
    }
    if(currentValue > 100){
      currentValue = 100;
      //Serial2.println("Max Up");
    }
    touch2detected = false;
 
}
   
   /*                                                       
  if(startTimer && (now-lastTrigger>(timeSeconds*1000))){ // Open Blinds after the number of seconds 
                                                          // defined in the timeSeconds variable
    myservo.write(currentValue);                          // Write Previous Motor Value 
                                                          // digitalWrite(led, LOW);
                                                          // Serial.print("trigger: ");
                                                          // Serial.println(lastTrigger);
    //Serial2.println("Blinds open");
    startTimer = false;                                   // Set Start Timer Flag low
                                                         }//end of time check
 */
  bluetooth();                                            // Call Bluetooth Function
  scheduling();                                           // Call Scheduling Function
            }                                             // End of MAIN loop()
/////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////           END OF MAIN FUNCTION             ///////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////       Bluetooth Function      ///////////////////////////////////////
void bluetooth(){
    if(Serial2.available()) {                             // New Char came from HM-10
      while(1){
        input = Serial2.read();                           // Read BT input
         if(input == 'm'){                                
            manualMode();                                 // Call manualMode() if 'm' is received
                         }                                // End of Manual Mode call
         if(input == 'l'){
            lightLevelMode();                             // Call lightLevelMode() if 'l' is received
                         }                        // End of lightLevel Mode call
         if(input == 's'){
            schedulingMode();                             // Call schedulingMode() if 's' is received
                         }                                // End of scheduling Mode call
         if(input == 'C'){
            //cameraMode();                                 // Call cameraMode() if 'C' is received
                         }                                // End of camera Mode call                     
          else return;                                    // Return to Main
              }                                           // End of WHILE(1)
                           }                              // End of Serial2.available()   
}                                                         // End of bluetooth() function
///////////////////////////////       END of Bluetooth Function      /////////////////////////////////


/////////////////////////////////         MANUAL MODE        /////////////////////////////////////////
void manualMode(){
    //Serial2.println("In Manual Mode, press q to return to menu");
    if(stepCount == 22){                                  // Keep Blinds under Turning Max
      //Serial2.println("At max value position, press '-' to decrease position"); 
    }                                                     
    if(stepCount == 0){                                   // Keep Blinds over Turning Min
      //Serial2.println("At min value position, press '+' to increase position");
      goto readingManual;                                 // Jumps to Read Input
    }
   if (Serial2.available()) {                             // Data came across serial              
    readingManual:                                        // Jump Start
      while(1) {                                          // Loop until '+' or '-' is received
        
          // if(startTimer) {return;}
          if((touch2detected)||(touch1detected)){return;    }          // PiR Flag Return to Main to Service Interrupt
                                   // End of PiR Flag check
             
          STEP = Serial2.read();                          // BT read from HM-10
          if(stepCount < 22){                             // If Motor Value is less than 60
            if(STEP == '+'){                              // If char received is '+'
              stepCount +=1;                              // Increment Motor position counter
              break;                                      // Leave WHILE(1) to move motor
                           }                              // End of positive step tracker
                            }                             // End of positive step loop
          if(stepCount > 0) {                             // If Motor Value is greater than 0
            if(STEP == '-'){                              // If char received is '-'  
              stepCount -= 1;                             // Decrement Motor position counter 
              break;                                      // Leave WHILE(1) to move motor
                           }                              // End of negative step tracker
                            }                             // End of negative step loop
          if(STEP == 'q') { return;}                      // Return to Main
              }                                           // End of WHILE(1)
      currentValue = stepCount * stepValue;               // Product of StepCount and StepValue
      servoSweep(currentValue);                           // Call function to move motor
      EEPROM.write(4,currentValue);
      EEPROM.commit(); 
      manualMode();                                       // Call Function for another Movement
  }                                                       // End of reading from Serial2(HM-10)  
}                                                         // End of manualMode()
/////////////////////////////////         END OF MANUAL MODE       ///////////////////////////////////


////////////////////////////////          lightLevel MODE         ////////////////////////////////////
void lightLevelMode(){
   calValue();                                            // Get Value from LDR fucntion
   if (Serial2.available()){    
     lightLevelInput:                                     // Jump Start

     while(1){
        lightLevel = Serial2.read();                      // Read lightLevel Input
        if(lightLevel == '2'){                            // 20% Light loop

            motorMove(60,80);                             // call LDR motor function with 20% range                           
            goto lightLevelInput;                         // Jump to Read Input
                             }                            // End of 20% loop
        if(lightLevel == '4'){                            // 40% Light loop
            motorMove(110,130);                           // call LDR motor function with 40% range  
            goto lightLevelInput;                         // Jump to Read Input
                             }                            // End of 40% loop 
        if(lightLevel == '6'){                            // 60% Light loop
            motorMove(160,180);                           // call LDR motor function with 60% range
            goto lightLevelInput;                         // Jump to Read Input
                             }                            // End of 60% loop
        if(lightLevel == '8'){                            // 80% Light loop                                            
            motorMove(210,240);                           // call LDR motor function with 80% range
            goto lightLevelInput;                         // Jump to Read Input
                             }                            // End of 80% loop 
        if(lightLevel == 'q'){ return; }         // Print Menu and Return to Main 
             }                                            // End of WHILE(1)  
                           }                              // End of reading from Serial2(Bluetooth)
                      }                                   // End of lightLevelMode()
////////////////////////////            END of lightLevel MODE         ///////////////////////////////


////////////////////////////              schedulingMode              ///////////////////////////////
void schedulingMode(){
  if(Serial2.available()){
    while(1){
      openClose = Serial2.read();
      if(openClose == 'o'){
        break; }
      if(openClose == 'c'){
        break; }
      if(openClose == 'b'){
        break; }
    }//end of while(1) for open or close or both validation and input control
    if((openClose == 'o') || (openClose == 'b')){
    while(1){
      openTime = Serial2.readStringUntil('\r');           // Read Until Carriage Return Char
      ind1 = openTime.indexOf(':');                       // Store index up to the ':' char
      openHour = openTime.substring(0,ind1);              // Read Hour up to the ':' char 
      ind2 = openTime.indexOf('\r');                      // Store index up to the '/r' char
      openMin = openTime.substring(ind1+1,ind2);          // Read min up to the '/r' char 
      if(openHour.length() == 2){                         // Input Validation
         O_hourInt = openHour.toInt();                    // Convert String to Integer
         EEPROM.write(0,O_hourInt); EEPROM.commit();      // Store Open Hour to Flash Memory
        break;                  }                         // Break out Input Length Check
            }                                             // End of while(1) for open hour
    while(1){
      if(openMin.length() <= 2){                          // Input Validation
         O_minInt = openMin.toInt();                      // Convert String to Integer
         EEPROM.write(1,O_minInt); EEPROM.commit();       // Store Open Min to Flash Memory 
        break;                 }                          // Break out Input Length Check
            }                                             // End of while(1) for open minute
                                                        } //END OF O or B            
    if((openClose == 'c') || (openClose == 'b')){
    while(1){
      closeTime = Serial2.readStringUntil('\r');          // Read Until Carriage Return Char
      ind3 = closeTime.indexOf(':');                      // Store index up to the ':' char
      closeHour = closeTime.substring(0,ind3);            // Read Hour up to the ':' char 
      ind4 = closeTime.indexOf('\r');                     // Store index up to the '/r' char
      closeMin = closeTime.substring(ind3+1,ind4);        // Read min up to the '/r' char 
      if(closeHour.length() == 2){                        // Input Validation
         C_hourInt = closeHour.toInt();                   // Convert String to Integer
         EEPROM.write(2,C_hourInt); EEPROM.commit();      // Store Close Hour to Flash Memory
         break;                  }                        // Break out Input Length Check
            }                                             // End of while(1) for close hour
    while(1){
      if(closeMin.length() <= 2){                         // Input Validation
         C_minInt = closeMin.toInt();                     // Convert String to Integer
         EEPROM.write(3,C_minInt); EEPROM.commit();       // Store Close Min to Flash Memory 
         break;                 }                         // Break out Input Length Check
            }                                             // End of while(1) for close hour
                                                        } // END OF C or B 
return;                                                   // Return to Main
                        }                                 // End of reading from Serial2
                     }                                    // End of schedulingMode()
//////////////////////////////          END of schedulingMode()       //////////////////////////////

////////////////////////////////          servoSweep         ////////////////////////////////////////
void servoSweep(int x){
  unsigned int y = x;                                     // Assign X(currentValue) to local variable
  myservo.write(y);                                       // Move motor to Y
                                                          //Serial2.print("Just Moved to: ");
                                                          //Serial2.println(y); 
  return;                                                 // Return to Previous Function
}                                                         // End of servoSweep()
//////////////////////////////////       END of servoSweep     //////////////////////////////////////

/////////////////////////////             motorMove                  ////////////////////////////////
void motorMove(int j,int k){
  calValue();                                             // Call LDR function to get Value
  while(sensorValue > k){                                 // While sensorValue > k
    if(increment < 0){                                    // ADD HEREEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE
      goto Between;    }                                  // ADD HEREEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE                       
    else{                                                 // ADD HEREEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE
    increment = increment - 3;                            // Decrement motor value
                                                          Serial2.print("Moved by :  ");
                                                         Serial2.println(increment);
                                                         Serial2.print("Sensor Value: ");
                                                          Serial2.println(sensorValue);
    myservo.write(increment);                             // Move Motor with adjustment value
    delay(1000);                                          // 1 second delay
    calValue();                                           // Call LDR function to get Value
    if(sensorValue <= k){                                 // If sensorValue <=k
                                                          Serial2.println("Breaking out of closing too bright");
      goto Between;                                       // ADDED HEREEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE
                        }                                 // End of If(sensorValue<=k)
        }                                                 // End of Else
                       }                                  // End of While(sensorValue>k)                                                  

Between:                                                  // Jump Start
  while(sensorValue <= j ){
    increment = increment + 3;                            // Increment motor value
                                                          Serial2.print("Moved by :  ");
                                                         Serial2.println(increment);
                                                         Serial2.print("Sensor Value: ");
                                                          Serial2.println(sensorValue);
    myservo.write(increment);                             // Move Motor with adjustment Value
    delay(1000);                                          // 1 sec delay
    calValue();                                           // Call LDR function to get Value
                          }                               // End of While(sensorValue<=j)
                                                          Serial2.println("LEAVING MOTOR MOVE");
                                                         Serial2.print("Sensor Value After Adjustments: ");
                                                          Serial2.println(sensorValue);
                          }                               // End of motorMove()
/////////////////////////////           END of motorMove      ///////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////            LDR Function(s)            //////////////////////////////////
void calValue(){
  sensorValue = analogRead(lightPin);                     // Read Analog value from pin 15
                                                          // Mapping function to sensor Min and Max
  sensorValue = map(sensorValue, sensorMin, sensorMax, 0, 300);
                                                         
  sensorValue = constrain(sensorValue, 0, 300);           // If sensorValue out of range during calibration
                                                          // 20%    40%        60%       80%
                                                          // 45-75  105-135    165-195   225-255
                                                          // 7% tolerance for each percentile
                                                          // 40-80  100-140    160-200   220-260
  total = total - readings[readIndex];                    // Subtract previous reading from total
  readings[readIndex] = sensorValue;                      // Mapped sensorValue stored in Array
  total = total + readings[readIndex];                    // Add previous reading to the total value
  readIndex = readIndex + 1;                              // Next position in Array
  if(readIndex >= numReadings){                           // If at the end of reading Array[10]
     readIndex = 0;           }                           // Wrap around to the beginning
  average = total / numReadings;                          // Calculate average of 10 readings
  delay(100);                                             // 100ms delay for read stability
  return;                                                 // Return to Previous Function
                }                                         // End of calValue()
/////////////////////////////////////////////////////////////////////////////////////////////////////
void smoothSetup(){                                       // Smoothing setup                                                       
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;}                           // End of Clear Readings
                  }                                       // End of smoothSetup()
/////////////////////////////////////////////////////////////////////////////////////////////////////
void calSetup(){
                        
  pinMode(13, OUTPUT);                                    // turn on LED to signal... 
  digitalWrite(13, HIGH);                                 // ... start of the calibration period                                                
  while (millis() < 5000){                                // calibrate during the first 5 seconds
    sensorValue = analogRead(lightPin);                   // Read Analog Value on pin 15
    if(sensorValue > sensorMax){                          // Record the maximum sensor value
       sensorMax = sensorValue;                           // Set New sensorMax to current sensorValue
                               }                          // End of New sensorMax
    if(sensorValue < sensorMin){                          // Record the minimum sensor value
      sensorMin = sensorValue;                            // Set New sensorMin to current sensorValue
                               }                          // End of New sensorMin
                          }                               // End of while for 5 seconds
  digitalWrite(13, LOW);                                  // Turn off LED at end of calibration
              }                                           // End of calSetup()
//////////////////////////////         END of LDR Function(s)      //////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

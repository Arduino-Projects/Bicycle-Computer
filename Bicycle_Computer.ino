//Make sure to download all of the required libraries before running any of the programs
//http://www.rinkydinkelectronics.com/library.php?id=51 (UTFT Display Library)
//http://www.rinkydinkelectronics.com/library.php?id=59 (UTFT Geometry Library)
//http://www.rinkydinkelectronics.com/library.php?id=92 (Touchscreen Library)
//Run the calibration example sketch first found in the URTouch Library and change the following values to the values the program gives you
#define CAL_X 0x00500F0BUL
#define CAL_Y 0x03C78142UL
#define CAL_S 0x000EF13FUL
//Connections
//Speed Hall Effect Sensor = Pin 20(Signal Pin)
//RPM Hall Effect Sensor = Pin 21(Signal pin)

int weight = 46;//Please enter your weight IN KG'S over here.
double wheelDiameter = 207.5;//Please enter the diameter of your bicycle wheel in CM over here.


#include <UTFT_Geometry.h>
#include <memorysaver.h>
#include <UTFT.h>
#include <URTouch.h>

URTouch  myTouch( 6, 5, 4, 3, 2);
UTFT myGLCD(SSD1289,38,39,40,41);
extern uint8_t SmallFont[];
extern uint8_t BigFont[];
extern uint8_t SevenSegNumFont[];

int calorieGoal = '0';
int start = "0";
int x;
int y;
UTFT_Geometry geo(&myGLCD);

long prevMillis;
double newMillis,newMillis1,newMillis2;
long Millis;
double Speed;
int speedRot;
int pedalRot;
int Ang;
int caloriesBurned;
double startTimeMins;
double TotalKm;
int mode;

double MET;

float totalKM;
float finalTripKM;
float avgSpdCalc;
float AvgSpd;
double AvgSpdOut;

long prevMillis2;
double newMillis3,newMillis4,newMillis5;
long Millis2;
double RPM;
long startTime;
long setupTime;
long finalTime;

int setPos1 = 1;
int setPos2 = 1;
int setPos3 = 1;

void setup()
{
// Setup the LCD
  myTouch.InitTouch();
  myGLCD.InitLCD();
  myGLCD.setBackColor(0,0,0);
  myGLCD.setColor(255,255, 255);
  myGLCD.setFont(BigFont);
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(21), detectWheelRotation, FALLING); //Speed Calculation
  attachInterrupt(digitalPinToInterrupt(20), detectPedalRotation, RISING); //RPM Calculation

}

void loop() {
  odometer();
  calcSpeedRot();
  calorieCalculator();
if (start == "-1"){
  homeFunction();
  }
  if (start == "0"){
    drawHome();
  }
if (start == "1"){
  askCalories();
}
if (start == "2"){
  calorieFunction();
}
if (start == "3"){
  drawStartBiking();
}
if (start == "4"){
  drawMainScreen();
}
if (start == "5"){
  mainScreenFunction();
}
if (start == "6"){
  drawOff();
}
if(start == "7"){
  
}
}

void drawHome() {  //Drawing out the Home Page
  myGLCD.clrScr();
  myGLCD.setColor(0,255,255);
  myGLCD.fillRect(30,28,289,7);
  myGLCD.setColor(255,255,255);
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(VGA_BLACK);
  myGLCD.print("Bicycle Computer", CENTER, 10);
  myGLCD.setColor(255,150,0);
  myGLCD.fillRoundRect(10,65,150,205);
  myGLCD.fillRoundRect(170,65,310,205);
  myGLCD.setColor(251,59,49);
  myGLCD.print("Calorie", 24, 115);
  myGLCD.print("Burner", 32, 135);
  myGLCD.print("Only", 210, 115);
  myGLCD.print("Data", 210, 135);
  start = "-1";
}

void drawOff() {
  int number;
 myGLCD.clrScr();
 myGLCD.setFont(SmallFont);
 myGLCD.setColor(VGA_RED);
 myGLCD.setBackColor(VGA_RED);
 myGLCD.fillScr(VGA_BLUE);
 myGLCD.fillRoundRect(40,50,280,190);
 myGLCD.setColor(VGA_BLUE);
 myGLCD.print("Calories Burned:", CENTER, 105);
 myGLCD.printNumI(caloriesBurned,257,105);
 myGLCD.print("Distance Biked(km):", CENTER, 125);
 myGLCD.printNumF(TotalKm,1,257,125,'.',2,0);
 myGLCD.print("Average Speed:", CENTER, 145);
 myGLCD.printNumF(AvgSpd,1,257,145,'.',3,'0');
 myGLCD.print("Elapsed Time(mins)):", CENTER, 165);
 myGLCD.printNumF(startTimeMins,1,257,165,'.',3,'0');
 delay(15000);
 myGLCD.fillScr(VGA_BLACK);
  start = "7";
 
}

void calcSpeedRot() {
  long check = millis();
  if (check - prevMillis > 3000){
speedRot = 0;
Speed = 0;
  }
  else{
speedRot = map(Speed, 0, 50, 1, 10);

}
 if(check - prevMillis2 > 3000) {
  pedalRot = 0;
  RPM = 0;
 }
 else{
  pedalRot = map(RPM, 10, 120, 1, 10);
 }
}
void askCalories() {  //If user clicks the calories button then this page is printed
  myGLCD.clrScr();
  myGLCD.setColor(255,255,0);
  myGLCD.setBackColor(VGA_BLACK);
  myGLCD.setFont(BigFont);
  myGLCD.print("What is your", CENTER, 15);
  myGLCD.print("calorie goal?",CENTER, 30);
  myGLCD.setColor(255,0,0);
  myGLCD.fillRoundRect(3,89,109,195);
  myGLCD.setColor(0,255,0);
  myGLCD.fillRoundRect(208,89,310,195);
  myGLCD.setColor(0,0,255);
  myGLCD.fillRect(8,135,104,145);
  myGLCD.fillRect(213,135,305,145);
  myGLCD.fillRect(254,95,264,185);
  myGLCD.setFont(SevenSegNumFont);
  myGLCD.print(calorieGoal, CENTER, 115);
  myGLCD.setColor(0,188,236);
  myGLCD.fillRect(3,210,120,235 );
  myGLCD.setColor(193,200,200);
  myGLCD.fillRect(30,220,115,225);
  geo.fillTriangle(30,215,30,230,21,222.5);
  myGLCD.setColor(0,188,236);
  myGLCD.fillRect(316,210,200,235);
  myGLCD.setColor(193,200,200);
  myGLCD.fillRect(290,220,205,225);
  geo.fillTriangle(290,215,290,230,299,222.5);
  start = "2";
}

void drawStartBiking() {
  myGLCD.clrScr();
  myGLCD.setFont(BigFont);
  myGLCD.setColor(227,18,48);
  myGLCD.fillScr(0,76,225);
  myGLCD.setBackColor(VGA_GREEN);
  myGLCD.print("START BIKING", CENTER, 70);
  delay(1000);
  myGLCD.print("3", 50, 150);
  delay(1000);
  myGLCD.print("2", 160, 150);
  delay(1000);
  myGLCD.print("1", 270, 150);
  delay(1000);
  setupTime = millis();
  start = "4";
  
}

void drawMainScreen(){
  myGLCD.clrScr();
  myGLCD.setColor(255,0,0);
  myGLCD.fillRoundRect(120,5,200,75);
  myGLCD.setColor(VGA_BLUE);
  myGLCD.fillRoundRect(10,5,90,75);
  myGLCD.fillRoundRect(230,5,310,75);
  myGLCD.drawRect(9,104,311,161);
  myGLCD.drawRect(9,174,311,231);
  myGLCD.setBackColor(255,0,0);
  myGLCD.setFont(SmallFont);
  myGLCD.setColor(VGA_PURPLE);
  myGLCD.print("Finish",190,80);
  myGLCD.print("Biking",190,90);
  myGLCD.setColor(VGA_BLUE);
  
  if(setPos1 == 1){
  myGLCD.print("KM/H",CENTER,70);
  }
  else if(setPos1 == 2){
  myGLCD.print("Calories",CENTER,70);
  }
  else if(setPos1 == 3){
  myGLCD.print("RPM",CENTER,70);
  }
  else if(setPos1 == 4){
  myGLCD.print("Total KM",CENTER,70);
  }
myGLCD.setBackColor(VGA_BLUE);
myGLCD.setColor(VGA_LIME);

  if(setPos2 == 1){
  myGLCD.print("Calories",19,70);
  }
  else if(setPos2 == 2){
  myGLCD.print("KM/H",35,70);
  }
  else if(setPos2 == 3){
  myGLCD.print("RPM",38,70);
  }
  else if(setPos2 == 4){
  myGLCD.print("Total KM",19,70);
  }

  if(setPos3 == 1){
  myGLCD.print("RPM",260,70);
  }
  else if(setPos3 == 2){
  myGLCD.print("Calories",240,70);
  }
  else if(setPos3 == 3){
  myGLCD.print("KM/H",255,70);
  }
  else if(setPos3 == 4){
  myGLCD.print("Total KM",239,70);
  }
  
  start = "5";
}

void mainScreenFunction() {
 if(myTouch.dataAvailable()){
    myTouch.read();
    x = myTouch.getX();
    y = myTouch.getY();
 if((x<=200)&&(x>=120)&&(y<=75)&&(y>=5)){
  if(setPos1<4){
    setPos1++;
  }
  else {
    setPos1 = 1;}
    start = "4";
  }
 if((x<=90)&&(x>=10)&&(y<=75)&&(y>=5)){
  if(setPos2<4){
    setPos2++;
  }
  else{
    setPos2 = 1;}
    start = "4";
 }
  if((x<=310)&&(x>=230)&&(y<=75)&&(y>=5)){
  if(setPos3<4){
    setPos3++;
  }
  else{
    setPos3 = 1;}
    start = "4";
 }
 if((x<=234)&&(x>=190)&&(y<=100)&&(y>=80)){
  start = "6";
 }
 }
    

  switch (speedRot) {
case 0:
myGLCD.setColor(VGA_BLACK);
myGLCD.fillRect(10,105,40,160); 
myGLCD.fillRect(40,105,70,160); 
myGLCD.fillRect(70,105,100,160); 
myGLCD.fillRect(100,105,130,160); 
myGLCD.fillRect(130,105,160,160); 
myGLCD.fillRect(160,105,190,160); 
myGLCD.fillRect(190,105,220,160); 
myGLCD.fillRect(220,105,250,160); 
myGLCD.fillRect(250,105,280,160); 
myGLCD.fillRect(280,105,310,160);
break;
case 1:
myGLCD.setColor(60, 255,0);
myGLCD.fillRect(10,105,40,160);
myGLCD.setColor(VGA_BLACK);
myGLCD.fillRect(40,105,70,160); 
myGLCD.fillRect(70,105,100,160); 
myGLCD.fillRect(100,105,130,160); 
myGLCD.fillRect(130,105,160,160); 
myGLCD.fillRect(160,105,190,160); 
myGLCD.fillRect(190,105,220,160); 
myGLCD.fillRect(220,105,250,160); 
myGLCD.fillRect(250,105,280,160); 
myGLCD.fillRect(280,105,310,160);
break;
case 2:
myGLCD.setColor(60, 255,0);
myGLCD.fillRect(10,105,40,160); 
myGLCD.setColor(196, 255,0);
myGLCD.fillRect(40,105,70,160);
myGLCD.setColor(VGA_BLACK); 
myGLCD.fillRect(70,105,100,160);
myGLCD.fillRect(100,105,130,160); 
myGLCD.fillRect(130,105,160,160); 
myGLCD.fillRect(160,105,190,160); 
myGLCD.fillRect(190,105,220,160); 
myGLCD.fillRect(220,105,250,160); 
myGLCD.fillRect(250,105,280,160); 
myGLCD.fillRect(280,105,310,160);
break;
case 3:
myGLCD.setColor(60, 255,0);
myGLCD.fillRect(10,105,40,160); 
myGLCD.setColor(196, 255,0);
myGLCD.fillRect(40,105,70,160); 
myGLCD.setColor(247, 255,0);
myGLCD.fillRect(70,105,100,160);
myGLCD.setColor(VGA_BLACK);
myGLCD.fillRect(100,105,130,160); 
myGLCD.fillRect(130,105,160,160); 
myGLCD.fillRect(160,105,190,160); 
myGLCD.fillRect(190,105,220,160); 
myGLCD.fillRect(220,105,250,160); 
myGLCD.fillRect(250,105,280,160); 
myGLCD.fillRect(280,105,310,160);
break;
case 4:
myGLCD.setColor(60, 255,0);
myGLCD.fillRect(10,105,40,160); 
myGLCD.setColor(196, 255,0);
myGLCD.fillRect(40,105,70,160); 
myGLCD.setColor(247, 255,0);
myGLCD.fillRect(70,105,100,160); 
myGLCD.setColor(255, 222,0);
myGLCD.fillRect(100,105,130,160);
myGLCD.setColor(VGA_BLACK);
myGLCD.fillRect(130,105,160,160); 
myGLCD.fillRect(160,105,190,160); 
myGLCD.fillRect(190,105,220,160); 
myGLCD.fillRect(220,105,250,160); 
myGLCD.fillRect(250,105,280,160); 
myGLCD.fillRect(280,105,310,160);
break;
case 5:
myGLCD.setColor(60, 255,0);
myGLCD.fillRect(10,105,40,160); 
myGLCD.setColor(196, 255,0);
myGLCD.fillRect(40,105,70,160); 
myGLCD.setColor(247, 255,0);
myGLCD.fillRect(70,105,100,160); 
myGLCD.setColor(255, 222,0);
myGLCD.fillRect(100,105,130,160); 
myGLCD.setColor(255, 179,0);
myGLCD.fillRect(130,105,160,160); 
myGLCD.setColor(VGA_BLACK);
myGLCD.fillRect(160,105,190,160); 
myGLCD.fillRect(190,105,220,160); 
myGLCD.fillRect(220,105,250,160); 
myGLCD.fillRect(250,105,280,160); 
myGLCD.fillRect(280,105,310,160);
break;
case 6:
myGLCD.setColor(60, 255,0);
myGLCD.fillRect(10,105,40,160); 
myGLCD.setColor(196, 255,0);
myGLCD.fillRect(40,105,70,160); 
myGLCD.setColor(247, 255,0);
myGLCD.fillRect(70,105,100,160); 
myGLCD.setColor(255, 222,0);
myGLCD.fillRect(100,105,130,160); 
myGLCD.setColor(255, 179,0);
myGLCD.fillRect(130,105,160,160); 
myGLCD.setColor(255, 154,0);
myGLCD.fillRect(160,105,190,160);
myGLCD.setColor(VGA_BLACK); 
myGLCD.fillRect(190,105,220,160); 
myGLCD.fillRect(220,105,250,160); 
myGLCD.fillRect(250,105,280,160); 
myGLCD.fillRect(280,105,310,160);
break;
case 7:
myGLCD.setColor(60, 255,0);
myGLCD.fillRect(10,105,40,160); 
myGLCD.setColor(196, 255,0);
myGLCD.fillRect(40,105,70,160); 
myGLCD.setColor(247, 255,0);
myGLCD.fillRect(70,105,100,160); 
myGLCD.setColor(255, 222,0);
myGLCD.fillRect(100,105,130,160); 
myGLCD.setColor(255, 179,0);
myGLCD.fillRect(130,105,160,160); 
myGLCD.setColor(255, 154,0);
myGLCD.fillRect(160,105,190,160); 
myGLCD.setColor(255, 111,0);
myGLCD.fillRect(190,105,220,160);
myGLCD.setColor(VGA_BLACK);  
myGLCD.fillRect(220,105,250,160); 
myGLCD.fillRect(250,105,280,160); 
myGLCD.fillRect(280,105,310,160);
break;
case 8:
myGLCD.setColor(60, 255,0);
myGLCD.fillRect(10,105,40,160); 
myGLCD.setColor(196, 255,0);
myGLCD.fillRect(40,105,70,160); 
myGLCD.setColor(247, 255,0);
myGLCD.fillRect(70,105,100,160); 
myGLCD.setColor(255, 222,0);
myGLCD.fillRect(100,105,130,160); 
myGLCD.setColor(255, 179,0);
myGLCD.fillRect(130,105,160,160); 
myGLCD.setColor(255, 154,0);
myGLCD.fillRect(160,105,190,160); 
myGLCD.setColor(255, 111,0);
myGLCD.fillRect(190,105,220,160); 
myGLCD.setColor(255, 70,0);
myGLCD.fillRect(220,105,250,160); 
myGLCD.setColor(VGA_BLACK);  
myGLCD.fillRect(250,105,280,160); 
myGLCD.fillRect(280,105,310,160);
break;
case 9:
myGLCD.setColor(60, 255,0);
myGLCD.fillRect(10,105,40,160); 
myGLCD.setColor(196, 255,0);
myGLCD.fillRect(40,105,70,160); 
myGLCD.setColor(247, 255,0);
myGLCD.fillRect(70,105,100,160); 
myGLCD.setColor(255, 222,0);
myGLCD.fillRect(100,105,130,160); 
myGLCD.setColor(255, 179,0);
myGLCD.fillRect(130,105,160,160); 
myGLCD.setColor(255, 154,0);
myGLCD.fillRect(160,105,190,160); 
myGLCD.setColor(255, 111,0);
myGLCD.fillRect(190,105,220,160); 
myGLCD.setColor(255, 70,0);
myGLCD.fillRect(220,105,250,160); 
myGLCD.setColor(255, 30,0);
myGLCD.fillRect(250,105,280,160); 
myGLCD.setColor(VGA_BLACK);  
myGLCD.fillRect(280,105,310,160);
break;
case 10:
myGLCD.setColor(60, 255,0);
myGLCD.fillRect(10,105,40,160); 
myGLCD.setColor(196, 255,0);
myGLCD.fillRect(40,105,70,160); 
myGLCD.setColor(247, 255,0);
myGLCD.fillRect(70,105,100,160); 
myGLCD.setColor(255, 222,0);
myGLCD.fillRect(100,105,130,160); 
myGLCD.setColor(255, 179,0);
myGLCD.fillRect(130,105,160,160); 
myGLCD.setColor(255, 154,0);
myGLCD.fillRect(160,105,190,160); 
myGLCD.setColor(255, 111,0);
myGLCD.fillRect(190,105,220,160); 
myGLCD.setColor(255, 70,0);
myGLCD.fillRect(220,105,250,160); 
myGLCD.setColor(255, 30,0);
myGLCD.fillRect(250,105,280,160); 
myGLCD.setColor(255, 0,0);
myGLCD.fillRect(280,105,310,160);
break;
}
switch (pedalRot) {
case 0:
myGLCD.setColor(VGA_BLACK);
myGLCD.fillRect(10,175,40,230); 
myGLCD.fillRect(40,175,70,230); 
myGLCD.fillRect(70,175,100,230); 
myGLCD.fillRect(100,175,130,230); 
myGLCD.fillRect(130,175,160,230); 
myGLCD.fillRect(160,175,190,230); 
myGLCD.fillRect(190,175,220,230); 
myGLCD.fillRect(220,175,250,230); 
myGLCD.fillRect(250,175,280,230); 
myGLCD.fillRect(280,175,310,230);
break;
case 1:
myGLCD.setColor(60, 255,0);
myGLCD.fillRect(10,175,40,230);
myGLCD.setColor(VGA_BLACK);
myGLCD.fillRect(40,175,70,230); 
myGLCD.fillRect(70,175,100,230); 
myGLCD.fillRect(100,175,130,230); 
myGLCD.fillRect(130,175,160,230); 
myGLCD.fillRect(160,175,190,230); 
myGLCD.fillRect(190,175,220,230); 
myGLCD.fillRect(220,175,250,230); 
myGLCD.fillRect(250,175,280,230); 
myGLCD.fillRect(280,175,310,230);
break;
case 2:
myGLCD.setColor(60, 255,0);
myGLCD.fillRect(10,175,40,230); 
myGLCD.setColor(196, 255,0);
myGLCD.fillRect(40,175,70,230);
myGLCD.setColor(VGA_BLACK); 
myGLCD.fillRect(70,175,100,230);
myGLCD.fillRect(100,175,130,230); 
myGLCD.fillRect(130,175,160,230); 
myGLCD.fillRect(160,175,190,230); 
myGLCD.fillRect(190,175,220,230); 
myGLCD.fillRect(220,175,250,230); 
myGLCD.fillRect(250,175,280,230); 
myGLCD.fillRect(280,175,310,230);
break;
case 3:
myGLCD.setColor(60, 255,0);
myGLCD.fillRect(10,175,40,230); 
myGLCD.setColor(196, 255,0);
myGLCD.fillRect(40,175,70,230); 
myGLCD.setColor(247, 255,0);
myGLCD.fillRect(70,175,100,230);
myGLCD.setColor(VGA_BLACK);
myGLCD.fillRect(100,175,130,230); 
myGLCD.fillRect(130,175,160,230); 
myGLCD.fillRect(160,175,190,230); 
myGLCD.fillRect(190,175,220,230); 
myGLCD.fillRect(220,175,250,230); 
myGLCD.fillRect(250,175,280,230); 
myGLCD.fillRect(280,175,310,230);
break;
case 4:
myGLCD.setColor(60, 255,0);
myGLCD.fillRect(10,175,40,230); 
myGLCD.setColor(196, 255,0);
myGLCD.fillRect(40,175,70,230); 
myGLCD.setColor(247, 255,0);
myGLCD.fillRect(70,175,100,230); 
myGLCD.setColor(255, 222,0);
myGLCD.fillRect(100,175,130,230);
myGLCD.setColor(VGA_BLACK);
myGLCD.fillRect(130,175,160,230); 
myGLCD.fillRect(160,175,190,230); 
myGLCD.fillRect(190,175,220,230); 
myGLCD.fillRect(220,175,250,230); 
myGLCD.fillRect(250,175,280,230); 
myGLCD.fillRect(280,175,310,230);
break;
case 5:
myGLCD.setColor(60, 255,0);
myGLCD.fillRect(10,175,40,230); 
myGLCD.setColor(196, 255,0);
myGLCD.fillRect(40,175,70,230); 
myGLCD.setColor(247, 255,0);
myGLCD.fillRect(70,175,100,230); 
myGLCD.setColor(255, 222,0);
myGLCD.fillRect(100,175,130,230); 
myGLCD.setColor(255, 179,0);
myGLCD.fillRect(130,175,160,230); 
myGLCD.setColor(VGA_BLACK);
myGLCD.fillRect(160,175,190,230); 
myGLCD.fillRect(190,175,220,230); 
myGLCD.fillRect(220,175,250,230); 
myGLCD.fillRect(250,175,280,230); 
myGLCD.fillRect(280,175,310,230);
break;
case 6:
myGLCD.setColor(60, 255,0);
myGLCD.fillRect(10,175,40,230); 
myGLCD.setColor(196, 255,0);
myGLCD.fillRect(40,175,70,230); 
myGLCD.setColor(247, 255,0);
myGLCD.fillRect(70,175,100,230); 
myGLCD.setColor(255, 222,0);
myGLCD.fillRect(100,175,130,230); 
myGLCD.setColor(255, 179,0);
myGLCD.fillRect(130,175,160,230); 
myGLCD.setColor(255, 154,0);
myGLCD.fillRect(160,175,190,230);
myGLCD.setColor(VGA_BLACK); 
myGLCD.fillRect(190,175,220,230); 
myGLCD.fillRect(220,175,250,230); 
myGLCD.fillRect(250,175,280,230); 
myGLCD.fillRect(280,175,310,230);
break;
case 7:
myGLCD.setColor(60, 255,0);
myGLCD.fillRect(10,175,40,230); 
myGLCD.setColor(196, 255,0);
myGLCD.fillRect(40,175,70,230); 
myGLCD.setColor(247, 255,0);
myGLCD.fillRect(70,175,100,230); 
myGLCD.setColor(255, 222,0);
myGLCD.fillRect(100,175,130,230); 
myGLCD.setColor(255, 179,0);
myGLCD.fillRect(130,175,160,230); 
myGLCD.setColor(255, 154,0);
myGLCD.fillRect(160,175,190,230); 
myGLCD.setColor(255, 111,0);
myGLCD.fillRect(190,175,220,230);
myGLCD.setColor(VGA_BLACK);  
myGLCD.fillRect(220,175,250,230); 
myGLCD.fillRect(250,175,280,230); 
myGLCD.fillRect(280,175,310,230);
break;
case 8:
myGLCD.setColor(60, 255,0);
myGLCD.fillRect(10,175,40,230); 
myGLCD.setColor(196, 255,0);
myGLCD.fillRect(40,175,70,230); 
myGLCD.setColor(247, 255,0);
myGLCD.fillRect(70,175,100,230); 
myGLCD.setColor(255, 222,0);
myGLCD.fillRect(100,175,130,230); 
myGLCD.setColor(255, 179,0);
myGLCD.fillRect(130,175,160,230); 
myGLCD.setColor(255, 154,0);
myGLCD.fillRect(160,175,190,230); 
myGLCD.setColor(255, 111,0);
myGLCD.fillRect(190,175,220,230); 
myGLCD.setColor(255, 70,0);
myGLCD.fillRect(220,175,250,230); 
myGLCD.setColor(VGA_BLACK);  
myGLCD.fillRect(250,175,280,230); 
myGLCD.fillRect(280,175,310,230);
break;
case 9:
myGLCD.setColor(60, 255,0);
myGLCD.fillRect(10,175,40,230); 
myGLCD.setColor(196, 255,0);
myGLCD.fillRect(40,175,70,230); 
myGLCD.setColor(247, 255,0);
myGLCD.fillRect(70,175,100,230); 
myGLCD.setColor(255, 222,0);
myGLCD.fillRect(100,175,130,230); 
myGLCD.setColor(255, 179,0);
myGLCD.fillRect(130,175,160,230); 
myGLCD.setColor(255, 154,0);
myGLCD.fillRect(160,175,190,230); 
myGLCD.setColor(255, 111,0);
myGLCD.fillRect(190,175,220,230); 
myGLCD.setColor(255, 70,0);
myGLCD.fillRect(220,175,250,230); 
myGLCD.setColor(255, 30,0);
myGLCD.fillRect(250,175,280,230); 
myGLCD.setColor(VGA_BLACK);  
myGLCD.fillRect(280,175,310,230);
break;
case 10:
myGLCD.setColor(60, 255,0);
myGLCD.fillRect(10,175,40,230); 
myGLCD.setColor(196, 255,0);
myGLCD.fillRect(40,175,70,230); 
myGLCD.setColor(247, 255,0);
myGLCD.fillRect(70,175,100,230); 
myGLCD.setColor(255, 222,0);
myGLCD.fillRect(100,175,130,230); 
myGLCD.setColor(255, 179,0);
myGLCD.fillRect(130,175,160,230); 
myGLCD.setColor(255, 154,0);
myGLCD.fillRect(160,175,190,230); 
myGLCD.setColor(255, 111,0);
myGLCD.fillRect(190,175,220,230); 
myGLCD.setColor(255, 70,0);
myGLCD.fillRect(220,175,250,230); 
myGLCD.setColor(255, 30,0);
myGLCD.fillRect(250,175,280,230); 
myGLCD.setColor(255, 0,0);
myGLCD.fillRect(280,175,310,230);
break;
}
if (setPos1 == 1){
  myGLCD.setBackColor(VGA_RED);
  myGLCD.setFont(SevenSegNumFont);
  myGLCD.setColor(VGA_BLUE);
  myGLCD.printNumI(Speed, CENTER,8,2,'0');
}
if(setPos1 == 2){
  myGLCD.setBackColor(VGA_RED);
  myGLCD.setFont(SevenSegNumFont);
  myGLCD.setColor(VGA_BLUE);
  myGLCD.printNumI(caloriesBurned, CENTER,8,2,'0');
}
if(setPos1 == 3){
  myGLCD.setBackColor(VGA_RED);
  myGLCD.setFont(SevenSegNumFont);
  myGLCD.setColor(VGA_BLUE);
  myGLCD.printNumI(RPM, CENTER,8,2,'0');
}
if(setPos1 == 4){
  myGLCD.setBackColor(VGA_BLUE);
  myGLCD.setFont(BigFont);
  myGLCD.setColor(VGA_RED);
  myGLCD.printNumF(TotalKm, 1,CENTER,30,'.',2,'0');
}
if (setPos2 == 2){
  myGLCD.setBackColor(VGA_BLUE);
  myGLCD.setFont(SevenSegNumFont);
  myGLCD.setColor(VGA_RED);
  myGLCD.printNumI(Speed, 19,8,2,'0');
}
if(setPos2 == 1){
  myGLCD.setBackColor(VGA_BLUE);
  myGLCD.setFont(SevenSegNumFont);
  myGLCD.setColor(VGA_RED);
  myGLCD.printNumI(caloriesBurned, 19,8,2,'0');
}
if(setPos2 == 3){
  myGLCD.setBackColor(VGA_BLUE);
  myGLCD.setFont(SevenSegNumFont);
  myGLCD.setColor(VGA_RED);
  myGLCD.printNumI(RPM, 19,8,2,'0');
}
if(setPos2 == 4){
  myGLCD.setBackColor(VGA_BLUE);
  myGLCD.setFont(BigFont);
  myGLCD.setColor(VGA_RED);
  myGLCD.printNumF(TotalKm, 1,25,30,'.',2,'0');
}
if (setPos3 == 3){
  myGLCD.setBackColor(VGA_BLUE);
  myGLCD.setFont(SevenSegNumFont);
  myGLCD.setColor(VGA_RED);
  myGLCD.printNumI(Speed, 240,8,2,'0');
}
if(setPos3 == 2){
  myGLCD.setBackColor(VGA_BLUE);
  myGLCD.setFont(SevenSegNumFont);
  myGLCD.setColor(VGA_RED);
  myGLCD.printNumI(caloriesBurned, 240,8,2,'0');
}
if(setPos3 == 1){
  myGLCD.setBackColor(VGA_BLUE);
  myGLCD.setFont(SevenSegNumFont);
  myGLCD.setColor(VGA_RED);
  myGLCD.printNumI(RPM, 240,8,2,'0');
}
if(setPos3 == 4){
  myGLCD.setBackColor(VGA_BLUE);
  myGLCD.setFont(BigFont);
  myGLCD.setColor(VGA_RED);
  myGLCD.printNumF(TotalKm, 1,246,30,'.',2,'0');
}
}


void calorieFunction() {
  if(myTouch.dataAvailable()){
    myTouch.read();
    x = myTouch.getX();
    y = myTouch.getY();
    if((x<=109)&(x>=3)&(y<=195)&(y>=89)){
      calorieGoal = calorieGoal - 5;
      delay(10);
    }
    else if ((x<=310)&(x>=208)&(y<=195)&(y>=89)){
      calorieGoal = calorieGoal + 5;
      delay(10);
    }
    else if((x<=120)&(x>=3)&(y<=235)&(y>=210)){
start = "0";  
    }
    else if((x<=316)&(x>=200)&(y<=235)&(y>=210)){
start = "3";
    }
    myGLCD.printNumI(calorieGoal, CENTER, 110, 3,'0');
    
}
}

void homeFunction() {
    if(myTouch.dataAvailable()){
    myTouch.read();
    x = myTouch.getX();
    y = myTouch.getY();
    if((x<=150)&(x>=10)&(y<=205)&(y>=65)){
      start = "1";
    }
    if((x<=310)&(x>=170)&(y<=205)&(y>=65)){
      start = "3";
    }
    }
}

void calorieCalculator() {
  int num;
  double startTimeSecs = startTime/1000;
  startTimeMins = startTimeSecs/60;
  MET = map(AvgSpd, 4,45,3,20);
  AvgSpdOut = AvgSpd;
  caloriesBurned = ((MET*3.5*weight)/200)*startTimeMins;
}


void detectWheelRotation() { //Calculating the Speed of the bike
  prevMillis = Millis;
  Millis = millis();
  newMillis = Millis - prevMillis;
  newMillis1 = 207.5/newMillis;
  newMillis2 = newMillis1*36;
  Speed = newMillis2;
  totalKM++;
  delay(10);
}

void detectPedalRotation() { // Calculating the RPM of the pedals
    prevMillis2 = Millis2;
  Millis2 = millis();
  newMillis3 = Millis2 - prevMillis2;
  newMillis4 = 60000/newMillis3;
  RPM = newMillis4;
  delay(100);
  }

void odometer() {
  float totalcm;
  totalcm = totalKM*wheelDiameter;
  TotalKm = totalcm/100000;
 long MILLIS = millis();
 startTime = MILLIS - setupTime;
 avgSpdCalc = totalcm/startTime;
 AvgSpd = avgSpdCalc*36;
}


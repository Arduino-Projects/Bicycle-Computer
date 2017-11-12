//CONNECTIONS
//HALL EFFECT SENSOR
//SIGNAL = PIN 2
//GROUND = GROUND
//VCC = 5 VOLTS
//MAKE SURE TO HAVE THE 10 K OHM RESISTOR IN THE CIRCUIT WHEN RUNNING ANY PROGRAM FOR THE HALL EFFECT SENSORS

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
attachInterrupt(0,magnetDetect,RISING);
}

void loop() {
  // put your main code here, to run repeatedly:

}
void magnetDetect(){
  Serial.println("DETECT");
}


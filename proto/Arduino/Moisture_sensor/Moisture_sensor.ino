/*
 * Moisture sensor test
*/


// I2C screen :
// Taille du module: 27.0 x 27.0 x 4.1 MM;
// Interface I2C/IIC, besoin 2 IO uniquement;
// Driver IC: SSD1306
// Ultra faible consommation d'énergie: plein écran allumé 0.08W;
//Tension: 3V ~ 5V DC;
//La température fonctionnante:-30 ° c ~ 70 ° c

const  long int minMicros = 0;
const  long int maxMicros = 0x1111111;
const byte Pin_IO_01 = 27;
const byte Pin_IO_02 = 32;
const byte Pin_IO_03 = 14;


// the setup function runs once when you press reset or power the board
void setup() {
  // Hygro mode
  /*
  pinMode(Pin_IO_01, OUTPUT);
  digitalWrite(Pin_IO_01,LOW);

  pinMode(Pin_IO_02, OUTPUT);
  digitalWrite(Pin_IO_02,HIGH);
  
  pinMode(Pin_IO_03, INPUT);
*/

  // EC mode
  pinMode(Pin_IO_01, OUTPUT);
  digitalWrite(Pin_IO_01,LOW);

  pinMode(Pin_IO_02, OUTPUT);
  digitalWrite(Pin_IO_02,LOW);
  
  pinMode(Pin_IO_03, INPUT);


  
  Serial.begin(250000);
  Serial.println("begin");
}

byte firstTime = 1;

long int microsDiff(const  long int a, const  long int b) {
  return (a >= b) ? (a - b) : ((a - minMicros) + (maxMicros - b));
}

// the loop function runs over and over again forever
void loop() {
  
  // Hygro mode
  /*
  delay(1000);
  int Value_IO_01 = analogRead(Pin_IO_03);
  Serial.print("Moisture Level : ");
  Serial.println(Value_IO_01);
*/
                      
  // EC mode
  if (firstTime) {
    firstTime = 0;
    delay(1000);
    Serial.println("");
    Serial.println("Time;Drive;T1+T2;T1m;T1u;Tz;Sensor");
    
      int cycle = 0;
      int Value_IO_01 = 0;
      int drive = 4095;
      unsigned long int t1=0;
      unsigned long int t2=0;
      byte isZeroTime = 1;
      unsigned long int startMillis = millis();
      unsigned long int measureStartMillis = startMillis;
      unsigned long int measureStartMicros = micros();
      unsigned long int zeroTimeEnd = measureStartMicros;
      unsigned long int measureStopMicros = measureStartMicros;
      unsigned long int measureStopMillis = measureStartMillis;
      
    while (1)
    {
      unsigned long int elapsedMillis = measureStopMillis-measureStartMillis;
      if (Value_IO_01 > 1000 || Value_IO_01 < -1000 || elapsedMillis > 30000) {
        unsigned long int elapsedMicros = microsDiff(measureStopMicros,measureStartMicros);
        drive = -drive;
        t2=t1;
        t1 = elapsedMicros + 1000 * elapsedMillis;
        Serial.print((double)(millis()-startMillis)/3600000.0,14);
        Serial.print(";");
        Serial.print(drive);
        Serial.print(";");
        Serial.print(t1+t2);
        Serial.print(";");
        Serial.print(t1);
        Serial.print(";");
        Serial.print(elapsedMillis);
        Serial.print(";");
        Serial.print(elapsedMicros);
        Serial.print(";");
        Serial.print(microsDiff(zeroTimeEnd,measureStartMicros));
        Serial.print(";");
        Serial.println(Value_IO_01);
        isZeroTime = 1;
        measureStartMicros = micros();
        measureStartMillis = millis();
        zeroTimeEnd = measureStartMicros;
      }
      
      if (drive > 0 ) {
        digitalWrite(Pin_IO_02,LOW);
        digitalWrite(Pin_IO_01,HIGH);
        measureStopMicros = micros();
        measureStopMillis = millis();
        Value_IO_01 = analogRead(Pin_IO_03);
        if (isZeroTime && 0 == Value_IO_01) {
          zeroTimeEnd = measureStopMicros;        
          isZeroTime = 0;
        }
      } else {
        digitalWrite(Pin_IO_01,LOW);
        digitalWrite(Pin_IO_02,HIGH);
        measureStopMicros = micros();
        measureStopMillis = millis();
        Value_IO_01 = analogRead(Pin_IO_03) - 4095;
        if (isZeroTime && 0 == Value_IO_01) {
          zeroTimeEnd = measureStopMicros;        
          isZeroTime = 0;
        }
      }
    }
  }

    
  /*
  digitalWrite(Pin_Out01,LOW);
  delay(500);                    

  int Value_In01 = analogRead(Pin_In01);
  unsigned long int startTime = millis();
  unsigned long int now = startTime;
  digitalWrite(Pin_Out01,HIGH);

  while ((Value_In01 < 1200) && (now - startTime < 500))
  {
    Value_In01 = analogRead(Pin_In01);
    unsigned long int now = startTime;
  }
  Serial.println(now - startTime);
  */


  
}

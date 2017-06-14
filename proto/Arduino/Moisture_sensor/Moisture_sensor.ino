/*
 * Moisture sensor test OSC4 algorithm
*/


// I2C screen :
// Taille du module: 27.0 x 27.0 x 4.1 MM;
// Interface I2C/IIC, besoin 2 IO uniquement;
// Driver IC: SSD1306
// Ultra faible consommation d'énergie: plein écran allumé 0.08W;
// Tension: 3V ~ 5V DC;
// La température fonctionnante:-30 ° c ~ 70 ° c

const long int minMicros = 0;
const long int maxMicros = 0x1111111;

const long int SAMPLING_DELAY = 200;
const long int RUN_TIME = 60000;
const long int REPORT_TIME = 60000;
const long int CYCLE_TIME[4] = {10,490,10,490};


const byte Pin_IO_1a = 22;
const byte Pin_IO_1b = 23;
const byte Pin_IO_2a = 12;
const byte Pin_IO_2b = 13;

const int Pin_LED = 4;

byte driveMode = -1;
long int  driveValue = 0;
unsigned long int bootMs;
unsigned long int lastLoopUs;
unsigned long int totalElapsedMs = 0;
unsigned long int modeElapsedMs = 0;
unsigned long int reportElapsedMs = 0;
unsigned long int measureElapsedMs = 0;
unsigned long int measureElapsedUs = 0;

/*
double measureTimeFSum = 0.0;
double measureTimeRSum = 0.0;
double measureTimeSum = 0.0;
*/
double measureRange1FSum = 0.0;
double measureRange1RSum = 0.0;
double measureRange1Sum = 0.0;
long int measure1Count = 0;

double measureRange2FSum = 0.0;
double measureRange2RSum = 0.0;
double measureRange2Sum = 0.0;
long int measure2Count = 0;

double measureRange3FSum = 0.0;
double measureRange3RSum = 0.0;
double measureRange3Sum = 0.0;
long int measure3Count = 0;

double measureRange4FSum = 0.0;
double measureRange4RSum = 0.0;
double measureRange4Sum = 0.0;
long int measure4Count = 0;

long int sensorValue1;
long int sensorValue2;
long int sensorValue3;


void readSensor() {
  sensorValue1 = (long int)analogRead(Pin_IO_2a);    
  sensorValue2 = (long int)analogRead(Pin_IO_2b);    
  sensorValue3 = sensorValue1 - sensorValue2;
}


void handleTime() {
  long int nowUs = micros(); 
  long int nowMs = millis(); 
  
  measureElapsedUs += microsDiff(nowUs,lastLoopUs);
  lastLoopUs = nowUs;

  if (measureElapsedUs >= 1000) {
    unsigned long int elapsedMs = measureElapsedUs / 1000;
    measureElapsedUs = measureElapsedUs % 1000;

    totalElapsedMs += elapsedMs;
    modeElapsedMs += elapsedMs;
    measureElapsedMs += elapsedMs;
    reportElapsedMs += elapsedMs;

    if (modeElapsedMs == 1) {
      handleSensor1();
    }
    else if (modeElapsedMs == 2) {
      handleSensor2();
    }
    else if (modeElapsedMs == 4) {
      handleSensor3();
    }
    else if (modeElapsedMs == 8) {
      handleSensor4();
    }
    else if (modeElapsedMs >= CYCLE_TIME[driveMode]) {
      switchMode();
    }
  }
}


void handleSensor1() {
  if (driveValue > 0) {
    readSensor();
    measureRange1FSum += (double)sensorValue3;
    measureRange1Sum  += (double)sensorValue3;       
    measure1Count++;    
  } else if (driveValue < 0) {
    readSensor();
    measureRange1RSum += -(double)sensorValue3;
    measureRange1Sum  += -(double)sensorValue3;      
    measure1Count++;    
  } else {
    // Idle time. Nothing to measure here
  }
}

void handleSensor2() {
  if (driveValue > 0) {
    readSensor();
    measureRange2FSum += (double)sensorValue3;
    measureRange2Sum  += (double)sensorValue3;       
    measure2Count++;    
    /// ToDo: Count R/F count separately 
  } else if (driveValue < 0) {
    readSensor();
    measureRange2RSum += -(double)sensorValue3;
    measureRange2Sum  += -(double)sensorValue3;      
    measure2Count++;    
  } else {
    // Idle time. Nothing to measure here
  }
}

void handleSensor3() {
  if (driveValue > 0) {
    readSensor();
    measureRange3FSum += (double)sensorValue3;
    measureRange3Sum  += (double)sensorValue3;       
    measure3Count++;    
  } else if (driveValue < 0) {
    readSensor();
    measureRange3RSum += -(double)sensorValue3;
    measureRange3Sum  += -(double)sensorValue3;      
    measure3Count++;    
  } else {
    // Idle time. Nothing to measure here
  }
}

void handleSensor4() {
  if (driveValue > 0) {
    readSensor();
    measureRange4FSum += (double)sensorValue3;
    measureRange4Sum  += (double)sensorValue3;       
    measure4Count++;    
  } else if (driveValue < 0) {
    readSensor();
    measureRange4RSum += -(double)sensorValue3;
    measureRange4Sum  += -(double)sensorValue3;      
    measure4Count++;    
  } else {
    // Idle time. Nothing to measure here
  }
}

void switchMode() {
  driveMode++;
  modeElapsedMs = 0;
  if (driveMode>3) {
    driveMode = 0;
  }
  
  switch (driveMode) {
    case 0: forwardMode();  break;
    case 1: offMode1();      break;
    case 2: reverseMode();  break;    
    case 3: offMode2();      break;
  }
}

void forwardMode() {
  //Serial.println('F');
  digitalWrite(Pin_LED,HIGH);  

  driveValue = 4095;
  digitalWrite(Pin_IO_1b,LOW);
  digitalWrite(Pin_IO_1a,HIGH);
}

void offMode1() {
  //Serial.println('1');
  digitalWrite(Pin_LED,LOW);  
  
  driveValue = 0;
  digitalWrite(Pin_IO_1a,LOW);
  digitalWrite(Pin_IO_1b,LOW);

}

void reverseMode() {
  //Serial.println('R');
  digitalWrite(Pin_LED,HIGH);  

  driveValue = -4095;
  digitalWrite(Pin_IO_1a,LOW);
  digitalWrite(Pin_IO_1b,HIGH);

}

void offMode2() {
  //Serial.println('2');
  digitalWrite(Pin_LED,LOW);  

  driveValue = 0;
  digitalWrite(Pin_IO_1a,LOW);
  digitalWrite(Pin_IO_1b,LOW);
}

void handleReport() {
  if (reportElapsedMs >= REPORT_TIME) {
    reportElapsedMs  = 0;
    sendReport();
  }
}

void sendReport() {
  //if (totalElapsedMs < RUN_TIME) {
    Serial.print(">");
    Serial.print((double)(totalElapsedMs)/3600000,14);
    
    Serial.print("|");
    Serial.print(measure1Count);
    Serial.print("|");
    Serial.print(measureRange1FSum/measure1Count);
    Serial.print("|");
    Serial.print(measureRange1RSum/measure1Count);
    Serial.print("|");
    Serial.print(measureRange1Sum/measure1Count);

    Serial.print("|");
    Serial.print(measure2Count);
    Serial.print("|");
    Serial.print(measureRange2FSum/measure2Count);
    Serial.print("|");
    Serial.print(measureRange2RSum/measure2Count);
    Serial.print("|");
    Serial.print(measureRange2Sum/measure2Count);

    Serial.print("|");
    Serial.print(measure3Count);
    Serial.print("|");
    Serial.print(measureRange3FSum/measure3Count);
    Serial.print("|");
    Serial.print(measureRange3RSum/measure3Count);
    Serial.print("|");
    Serial.print(measureRange3Sum/measure3Count);

    Serial.print("|");
    Serial.print(measure4Count);
    Serial.print("|");
    Serial.print(measureRange4FSum/measure4Count);
    Serial.print("|");
    Serial.print(measureRange4RSum/measure4Count);
    Serial.print("|");
    Serial.print(measureRange4Sum/measure4Count);

    Serial.println();

    measure1Count = 0;
    measureRange1FSum = 0;
    measureRange1RSum = 0;
    measureRange1Sum = 0;

    measure2Count = 0;
    measureRange2FSum = 0;
    measureRange2RSum = 0;
    measureRange2Sum = 0;

    measure3Count = 0;
    measureRange3FSum = 0;
    measureRange3RSum = 0;
    measureRange3Sum = 0;

    measure4Count = 0;
    measureRange4FSum = 0;
    measureRange4RSum = 0;
    measureRange4Sum = 0;
  //}
 }

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(115200);
  Serial.println("begin");
  Serial.println("");
  delay(1000);
  Serial.println("");
  
  // Time : sample time in hour (one sample = average of all measures during a 60 s period)
  // Cx   : count of measures for the sample at x ms after pulse start
  // SxF  : sample at x ms after pulse start in forward mode
  // SxR  : sample at x ms after pulse start in reverse mode
  // Sx   : combined sample at x ms after pulse start (SxF + SxR)

 
  Serial.println("!Time|C1|S1F|S1R|S1|C2|S2F|S2R|S2|C4|S4F|S4R|S4|C8|S8F|S8R|S8");

  pinMode(Pin_IO_2a, INPUT);
  pinMode(Pin_IO_2b, INPUT);
  pinMode(Pin_IO_1a, OUTPUT);
  pinMode(Pin_IO_1b, OUTPUT);
  pinMode(Pin_LED, OUTPUT);


  lastLoopUs = micros();
  switchMode();
}


long int microsDiff(const  long int a, const  long int b) {
  return (a >= b) ? (a - b) : ((a - minMicros) + (maxMicros - b));
}


// the loop function runs over and over again forever
void loop() {
  handleTime();
  //handleSensor();
  handleReport();    
}




void setup() {
  // put your setup code here, to run once:
  pinMode(A0,INPUT);  
  pinMode(A1,INPUT);  
  pinMode(LED_BUILTIN,OUTPUT);  
  Serial.begin(115200);
  Serial.println("");
  delay(3000);
  Serial.println("|T;V1;V2");
}

byte ledState = false;

void loop() {
  if (ledState) {
    digitalWrite(LED_BUILTIN,HIGH);
    ledState = false;
  } else {
    digitalWrite(LED_BUILTIN,LOW);
    ledState = true;
  }
  long int V1 = analogRead(A0);
  long int V2 = analogRead(A1);
  Serial.print('>');
  Serial.print(millis()/1000);
  Serial.print(';');
  Serial.print(V1);
  Serial.print(';');
  Serial.print(V2);
  Serial.println("");
  delay(1000);
}

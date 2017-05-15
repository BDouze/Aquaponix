import processing.serial.*;

int LF = 10;
Serial inputPort;  // Create object from Serial class
String inputLine;
String header;
PrintWriter outputFile;

int fieldCount = 0;


void setup() 
{
  size(800, 600);
  frameRate(1);
  
  String fileName = "data.csv";
  outputFile = createWriter(fileName);
  
  //String portName = Serial.list()[0];
  //String portName = "/dev/cu.wchusbserial1410"; // Arduino nano clone 
  String portName = "/dev/cu.SLAB_USBtoUART"; // ESP32 Dev Board
  inputPort = new Serial(this, portName, 115200);
  //port.clear(); 
  //inputLine = inputPort.readStringUntil(end); // function that reads the string from serial port until a println and then assigns string to our string variable (called 'serial')
  //inputLine = null; // initially, the string will be null (empty)
  inputPort.bufferUntil(LF);
}



void draw() {
  background(0);
  if (null != header && 0 < header.length()) {
  text(header, 10,50);
  }
  if (null != inputLine && 0 < inputLine.length()) {
    text(inputLine, 10,100);
  }
}

// Handler called when serial port buffer condition is met (when a LF is received)
void serialEvent(Serial p) {
  // Get serial port buffer content
  inputLine = p.readString();
  if (null == inputLine || 1 >= inputLine.length() || ('>' != inputLine.charAt(0) && '!' != inputLine.charAt(0))) {
    inputLine = "Partial or corrupted line";
  } else {
    boolean isHeader = ('!' == inputLine.charAt(0));
    // remove the first char ('!' or '>') and the last char (LF) of the received line
    inputLine = inputLine.substring(1,inputLine.length()-2);
    // write input line to output file
    outputFile.println(inputLine);
    // Flush the output file after each line to increase the chances of having the latest data written to the file in case of a crash 
    // This will decrease perf if incomming data rate is very high
    // It's OK for one input line per second or slower. It' probably not OK for 100 lines per second or more.
    // This could be improved to flush only once per second whatever the input data rate is.
    outputFile.flush(); 
    
    if (isHeader) {
      //The input line is a header line
      //keep the last header line  
      header = inputLine;
      inputLine = null;
      //tokens = splitTokens(header, ";");
    }
  }
  
}

void keyPressed() { // Press a key to save the data
  if ('q' == key) {
    outputFile.flush(); // Write the remaining data
    outputFile.close(); // Finish the file
    exit(); // Stop the program
  }
}
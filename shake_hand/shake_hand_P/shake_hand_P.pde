//Arduino和Processing都可以发送和接收数据shake hand
import processing.serial.*; //import the Serial library
 Serial myPort;  //the Serial port object
 String val;
int value=0;//窗口颜色
boolean ledState = false; //to toggle our LED
// since we're doing serial handshaking, 
// we need to check if we've heard from the microcontroller
boolean firstContact = false;

void setup() {
  size(200, 200); //make our canvas 200 x 200 pixels big
  textSize(14);fill(0, 102, 153);
  text("mouseClik change the LED",10,30);
  //  initialize your serial port and set the baud rate to 9600

  myPort = new Serial(this,"COM3", 9600);
  myPort.bufferUntil('\n'); 
}

void draw() {
  //we can leave the draw method empty, 
  //because all our programming happens in the serialEvent (see below)
    if (mousePressed == true) 
    {                           //if we clicked in the window
      if(ledState){
        value = 255;
      } else {
        value = 0;
      }
      fill(value);                     // change color black
      rect(50, 50, 100, 100);          // Draw a square 
    }
    //else {
    //  fill(255);                     // change color white
    //  rect(50, 50, 100, 100);          // Draw a square 
    //}
}

void serialEvent( Serial myPort) {
//put the incoming data into a String - 
//the '\n' is our end delimiter indicating the end of a complete packet
val = myPort.readStringUntil('\n');
//make sure our data isn't empty before continuing
if (val != null) {
  //trim whitespace and formatting characters (like carriage return)
  val = trim(val);
  println(val);

  //look for our 'A' string to start the handshake
  //if it's there, clear the buffer, and send a request for data
  if (firstContact == false) {
    if (val.equals("A")) {
      myPort.clear();
      firstContact = true;
      myPort.write("A");
      println("contact");
    }
  }
  else { //if we've already established contact, keep getting and parsing data
    println(val);

    if (mousePressed == true) 
    {                           //if we clicked in the window
      myPort.write('1');        //send a 1
      ledState = !ledState; //flip the ledState
      println("LED");

    }

    // when you've parsed the data you have, ask for more:
    myPort.write("A");
    }
  }
}

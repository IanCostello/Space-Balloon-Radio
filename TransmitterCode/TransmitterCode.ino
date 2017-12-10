//These filters should match start receiver code and are used for filtering out meaningful data from noise
const String START_FILTER = "nXTs4";
const String END_FILTER = "PuoWQ";
int currPacket = 0; //Test sender code

void setup() {
  //Begin listening on serial port (RX-0)
  Serial.begin(4800);
}

void loop() {
  //Increment current packet and send
  currPacket += 1;
  send((String) currPacket);

  //Iterate back to start after a set value
  if (currPacket  rr > 100000) {
    currPacket = 0;
  }
}

void send(String message) {
  //Turn on transmit mode
  digitalWrite(3, HIGH);
  delay(10);
  String message = START_FILTER + toSend + END_FILTER;
  String toSend = message;
  
  //Send message multiple times so less susceptible to corruption
  for (int i = 0; i < 10; i += 1) {
    toSend += message;
  }

  //Transmit message over serial
  Serial.print(toSend);

  //Give radio time to send
  delay(10);

  //Turn off transmit mode
  digitalWrite(3, LOW);
  delay(10);
}


const int INLENGTH = 500;  //Message length
const String NULL_TOKEN = "0000"; //No message break sequence
const int ANALOG_PIN = 0; //Analog pin on the arduino

//These should match start transmitter code  and are used for filtering out meaningful data from noise
String START_FILTER = "nXTs4"; 
String END_FILTER = "PuoWQ";

/** Returns if a message contains only valid [A-Z0-9] 
 *  plus some punctuation characters */
bool validMessage(String message) {
  for (int i = 0; i < message.length(); i += 1) {
    char currChar = message.charAt(i);
    //ASCII Character code 31 to 124
    if (currChar < 31 && currChar >  124) {
      return false;
    }
  }
  return true;  
}

/** Extracts a message between a starter and ender */
String extract(String s, int depth) {
  bool foundEnder = false; //Break variable
  int starterLength = START_FILTER.length(); //Store for efficiency
  
  //Loop till starter found or end of message
  int currPos = 0;
  char ENDER_FIRST = END_FILTER.charAt(0);
  while (currPos < INLENGTH && !foundEnder) {
    for (int j = 0; j <= starterLength; j+=1) {
      //Reached end of starter so exit
      if (j == starterLength) {
        foundEnder = true;
        currPos += starterLength - 1;
        break;
      }

      //If sequence doesn't match then skip
      if (s.charAt(currPos + j) != START_FILTER.charAt(j)) {
         break;
      }
    }

    //Increment forward
    currPos++;
  }

  //Store start of new string for extraction
  int startOfMessage = currPos;

  //Loop Till End Sequence Found
  bool endReached = false;
  while (!endReached && currPos < s.length()) {
    char currChar = s.charAt(currPos);

    //Only enter end loop if first character matches for efficiency
    if (currChar = ENDER_FIRST) {
      for (int j = 0; j <= END_FILTER.length(); j += 1) {
        //Gets to end of ender sequence so exit
        if (j == END_FILTER.length()) {
          endReached = true;
        }

        //Break if ender doesn't match
        if (s.charAt(currPos + j) != END_FILTER.charAt(j)) {
          break;
        }
      }
    }

    //Otherwise increment position
    if (!endReached) {
       currPos += 1;
    }
  }

  //No Message found so return null
  if (currPos - startOfMessage < 2) {
     return NULL_TOKEN;

  //Otherwise return the inner message
  } else {
    //check if contains valid characters or start over
    String message = s.substring(startOfMessage, currPos);
    if (validMessage(message)) {
      return message;
    } else {
      if (depth > 10) {
        return "max depth" + message;
      } else {
        return extract(s.substring(currPos, s.length()), depth + 1); 
      }
    }
  }
}

void setup() {
  Serial.begin(4800);
}

void loop()
{
  //Clear serial
  Serial.flush();
  delay(1);
  char inString[INLENGTH+1]; //Message
  int inCount = 0;

  //Loop till full message is read
  do {
    //Wait for incoming message on serial
    while(!Serial.available());

    //React next char in
    inString[inCount] = Serial.read();

  //Increment count and exit after correct length
  } while (++inCount < INLENGTH);

  //Terminate c-string and convert into String object
  inString[inCount] = '\0';
  String s = String(inString);

  //Extract message
  s = extract(s, 0);

  //Print message if received 
  if (s == NULL_TOKEN) {
    Serial.println("No Message");
  } else {
    Serial.println(s);
  }

  //Display max signal strength over past 1/2 second
  int max = 0;
  int signalStrength = 0;
  for (int i = 0; i < 50; i += 1) {
     analogReference(EXTERNAL); //Enable AREF pin to set full scale input voltage to 2.5V
     signalStrength = analogRead(ANALOG_PIN);
     if (signalStrength > max) {
      max = signalStrength;
     }
     delay(10);
  }

  //Print
  Serial.println("Voltage " + (String) signalStrength);
}


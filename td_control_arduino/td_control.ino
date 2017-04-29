// TD1208 Module Control Code
// code was used for the purpose of experimenting with sigfox IOT network communication
// send AT commands to the TD1208's UART which enables TX'ing and RX'ing over network

// need means of extracting received data
// 7.13, 5:15pm: can stay in receiving function until RX END has been received, but sometimes part of downlink is missed
// 7/13: created receiving function, will work on timing there
// 7/10, 5:40pm: received characters are outputted correctly and with good formatting, will separate into function
// 7/10, 5pm: it would probably be best to remove the receive data stuff from the send data function


int send_pin = 52;
int receive_pin = 53;
int sendingLED = 51;
int receivingLED = 50;
// for testing receiving methods
bool receipt = 0; 

void setup() {
  pinMode(13, OUTPUT); // for LED indication
  Serial1.begin(9600); // allows communication over ports 18 & 19
  Serial.begin(9600); // allows communication with the PC2

  // pins connected to switches, will trigger send and receive functions
  pinMode(send_pin, INPUT_PULLUP); 
  pinMode(receive_pin, INPUT_PULLUP);

  // indicator lights
  pinMode(sendingLED, OUTPUT); 
  pinMode(receivingLED, OUTPUT);
}


void loop() {
  digitalWrite(13, LOW); // at the start of the loop, turn off LED

  // hardcoded AT command to send
  char* command = "AT$SB="; 
  // parameters: <single bit>, <rep (2 up) or (1)>, <up (0) /down (1)>, <reserved 0>
  char* data_to_send = "1,2,1,0"; 
  // to hold received characters from transceiver
  char incoming; 
  // to hold extracted authorization code
  //char* auth_code [16]; 

  /*send message to transceiver*/
  // data to send should be 12 bytes (or less - puts 0 on the MSB) hexadecimal transformed to a chain of character
  // inherited function from example code, sends out AT command
  sig_send(command, data_to_send); 
  /*after sending message*/

  sig_receive(incoming);
  Serial.print("authorization code:");
  for (int f = 0; f < 16; f++) {
    Serial.print(auth_code[f]);
  }

  // wait 10 secs before sending another message
  delay(10000); 
}

String sig_send(char* command, char* data) {
  digitalWrite(sendingLED, HIGH);
  String protocol = command;
  protocol += (String)data;
  protocol += "\r";
  // wait before writing command
  delay(2000); 
  
  // write out AT command to transceiver
  Serial1.print(String(protocol)); 
  Serial.println("To transceiver: ");
  // sanity check
  Serial.println(String(protocol)); 
  delay(1000);
  // turn on LED to indicate function has run
  digitalWrite(13, HIGH); 
  digitalWrite(sendingLED, LOW);

  return "OK";
}

String sig_receive (char incoming) {
  // store incoming values in this array
  char bank[100]; 
  int i = 0; 
  bool receipt = 1;
  char auth_code[16];
  
  /*receiving mode*/
  while (receipt == 1) {

    digitalWrite(receivingLED, HIGH);
    
    /*line 1*/
    // so it doesn't print again on the way back
    Serial.println("\rFrom transceiver:"); 
    // msg comes after about 5 seconds, read while there then print
    delay(5000); 
    while (Serial1.available()) {
      incoming = Serial1.read();
      bank[i] = incoming;
      i += 1;
      Serial.print(incoming);
    }
    Serial.print("\r");

    /*line 2*/
    delay(16000);
    while (Serial1.available()) {
      incoming = Serial1.read();
      bank[i] = incoming;
      i += 1;
      Serial.print(incoming);
    }
    Serial.print("\r");

    /*line 3*/
    delay(13000);
    while (Serial1.available()) {
      incoming = Serial1.read();
      bank[i] = incoming;
      i += 1;
      Serial.print(incoming);
    }
    Serial.print("\r");

    /*line 4*/
    delay(3500);
    while (Serial1.available()) {
      incoming = Serial1.read();
      bank[i] = incoming;
      i += 1;
      Serial.print(incoming);
    }
    Serial.print("\r");

    delay(3500);
    while (Serial1.available()) {
      incoming = Serial1.read();
      bank[i] = incoming;
      i += 1;
      Serial.print(incoming);
    }
    Serial.print("\r");

    // done receiving for the moment
    digitalWrite(receivingLED, LOW); 
    Serial.println(i);

    if (i > 60) {
      Serial.println("gonna check the bank homie....");
      
      /*Verify message has been fully received*/
      for (int c = 0; c < 100; c++) {
        Serial.print("'");
        Serial.print(bank[c]);
        
        if (bank[c] == 68) {
          // if this is ture, then RX END has been received, that means the 16 bit message should be extracted too
          Serial.println("\r extracting the code homie"); 

          // will use loop to cycle through received values again to find sequence of downlink data
          for (int d = 0; d < 100; d++) {
            // asci chars 88(X) and 61(=) precede the sequence of received data
            if ((bank[d] == 88 ) && (bank[(d + 1)] == 61 )) {      
              // move through data sequence  
              for (int e = 0; e <  23; e++) {  
                // put value into array only if the value isn't a space 
                if (bank[(d + 2 + e)] != 32) { 
                  // record 16 digits into auth code if they're actual values
                  for (int g = 0; g < 16; g++) {
                    auth_code[g] = bank[(d + 2 + e)]; 
                  }
                }
              }
              
            }
          }
          
          Serial.print("authorization code:");
          for (int f = 0; f < 16; f++) {
            Serial.print(auth_code[f]);
          }
          digitalWrite(receivingLED, LOW);
          digitalWrite(sendingLED, HIGH);
          Serial.println("");
          receipt = 0;
          i = 0;
          return auth_code;
        }
        
        // remain in loop until count reaches proper length
        else {
          receipt = 1; 
          digitalWrite(receivingLED, HIGH);
          digitalWrite(sendingLED, HIGH);
        }
        
      }
    
    }
  }

}




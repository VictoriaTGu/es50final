	/*
	This sketch captures data from NeuroSky MindWave Mobile and save it on SD card (by 
	means of Adafruit datalogging shield for Arduino).
	-- See TGAM1_Communication_Protocol pdf from NeuroSky for details on the communication 
	protocol and packet structure of NeuroSky MindWave Mobile.
	-- See ladyada.net for details on the Logger shield.
	Feel free to download a complete description of the project from: 
	March-April 2013
	Salvino A. Salvaggio - salvino@salvaggio.net - www.salvaggio.net
	*/
	
        #include <Servo.h>
	#define BAUDRATE 57600
	// define variable used for debug (when = 1, all data are also shown on the serial monitor).
	// If you don't need to show all data on serial monitor, modify this line as #define ECHO 0
	#define ECHO 1

        // Ping Pong Machine Code
        const int motorpin = 9;
        const int servopin = 6;
        const int rawserial = 0;
        
        int num_raise = 0;
        int num_lower = 0;
        

        Servo myservo;                           

	//#define redLEDpin 3
	//#define greenLEDpin 2
	byte pinLED[2] = {10, 11} ; //define pins to connect embedded LEDs (pin 2 is green LEDs, pin 3 is red LED)

	
	// checksum variables
	byte generatedChecksum = 0;
	byte checksum = 0; 
	int payloadLength = 0;
	byte payloadData[64] = {0};
	byte poorQuality;
	byte attention;
	byte meditation;
	byte j ;
	// system variables
	long lastReceivedPacket = 0;
	long timeStamp = 0 ;
	long nunc = 0 ; // "nunc" means "now" in Latin. I used nunc to avoid confusion with the now command from the RTC library
	int packetCount = 0 ;
	boolean bigPacket = false;

        unsigned long wave[7];
        
        /* We don't use it, but could be cool: 
        wave array captures data as follows:
        wave[0] --> delta
	wave[1] --> theta
	wave[2] --> lowAlpha
	wave[3] --> highAlpha
	wave[4] --> lowBeta
	wave[5] --> highBeta
	wave[6] --> lowGamma
	wave[7] --> midGamma
	*/
        ////////////////////////////////
		// Read data from Serial UART //
		////////////////////////////////
		byte ReadOneByte() {
		  int ByteRead;
		  while(!Serial.available());
		  ByteRead = Serial.read();
		  return ByteRead;
		}
  
        // Ping Pong Code
        void servospin(int angle){
          myservo.attach(servopin);
          myservo.write(angle);
          //delay(100);
          myservo.detach();
        }

        //If there's no attention - i.e. not connected- pin 11 will blink. else pin 10 will blink
        
	void flashLED(char myChar) { 
            //j = which pin to flash (from the array): flashLED(G) --> 10
	    //flashLED(R) --> 11;
	if (myChar == 'G') {
	  j = 0 ;
	 } else {
	  j = 1 ;
	 }
	digitalWrite(pinLED[j], HIGH);
	delay(100);
	digitalWrite(pinLED[j], LOW);
	delay(100);
	digitalWrite(pinLED[j], HIGH);
	delay(100);
	digitalWrite(pinLED[j], LOW); 
	}
		//////////////////////////
		// Setup
		//////////////////////////
		void setup() {
                  digitalWrite(pinLED[j], LOW); 
		  Serial.begin(BAUDRATE); // USB
		  // initialize LEDs
		  pinMode(pinLED[0], OUTPUT);
		  pinMode(pinLED[1], OUTPUT);
		  digitalWrite(pinLED[0], LOW);
		  digitalWrite(pinLED[1], LOW);
		  // make sure that the default chip select pin is set to
		  // output, even if you don't use it:
		  pinMode(10, OUTPUT);
                  pinMode(motorpin, OUTPUT); 
                  
		}

		void loop() {
		  if(ReadOneByte() == 170) { // check 1st byte is sync byte (value must be 170)
		  if(ReadOneByte() == 170) { // check 2nd byte is also sync byte (value must be 170)
		  payloadLength = ReadOneByte(); // 3rd byte is payload length
		  if(payloadLength > 6) bigPacket = true; // check the packet is a long packet, not a raw data packet
		  if(payloadLength > 169) 
		    return;
		// check the checksum comparing checksum as in the packet to calculated checksum
		generatedChecksum = 0; 
		for(int i = 0; i < payloadLength; i++) { 
		  payloadData[i] = ReadOneByte(); 
		  generatedChecksum += payloadData[i];
		} 
		checksum = ReadOneByte(); 
		generatedChecksum = 255 - generatedChecksum; 
		if((checksum == generatedChecksum) && (bigPacket == true)) {
		  // end check the checksum
		  packetCount = packetCount + 1 ;
		  //reconstruct the value of waves based on big-endian 3-byte unsigned integer 
		byte a = 4 ;
		for (int i = 0 ; i < 8 ; i++ ) {
		  wave[i] = payloadData[a] << 8 ;
		  wave[i] = ((wave[i] + payloadData[a+1]) << 8) + payloadData[a+2] ;
		  a = a + 3;
		}

		// get time since last big packet 
		nunc = millis() - lastReceivedPacket ;
		lastReceivedPacket = millis();
		timeStamp = timeStamp + nunc ;

                int attention = payloadData[29];
		Serial.print(payloadData[29],DEC);
		Serial.print(",");
		Serial.print(payloadData[31],DEC);
                Serial.print(",");
                Serial.println();
		Serial.flush();
		if (payloadData[29] == 0) {
		  flashLED('R');
		} else {
		  flashLED('G');
		}

                // ping pong code 
                /*if (Serial.available()) {
                 /* rawserial = Serial.read();
                if (rawserial == '1')
                  servospin(85);
                else if (rawserial == '2')
                  servospin(95);
                  servospin((attention/5) + 80);
                } */
                
                //servospin((attention/5) + 80);
                
                servospin(70);
                Serial.print("Spin");
                Serial.println();
                Serial.flush();
                if (attention == 0){
                  //analogWrite(motorpin, 0);
                  Serial.print("Attention 0");
                  Serial.println();
                  Serial.flush();
                }
                else{
                  // start the motor
                  analogWrite(motorpin, 255);
                } 
                // Servo control
                if (attention > 65){
                   // raise ramp at most 3 times consecutively
                   if(num_raise < 4){
                       servospin(80);
                       num_raise += 1;
                       Serial.print("Raise ");
                       Serial.print(num_raise);
                       Serial.println();
                       Serial.flush();
                       
                       // can now lower one more time
                       if(num_lower > 0){
                         num_lower -= 1;
                       }
                     }
                 }
                 else{
                   if (attention < 40 && attention > 0){
                     // lower ramp at most 3 times consecutively
                     if(num_lower < 4){
                       servospin(100);
                       num_lower += 1;
                       Serial.print("Lower ");
                       Serial.print(num_lower);
                       Serial.println();
                       Serial.flush();
                       
                       // can now raise one more time
                       if(num_raise > 0){
                         num_raise -= 1;
                       }
                     }
                   }
                 }
                 


	        }
	      }
	    }
	  bigPacket = false;
	  }

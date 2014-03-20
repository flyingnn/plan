///////////控制板IO/////////////

//电机控制端口设置
#define Relay1  2	//   
#define Relay2  3 	//
#define Relay3  4 	//
#define Relay4  5 	//

int incomingByte = 0;	

void setup() {
	Serial.begin(9600);
  	int i;
  	for (i=2; i<=5; i++) { 
	  	pinMode(i, OUTPUT); //设置控制电机的各端口为输出模式   
	}
}

void loop() {
    if (Serial.available() > 0) {
        incomingByte = Serial.read();
        switch (incomingByte) {
           case 0x00:
                digitalWrite(Relay1, LOW);
              	break;     
           case 0x01:
                digitalWrite(Relay1, HIGH);
              	break;
           case 0x10:
                digitalWrite(Relay2, LOW);
              	break;     
           case 0x11:
                digitalWrite(Relay2, HIGH);
              	break;
           case 0x20:
                digitalWrite(Relay3, LOW);
              	break;     
           case 0x21:
                digitalWrite(Relay3, HIGH);
              	break;
           case 0x30:
                digitalWrite(Relay4, LOW);
              	break;     
           case 0x31:
                digitalWrite(Relay4, HIGH);
              	break;
           default:
              	break;      
        }
    }
}
//Serial.flush();








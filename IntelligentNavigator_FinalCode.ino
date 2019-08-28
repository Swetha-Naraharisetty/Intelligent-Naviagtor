
#include  <Wire.h>
#include <Keypad.h>
#define EEPRROM_ADDR 0x50
#define LEFT         0x01
#define RIGHT        0x02
#define STRAIGHT     0x03

int i, j = 0, k = 0, l = 0;
int dir_arr[10];
int delay_arr[10];
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'E','0','F','D'}
};
byte rowPins[ROWS] = {29,27,25,23}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {37,35,33,31}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

#define trigPin 12
#define echoPin 13
unsigned long previousTTime = 0;
int byte_cnt;
int pir_sensor = 5;
int buzzer = 22;
byte Tseconds = 0 ;
char customKey;
boolean stop_working_mode=false;
int m1 = 24;
int m2 = 26;
int m3 = 28;
int m4 = 30;
int m5 = 32;
int m6 = 34;
int m7 = 36;
int m8 = 38;

int straight = 6;
int left = 7;
int right = 8;
int stpBtn = 2;
int mode= 11, strtBtn = 10;
boolean flag = false;
int index;
int index1 = 0, count;
char arr[20];
int dir[20];
float duration, distance, minDis = 10;
byte dir_status;
int formula,key;
String s = "";
char c;
byte delay_read_data;
byte store[100];
void setup() {
 pinMode(m1,OUTPUT);
  pinMode(m2,OUTPUT);
  pinMode(m3,OUTPUT);
  pinMode(m4,OUTPUT);
  pinMode(m5,OUTPUT);
  pinMode(m6,OUTPUT);
  pinMode(m7,OUTPUT);
  pinMode(m8,OUTPUT);
  Serial.begin(9600);
  Wire.begin();
  pinMode(pir_sensor,INPUT);//pir pins
  //buttons pins
  pinMode(straight,INPUT);
  pinMode(left,INPUT);
  pinMode(right,INPUT);
  pinMode(stpBtn, INPUT);
 
  pinMode(strtBtn, INPUT);
  pinMode(mode, INPUT);

 // Ultrasonic pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  pinMode(buzzer, OUTPUT);
  //default playback module states must be high
  digitalWrite(m1,HIGH);
  digitalWrite(m2,HIGH);
  digitalWrite(m3,HIGH);
  digitalWrite(m4,HIGH);
  digitalWrite(m5,HIGH);
  digitalWrite(m6,HIGH);
  digitalWrite(m7,HIGH);
  digitalWrite(m8,HIGH);
  Serial.println("wait...."); 
}



void loop() {
  // put your main code here, to run repeatedly:

  customKey = customKeypad.getKey(); 
      Serial.println(customKey ); 
      if(customKey!=0)// selecting the path
      {   
            buzzer_fun();
            while(1)
              {
                  int temp = digitalRead(mode); 
                  Serial.println("Working mode .. ");
                  if(temp == HIGH)// working mode
                      {
                       // read_data_from_eeprom() ;
                       while(digitalRead(strtBtn)!=HIGH);  // wait for start pulse
                          buzzer_fun();
                          Serial.print("  STARTING  ");
                          play_back(5);              
                          index = cal_formula(index, customKey);
                          Serial.println("Function calling ...................");
                          read_data_from_eeprom(index);
                          Serial.println("Function called ...................");
                          for(int i = 0; i < 10; i++)
                          {
                                Serial.println(delay_arr[i]);
                                W_elapsedTime(delay_arr[i]);
                                delay(5);
                                Serial.println(dir_arr[i]);
                                play_back(dir_arr[i]);  
                                delay(1000); 
                          }
                          play_back(6);                      
                                                
                      }
                  else
                      {
                        Serial.println("Training ...............");
                          // stop_trigger(); 
                           if(stop_working_mode == true)
                          {
                              Serial.println("ENDING.......");
                              play_back(6);
                              break;
                              
                           } 
                           while(digitalRead(strtBtn)!=HIGH);  // wait for start pulse
                           buzzer_fun();
                           Serial.print("  STARTING  ");
                           play_back(5);
                           arr[index]=elapsedtime(); // calculating the delay time
                           writeEEPROM(cal_formula(index,customKey ),  Tseconds ); // storing the delay time
                           Serial.print("    formula =  ");
                           Serial.println(formula);
                           delay(100);
                           index++;
                           int temp=0;
                           while(temp==0)
                               {
                                  temp = ((digitalRead(straight)<<3)|(digitalRead(right)<<2)|(digitalRead(left)<<1));
                               }
                           Serial.print("temp =");
                           Serial.println(temp);    
                           if(digitalRead(straight)== HIGH)
                               {
                                  dir_status = STRAIGHT; // 3
                               }
                           if(digitalRead(left)== HIGH)
                               {
                                  dir_status = LEFT; //1
                               }
                           if(digitalRead(right)== HIGH)
                               {
                                 dir_status = RIGHT; //2
                               }
                           Serial.print("index = ");
                           Serial.println(index);
                           writeEEPROM(cal_formula(index, customKey),  dir_status ); // storing the direction
                           index=index+2;
                           Serial.print("    dir status =  ");
                           Serial.println(dir_status);
                           Serial.print("    formula =  ");
                           Serial.println(formula);
                           play_back(dir_status);
                           delay(3000);
                           stop_trigger(); 
                           if(stop_working_mode == true)
                           {
                              Serial.println("ENDING.......");
                               play_back(6);
                              break;
                              
                           }
                      }     
               }
      }
      else
      {
          Serial.println(" plz press Button");  
      }

}

void read_data_from_eeprom( int index2) 
{

                    while(1){
                          Serial.print("index  = ");
                          Serial.print(index2);
                          delay_read_data = readEEPROM(index2);
                          
                          if(delay_read_data == 'E'){
                                 s +=  char(delay_read_data);
                                  s += char(readEEPROM(index2 + 1));
                                  s  += char(readEEPROM(index2 + 2));
                                  
                                   index2 = 0;
                                  
                                  Serial.print("s == ");
                                  Serial.println(s);
                                
                            }
                            if(s == "END")
                            {
                                Serial.print("s =   ");
                                Serial.println(s);
                                s = "";
                                index2 = 0;
                                k = 0;
                                l = 0;
                                Serial.println("ENDING.......");
                                break;
                            } else if(index2 % 3 == 0)
                            {
                              delay_arr[k] = delay_read_data;
                              index2++;
                              k++;    
                            }else if (index2 % 3 == 1)
                            {
                                   dir_arr[l] = delay_read_data;
                                   index2 = index2 + 2;
                                   l++;
                            }

                          Serial.print("index  = ");
                          Serial.print(index2);
                          Serial.print("data = ");
                          Serial.println(delay_read_data);
                            
                            delay(150);
                    }

}


byte readEEPROM(unsigned int eeaddress ) // to read the data from eeprom
{
  //Serial.print("   eeaddress = ");
  //Serial.println(eeaddress);
        byte data;
        address_send_eeprom(eeaddress);
        Wire.requestFrom(EEPRROM_ADDR,1);
        if (Wire.available()) {
              //Serial.print("eeaddress = ");
              //Serial.println(eeaddress);
              data = Wire.read();
             /// store[eeaddress]=data;
              //Serial.print("data = ");
             // Serial.println(data);
              //i++;
        }
        return data;
}




 void writeEEPROM(unsigned int eeaddress, byte data )  // to write data to eeprom
{
        Wire.beginTransmission(EEPRROM_ADDR);
        Wire.write((int)(eeaddress >> 8));   // MSB
        Wire.write((int)(eeaddress & 0xFF)); // LSB
       // Serial.print("eeaddress = ");
        //Serial.print(eeaddress);
        //Serial.print("         data = ");
        //Serial.println(data);
        Wire.write(data);
        Wire.endTransmission();
        delay(5);
}

void address_send_eeprom(unsigned int eeaddress) // to begin the transmission
  {
        //Serial.print("addresss");
        Wire.beginTransmission(EEPRROM_ADDR);
        Wire.write((int)(eeaddress >> 8));   // MSB
        Wire.write((int)(eeaddress & 0xFF)); // LSB
        Wire.endTransmission();
        
 
  }

byte elapsedtime()  //calculating the time in training mode
 {
     int temp = digitalRead(strtBtn);  
     Tseconds = 0; 
     if(temp ==HIGH)  // to check condition switch status
     { 
            Serial.println("start");
            while(temp == HIGH) // wait till switch is open
            {
                    temp = digitalRead(strtBtn); // to read the input from switch
                    if (millis() >= (previousTTime)) 
                    {
                         previousTTime = previousTTime + 1000;  // use 1000000 for uS
                         Tseconds = Tseconds +1;
                    } // end 1 second
            
            }
            //buzzer_fun();
             Serial.println("stop");
             Serial.print("Sec = ");
             Serial.println(Tseconds);
             return Tseconds;
     }
  else
          return 0;
 }



  void W_elapsedTime(byte temp1)    // calculating the time in working mode
{  
    unsigned long previousTime = 0;
    byte seconds = 0;
    int pir = digitalRead(pir_sensor);
    Serial.println("start");
    while(temp1 > seconds){
        if (millis() >= (previousTime)) 
            {
               previousTime = previousTime + 1000;  // use 1000000 for uS
               seconds = seconds +1;
               //checking for obstracle.....
               float distance = US_sensor();
              /* while(  distance < minDis) // checking for any human motion
               {
                    // Serial.println("human present"); 
                     Serial.print("distance   =  "); 
                     play_back(4);
                     Serial.println(distance); 
                     delay(500);
                     distance = US_sensor();
                  //   pir = digitalRead(pir_sensor);
               }*/
               while(distance < minDis ) // checking for static obstracle
               {
                     Serial.println("Obstacle Detected");
                     Serial.print("distance   =  "); 
                     play_back(4);
                     Serial.println(distance); 
                     distance = US_sensor();
                    // pir = digitalRead(pir_sensor);
            }
        } // end 1 second
        Serial.println(seconds);
        delay(1000);
    }
 }

 float US_sensor() // to identify any obstracle if any
{
        digitalWrite(trigPin, LOW); 
        delayMicroseconds(2);
       
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin, LOW);
        
        duration = pulseIn(echoPin, HIGH);
        distance = (duration / 58.2);
       
        return distance; 
}


void buzzer_fun()
{
  digitalWrite(buzzer,HIGH);
  delay(200);
  digitalWrite(buzzer,LOW);
  delay(50);
}

void stop_trigger() // to stop the mode
{
     if(digitalRead(stpBtn)==HIGH)
         {
               Serial.println("Ending Point");
               //index;
               writeEEPROM(cal_formula(index,customKey ),  'E' ); // storing the Ending point
               index++;
               writeEEPROM(cal_formula(index,customKey ),  'N' ); // storing the Ending point
               index++;
               writeEEPROM(cal_formula(index,customKey ),  'D' ); // storing the Ending point    
               index=0;  
               delay(1000);
               Serial.println("END Training");
               stop_working_mode=true;
         }
         else
              stop_working_mode=false;
 }

void play_back(int value)
{
  
 switch(value)
       {
                          case 1: digitalWrite(m1,LOW);
                                  delay(500);
                                  digitalWrite(m1,HIGH);
                                  delay(500);
                                  break;
                          case 2: digitalWrite(m2,LOW);
                                  delay(500);
                                  digitalWrite(m2,HIGH);
                                  delay(500);
                                  break;
                          case 3: digitalWrite(m3,LOW);
                                  delay(500);
                                  digitalWrite(m3,HIGH);
                                  delay(500);
                                  break;
                          case 4: digitalWrite(m4,LOW);
                                  delay(500);
                                  digitalWrite(m4,HIGH);
                                  delay(500);
                                  break;
                          case 5: digitalWrite(m5,LOW);
                                  delay(500);
                                  digitalWrite(m5,HIGH);
                                  delay(500);
                                  break;
                          case 6: digitalWrite(m6,LOW);
                                  delay(500);
                                  digitalWrite(m6,HIGH);
                                  delay(500);
                                  break;
                          case 7: digitalWrite(m7,LOW);
                                  delay(500);
                                  digitalWrite(m7,HIGH);
                                  delay(500);
                                  break;
                          case 8: digitalWrite(m8,LOW);
                                  delay(500);
                                  digitalWrite(m8,HIGH);
                                  delay(500);
                                  break;
                           
                  }  
                  digitalWrite(m1,HIGH);
                  digitalWrite(m2,HIGH);
                  digitalWrite(m3,HIGH);
                  digitalWrite(m4,HIGH);
                  digitalWrite(m5,HIGH);
                  digitalWrite(m6,HIGH);
                  digitalWrite(m7,HIGH);
                  digitalWrite(m8,HIGH);
}


int cal_formula(int i, char custom)    // calculating the position in the eeprom 
{ 
       int mod = 0,di = 0;
        mod = i % 3;
        di = i / 3;
        key = (custom - '0') - 1;
        formula = ((30 * key) + (3 * di) + mod);
        Serial.print("formula =  ");
        Serial.println(formula);
        return formula;
}

char keypad_return()
{
  customKey = customKeypad.getKey(); 
  return customKey;
}


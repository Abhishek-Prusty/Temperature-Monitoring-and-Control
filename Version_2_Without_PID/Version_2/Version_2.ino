// include the library code:
#include <LiquidCrystal.h>


// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


float tempC;
float tempF;
int pwmfan1 = 6;
int pwmfan2 = 9;

int pwmbulb = 10;

int reading;
float referenceVoltage;

int tempPin = 1; //Analog pin connected to LM35


int x=0;
int t=0;
int flag=0;
long long int userRead=0;
int tl=20;
int th=80;
int tcounter=0;
//int setTemp=(tl+th)/2;
float setTemp= 45;
//int inc=144;
//int dec=2192;
int logd=2640;

int logpin=A2;
int logcount=0;
int maxdata=500;
int data[500];
char rem = 0;
String tmp;
int tolerance=0;

void setup()
{
  
 Serial.begin(9600);
 analogReference(INTERNAL);
 referenceVoltage = 1.1; 
   pinMode(pwmfan1, OUTPUT);
   pinMode(pwmfan2, OUTPUT);
  pinMode(pwmbulb, OUTPUT);
  pinMode(logpin, INPUT);
  
 setPwmFrequency(pwmbulb, 1024);
 //setPwmFrequency(pwmfan, 1024);
  Serial.begin(9600); 
  
    lcd.begin(16, 2);
    lcd.setCursor(0,0);
    lcd.print("Set Temp: ");
    lcd.setCursor(0,1);
    lcd.print("Cur Temp: ");
  // Print a message to the LCD.
  
}

void loop()
{

  if (Serial.available()>0)
  {
    rem=Serial.read();
    if(rem=='t')
    {
      Serial.println("Enter required temperature: ");
      
 //     lcd.print("Enter required temperature: ");
     setTemp=0;
     while(setTemp==0)
     {
      tmp=Serial.readString();
      setTemp=tmp.toInt();

     }     
           Serial.print("Set Temp: ");
      Serial.print(setTemp);
      lcd.setCursor(10,0);
      lcd.print(setTemp);
      delay(100);
    }
  }
  
  reading = 0;
  for(int i = 0; i < 10; i++) 
  {
     reading += analogRead(tempPin); 
     delay(10);
  }
    tempC =  (referenceVoltage * reading * 10) / 1023;
   if(tempC > setTemp+tolerance)
  {
      {
        analogWrite(pwmbulb,0);
      }
      /*
      int factor=155/(tempC-tl);
      int fx=100+((tempC-setTemp)*factor);*/
     {
      analogWrite(pwmfan1,0);
      analogWrite(pwmfan2,0);
     }


  }
  else if(tempC<setTemp-tolerance)
  {
    analogWrite(pwmfan1,255);
    analogWrite(pwmfan2,255);

    
      int factor=(155)/(th-tempC);
      int fx=100+((setTemp-tempC)*factor);

     if(setTemp-tempC<=1)
     {
     analogWrite(pwmbulb,((setTemp)-tempC)*100+155);
     }
     else
     {
      analogWrite(pwmbulb,255); 
     }
  }
  
  // Convert to Fahrenheit
  tempF = (tempC * 9 / 5) + 32;
  Serial.print(tempC); 
  lcd.setCursor(10,1);
  lcd.print(tempC);
  Serial.println(" C");
  lcd.println("C");
  Serial.print("setTemp");
  Serial.print(setTemp);
  lcd.setCursor(10,0);
  lcd.print(setTemp);
  Serial.print(" C\r");
  lcd.println("C");
  
  Serial.println(" ");
  lcd.display();
  delay(500);
 // lcd.noDisplay();
}
void setPwmFrequency(int pin, int divisor) {
  byte mode;
  if(pin == 5 || pin == 6 || pin == 9 || pin == 10) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x03; break;
      case 256: mode = 0x04; break;
      case 1024: mode = 0x05; break;
      default: return;
    }
    if(pin == 5 || pin == 6) {
      TCCR0B = TCCR0B & 0b11111000 | mode;
    } else {
      TCCR1B = TCCR1B & 0b11111000 | mode;
    }
  } else if(pin == 3 || pin == 11) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 32: mode = 0x03; break;
      case 64: mode = 0x04; break;
      case 128: mode = 0x05; break;
      case 256: mode = 0x06; break;
      case 1024: mode = 0x7; break;
      default: return;
    }
    TCCR2B = TCCR2B & 0b11111000 | mode;
  }
 
}


/* By Quvonchbek Rayxonov */
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <virtuabotixRTC.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
virtuabotixRTC myRTC(6, 7, 8);

// Qo‘ng‘iroq vaqtlarini massiv ko‘rinishida saqlaymiz
int bellTimes[][2] = {
  {8, 30},
  {9, 15},
  {9, 20},
  {10, 5},
  {10, 10},
  {10, 55},
  {11, 0},
  {11, 45},
  {12, 30},
  {13, 15},
  {13, 20},
  {14, 5},
  {14, 10},
  {14, 55},
  {15, 0},
  {17, 0}
};

int bellCount = sizeof(bellTimes) / sizeof(bellTimes[0]);

void setup() {
  Serial.begin(9600);
  pinMode(2, OUTPUT);   
  pinMode(3, INPUT);    

  lcd.init();
  lcd.backlight();

  // Bir marta vaqt o‘rnatiladi va ushbu kodni komentaryiaga olib yana kod yuklanadi:
  // myRTC.setDS1302Time(10, 17, 12, 4, 27, 11, 2025);
}

void loop() {
  myRTC.updateTime();


  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Soat:");
  if (myRTC.hours < 10) lcd.print("0");
  lcd.print(myRTC.hours);
  lcd.print(":");
  if (myRTC.minutes < 10) lcd.print("0");
  lcd.print(myRTC.minutes);
  lcd.print(":");
  if (myRTC.seconds < 10) lcd.print("0");
  lcd.print(myRTC.seconds);


  Serial.print(myRTC.hours);
  Serial.print(":");
  Serial.print(myRTC.minutes);
  Serial.print(":");
  Serial.println(myRTC.seconds);

  schoolbell();
  nextbell();
  button();
  delay(200);
}

void schoolbell() 
{
  if (myRTC.dayofweek < 1 || myRTC.dayofweek > 6) return;

  for (int i = 0; i < bellCount; i++) 
  {
    if (myRTC.hours == bellTimes[i][0] &&
        myRTC.minutes == bellTimes[i][1] &&
        myRTC.seconds >= 1 && myRTC.seconds <= 4) {
      digitalWrite(2, HIGH);  
      tone(12, 500, 1000);
      delay(1000);
      noTone(12);
      digitalWrite(2, LOW);
      delay(1000);
    }
  }
}
void nextbell()
{
  
  bool found = false;
  if (myRTC.dayofweek < 1 || myRTC.dayofweek > 6) return;
  for (int i = 0; i < bellCount; i++) 
  {
    if (myRTC.hours < bellTimes[i][0] ||
        myRTC.hours == bellTimes[i][0]  && myRTC.minutes < bellTimes[i][1]) 
        {
          lcd.setCursor(0, 1);
          lcd.print("Keyingi:");
          if (bellTimes[i][0] < 10) lcd.print("0");
          lcd.print(bellTimes[i][0]);
          lcd.print(":");
          if (bellTimes[i][1] < 10) lcd.print("0");
          lcd.print(bellTimes[i][1]);
          lcd.print(":00");
          found = true;
          break;
        }
  }
  if (!found) 
    {
      lcd.setCursor(0, 1);
      lcd.print("Keyingi:");
      lcd.print(bellTimes[0][0]);
      lcd.print(":");
      lcd.print(bellTimes[0][1]);
      lcd.print(":00");
      return;
    }
}
void button()
{
   if (digitalRead(3) == 1) 
  {
    digitalWrite(2, 1);
    tone(12, 500, 1000);
    Serial.println("qolda yoqildi");
  }
  else 
  {
    digitalWrite(2, 0);
    noTone(12);
  }
  delay(100);
}

#define relayPin 13
#define motorPin 11
#define potPin A0
#define directionPin A1
#define buzzerPin 9
#define led1 A4
#define led2 A5
#define LDR  A2
#define fadeLed 10

int potValue;
int potVoltage;
int LDRval;

bool R_F ;
int potValue2;

byte brightness = 0;
byte fade = 5;

unsigned long currentT;
unsigned long ledfadeT = 0;
unsigned long previousT = 0;

//dilaw notes
#define F8 5588
#define D8 4698
#define C8 4186
#define G7 3136

int noteIndex = -1;           
int noteDuration = 0;    

int melody[] = {
  F8, D8, D8, C8, C8, G7,
  };

int noteDurations[] = {
  16,5,16,5,16,2
};


void setup() {
  pinMode(fadeLed,OUTPUT);
  pinMode(relayPin,OUTPUT);
  pinMode(potPin,INPUT);
  pinMode(buzzerPin,OUTPUT);
  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
  pinMode(motorPin,OUTPUT);
  pinMode(LDR,INPUT);

  Serial.begin(9600);
}

void loop() {
  currentT = millis();
  potVoltage = analogRead(potPin);
  potValue = map(potVoltage,0,900,0,3);
  motorSpeed(potValue);
  if (potValue == 0){
    LED_fade();
    noTone(buzzerPin);
  }else{
    brightness = 5;
    digitalWrite(fadeLed,LOW);
  }
  if (potValue == 3){
      buzzer_sound();
  }else{
    noteIndex = -1;
  }
  LDRval = analogRead(LDR);
  while(LDRval <= 70){
    LDRval = analogRead(LDR);
    motorSpeed(0);
  }

  potValue2 = analogRead(directionPin);
  Direction(potValue2);
  direction(R_F);

  Serial.println(potValue);
  Serial.println(potValue2);
}

void motorSpeed(int speed){
  switch(speed){
  case 0:
    analogWrite(motorPin,0);
    break;
  case 1:
    analogWrite(motorPin,100);
    break;
  case 2:
    analogWrite(motorPin,175);
    break;
  case 3:
    analogWrite(motorPin, 255);
    break;
  default:
    analogWrite(motorPin,0);
    break;
  }
}

void direction(bool State){
  switch(State){
  case true:
    digitalWrite(relayPin,HIGH);
     digitalWrite(led1,HIGH);
    digitalWrite(led2,LOW);
    break;
  case false:
    digitalWrite(relayPin,LOW);
    digitalWrite(led2,HIGH);
    digitalWrite(led1,LOW);
    break;
  default:
    digitalWrite(relayPin,LOW);
    break;
    }
  }

bool Direction(int potentiometer){
  if (potentiometer > 500){
    return R_F = true;
  }else{
    return R_F = false;
     digitalWrite(led1,LOW);
  }
}

void LED_fade(){
  if (currentT - ledfadeT >= 20){
    analogWrite(fadeLed,brightness);
    brightness += fade;
    if (brightness >= 250 || brightness <= 0){
      fade = -fade;
    }
    ledfadeT = currentT;
  }
}

void buzzer_sound(){
  if (currentT - previousT >= noteDuration * 1.30 && noteIndex < 6) { 
    noTone(buzzerPin);  
    noteIndex++;       

    noteDuration = 4000 / noteDurations[noteIndex];
    tone(buzzerPin, melody[noteIndex], noteDuration);

    previousT = currentT;
  }
  if (noteIndex >= 6) {
      noTone(buzzerPin);  
    }

  }


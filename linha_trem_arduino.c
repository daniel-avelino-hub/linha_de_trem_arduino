#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

int echoPin = 6;
int trigPin = 7;
int buzzer = 9;
int led = 10;
Servo myservo;

long duration;
int distanceCm;

void setup() {
  lcd.init();
  lcd.backlight();
  
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(led, OUTPUT);
  myservo.attach(13);
  myservo.write(0);
}

void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH, 30000); // Timeout de 30ms evita travar se falhar leitura
  distanceCm = (duration * 0.034 / 2);

  Serial.print("Distancia cm: ");
  Serial.println(distanceCm);
  
  lcd.setCursor(0, 0);
  lcd.print("Dist: ");
  lcd.print(distanceCm);
  lcd.print(" cm   ");

  // ZONA 1: Trem muito perto (1 cm a 9 cm) -> Para de apitar, luz vermelha fixa
  if (distanceCm > 0 && distanceCm < 10) {
    myservo.write(72);
    noTone(buzzer);
    digitalWrite(buzzer, LOW);
    digitalWrite(led, HIGH);
    
    lcd.setCursor(0, 1);
    lcd.print("TREM PASSANDO!  ");
    delay(100);
  }
  // ZONA 2: Trem se aproximando (10 cm a 30 cm) -> Apita e pisca
  else if (distanceCm >= 10 && distanceCm <= 30) {
    myservo.write(72);
    
    lcd.setCursor(0, 1);
    lcd.print("ATENCAO: TREM!  ");
    
    tone(buzzer, 1500);
    digitalWrite(buzzer, HIGH);
    digitalWrite(led, HIGH);
    delay(200);
    
    noTone(buzzer);
    digitalWrite(buzzer, LOW);
    digitalWrite(led, LOW);
    delay(200);
  }
  // ZONA 3: Via livre (maior que 30 cm ou sem obstáculo) -> Abre cancela e desliga tudo
  else {
    myservo.write(0);
    noTone(buzzer);
    digitalWrite(buzzer, LOW);
    digitalWrite(led, LOW);
    
    lcd.setCursor(0, 1);
    lcd.print("VIA LIVRE       ");
    delay(100);
  }
}

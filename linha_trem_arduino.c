#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>


// Endereço I2C padrão no Tinkercad: 0x27 (se não aparecer texto, teste 0x20 ou 0x3F)
LiquidCrystal_I2C lcd(0x27, 16, 2);


int echoPin = 6;
int trigPin = 7;
int buzzer = 10;
int led = 9;
Servo myservo;


long duration;
int distanceCm;


void setup() {
  lcd.init();        // Inicializa o LCD I2C
  lcd.backlight();   // Liga a luz de fundo do display
  
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
  
  duration = pulseIn(echoPin, HIGH);
  distanceCm = (duration * 0.034 / 2);


  Serial.print("Distancia cm: ");
  Serial.println(distanceCm);
  
  lcd.setCursor(0, 0);
  lcd.print("Dist: ");
  lcd.print(distanceCm);
  lcd.print(" cm   ");


  // ZONA 1: Trem muito perto (menos de 30 cm) -> Para de apitar, luz vermelha fixa
  if (distanceCm > 0 && distanceCm < 30) {
    myservo.write(72);
    noTone(buzzer);
    digitalWrite(led, HIGH);
    
    lcd.setCursor(0, 1);
    lcd.print("TREM PASSANDO!  ");
    delay(100);
  }
  // ZONA 2: Trem se aproximando (entre 30 cm e 199 cm) -> Apita e pisca
  else if (distanceCm >= 30 && distanceCm < 200) {
    myservo.write(72);
    
    lcd.setCursor(0, 1);
    lcd.print("ATENCAO: TREM!  ");
    
    // Toque de sino metálico
    tone(buzzer, 1500);
    digitalWrite(led, HIGH);
    delay(150);
    
    noTone(buzzer);
    digitalWrite(led, LOW);
    delay(200);
  }
  // ZONA 3: Via livre (200 cm ou mais) -> Abre cancela e desliga tudo
  else {
    myservo.write(0);
    noTone(buzzer);
    digitalWrite(led, LOW);
    
    lcd.setCursor(0, 1);
    lcd.print("VIA LIVRE       ");
    delay(100);
  }
}
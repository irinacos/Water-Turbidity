#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

#define SENSOR_PIN A0             // Pinul analogic pentru senzorul de turbiditate (A0 = ADC0)
#define GREEN_LED_BIT (1 << PD2)  // Bitul pinului 2 din PORTD
#define YELLOW_LED_BIT (1 << PD4) // Bitul pinului 4
#define RED_LED_BIT (1 << PD6)    // Bitul pinului 6

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Functie care calculeaza nivelul de turbiditate, calculele fiind adaptate la caracteristicile senzorului:
// 100*voltage reprezinta NTU in functie de V. Numarul 380 l-am ales experimental, dupa ce am observat 
// ca NTU-ul era inversat (era aprox. 375 cand apa era clara si aprox. 0 cand apa era foarte tulbure)
float calcNTU(float voltage) {
  return 380 - 100 * voltage;
}

void setup() {
  // Configurarea LED-urilor ca output-uri
  DDRD |= GREEN_LED_BIT;
  DDRD |= YELLOW_LED_BIT;
  DDRD |= RED_LED_BIT;

  lcd.init();
  lcd.backlight();

  Serial.begin(9600); // Baud rate

  // Configurare ADC
  ADMUX = (1 << REFS0); // Seteaza bitul REFS0 la 1 => referinta de tensiune pentru ADC e AVcc (5 V)
  ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Porneste convertorul si seteaza prescalerul la 128
}

uint16_t readADC(uint8_t channel) {
  ADMUX = (ADMUX & 0xF8) | (channel & 0x07); // Selecteaza canalul ADC

  // Porneste conversia ADC si asteapta pana este gata
  ADCSRA |= (1 << ADSC);
  while (ADCSRA & (1 << ADSC));

  return ADC; // Rezultatul conversiei
}

void loop() {
  uint16_t sensorValue = readADC(0);            // Citirea valoarii de la senzor (canalul ADC0)
  float voltage = sensorValue * (5.0 / 1024.0); // V_in = ADC * V_ref / 1024
  float ntu = calcNTU(voltage);
  float percentage = 100 * ntu / 380;           // Scalarea nivelului de turbiditate la valori cuprinse intre 0-100

  // Afisare pe LCD
  lcd.setCursor(0, 0);
  lcd.print(voltage);
  lcd.print("V - ");
  lcd.print(ntu);
  lcd.print("NTU");

  lcd.setCursor(0, 1);
  lcd.print(percentage);
  lcd.print("% - ");

  if (percentage < 30) {
    PORTD |= GREEN_LED_BIT;   // Aprinde LED-ul verde
    PORTD &= ~YELLOW_LED_BIT; // Stinge LED-ul galben
    PORTD &= ~RED_LED_BIT;    // Stinge LED-ul rosu
    lcd.print("Curata ");
  } else if (percentage < 70) {
    PORTD &= ~GREEN_LED_BIT;  // Stinge LED-ul verde
    PORTD |= YELLOW_LED_BIT;  // Aprinde LED-ul galben
    PORTD &= ~RED_LED_BIT;    // Stinge LED-ul rosu
    lcd.print("Tulbure");
  } else {
    PORTD &= ~GREEN_LED_BIT;  // Stinge LED-ul verde
    PORTD &= ~YELLOW_LED_BIT; // Stinge LED-ul galben
    PORTD |= RED_LED_BIT;     // Aprinde LED-ul rosu
    lcd.print("Murdara");
  }

  delay(1000);
}
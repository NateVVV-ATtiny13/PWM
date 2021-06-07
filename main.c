/*
 * Projekt-Name:
 *
 * Created (Datum): 
 *  Author: Nathanael
 */ 


#include <avr/io.h>
#define F_CPU 1200000UL  // 1,2 MHz (für ATtiny13, ansonsten verwendete Frequenz einfügen (für die "delay"-Funktion))
#include <util/delay.h>
// hier die benötigten Bibliotheken einfügen und wiederkehrende Befehle mit
// #define deklarieren

#define toggle_PB4 PORTB ^= (1<<PB4);

void sleep(uint16_t delay)  // Soft-PWM
{
  for(uint16_t i = 0; i < delay; i++)
  {
    _delay_ms(1);
  }
}

void PWM_Init(void)	// PWM-Bits initialisieren
{  
  TCCR0A |= (1<<COM0A1) | (1<<WGM01) | (1<<WGM00);
  TCCR0B = (1<<CS01);
}

void PWM_start(void)	// PWM-Funktion für LEDs, nur heller werden
{    
  for(uint8_t i = 0; i < 255; i++)
  {
    OCR0A = i;
    _delay_ms(5);
    if(i == 254)
    {
      // an der hellsten Stufe eine Sekunde lang stehen bleiben
      // und zur Anzeige die anderen LEDs ausmachen
      //PORTB &= ~(1<<PB4);
      _delay_ms(1000);
    }
  }
    
  OCR0A = 0;
  _delay_ms(1000);
  //PORTB |= (1<<PB4);
  //_delay_ms(1000);
}

void PWM_fade(void) // PWM-Funktion, LED hoch und runter gefadet
{
  uint8_t brightness = 0;
  uint8_t scaler = 1;  // scaler zählt hoch oder runter
  while(1)
  {
    OCR0A = brightness;
    brightness += scaler;
    if (brightness == 0 || brightness == 255)
    {
      // scaler "umdrehen" --> statt hoch- soll man nun runterzählen und umgekehrt --> deshalb Vorzeichen von scaler ändern
      scaler = -scaler;
      for (int i = 0; i < 6; ++i)
      {
        toggle_PB4;
        _delay_ms(100);
      }
    }
    //_delay_ms(10); // 5 ms bei jedem Zwischenschritt warten, damit man auch was sieht
    if (brightness < 35)
    {
      _delay_ms(40);
    } else if (brightness >= 35 && brightness <= 100)
    {
      _delay_ms(20);
    } else if (brightness > 100)
    {
      _delay_ms(5);
    }
  }
}

void setup(void)  // Register und Ports einstellen
{
  DDRB |= (1<<DDB0) | (1<<DDB4);  // PB0, PB4 als Ausgang
  PORTB |= (1<<PB4);  // PB4 an
}

int main(void)
{
  setup();
  PWM_Init();

  while(1)
  {
    PWM_fade();
  }
  return 0;
}

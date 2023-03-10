#define __DEBUG__

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <string.h>

// Definir constantes
#define ANCHO_PANTALLA 128 // ancho pantalla OLED
#define ALTO_PANTALLA 64   // alto pantalla OLED
#define LOGO_UTN_ALTO 64
#define LOGO_UTN_ANCHO 51
#define mideCap 36 
#define cargaCap 18
#define descaCap 19
#define outImpulso 17
#define inImpulso 16
#define botonBrillo 27

// Definir funciones
void mensajeInicio(void);
void IRAM_ATTR detectaBotonBrillo(void);
float inductancia(void);
float capacitancia(void);
void mostrarValorCapacitor(float valorCapacitor);
void mostrarValorInductor(float valorInductor);

// Objeto de la clase Adafruit_SSD1306
Adafruit_SSD1306 display(ANCHO_PANTALLA, ALTO_PANTALLA, &Wire, -1);

// Definir Variables Globales

bool brillo = false;

float valorCapacitor = 0;
float valorInductor = 0;


static const uint8_t logoUTN[] = {
    0xff, 0xc0, 0x0f, 0xff, 0x00, 0x7f, 0xe0,
    0xff, 0xc0, 0x0f, 0xff, 0x00, 0x7f, 0xe0,
    0xff, 0xc0, 0x0f, 0xff, 0x00, 0x7f, 0xe0,
    0x7f, 0xe0, 0x0f, 0xff, 0x00, 0x7f, 0xe0,
    0x7f, 0xe0, 0x0f, 0xff, 0x00, 0x7f, 0xe0,
    0x7f, 0xe0, 0x0f, 0xff, 0x00, 0x7f, 0xe0,
    0x3f, 0xe0, 0x0f, 0xff, 0x00, 0xff, 0xc0,
    0x3f, 0xf0, 0x0f, 0xff, 0x00, 0xff, 0xc0,
    0x3f, 0xf0, 0x0f, 0xff, 0x00, 0xff, 0xc0,
    0x3f, 0xf8, 0x0f, 0xff, 0x01, 0xff, 0x80,
    0x1f, 0xf8, 0x0f, 0xff, 0x03, 0xff, 0x80,
    0x1f, 0xfc, 0x0f, 0xff, 0x03, 0xff, 0x80,
    0x0f, 0xfe, 0x0f, 0xff, 0x0f, 0xff, 0x00,
    0x0f, 0xff, 0x0f, 0xff, 0x1f, 0xff, 0x00,
    0x07, 0xff, 0xcf, 0xff, 0x3f, 0xfe, 0x00,
    0x07, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00,
    0x03, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00,
    0x01, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00,
    0x00, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00,
    0x00, 0x7f, 0xff, 0xff, 0xff, 0xe0, 0x00,
    0x00, 0x3f, 0xff, 0xff, 0xff, 0xc0, 0x00,
    0x00, 0x1f, 0xff, 0xff, 0xff, 0x80, 0x00,
    0x00, 0x0f, 0xff, 0xff, 0xfe, 0x00, 0x00,
    0x00, 0x03, 0xff, 0xff, 0xf8, 0x00, 0x00,
    0x00, 0x00, 0xff, 0xff, 0xf0, 0x00, 0x00,
    0x00, 0x00, 0x1f, 0xff, 0x80, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0,
    0x00, 0x00, 0x1f, 0xff, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x3f, 0xff, 0xc0, 0x00, 0x00,
    0x00, 0x00, 0xff, 0xff, 0xf0, 0x00, 0x00,
    0x00, 0x03, 0xff, 0xff, 0xfc, 0x00, 0x00,
    0x00, 0x0f, 0xff, 0xff, 0xff, 0x00, 0x00,
    0x00, 0x1f, 0xff, 0xff, 0xff, 0x80, 0x00,
    0x00, 0x3f, 0xff, 0xff, 0xff, 0xc0, 0x00,
    0x00, 0x7f, 0xff, 0xff, 0xff, 0xe0, 0x00,
    0x00, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00,
    0x01, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00,
    0x03, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00,
    0x07, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00,
    0x07, 0xff, 0xcf, 0xff, 0x3f, 0xfe, 0x00,
    0x0f, 0xff, 0x0f, 0xff, 0x1f, 0xff, 0x00,
    0x0f, 0xfe, 0x0f, 0xff, 0x07, 0xff, 0x00,
    0x1f, 0xfc, 0x0f, 0xff, 0x03, 0xff, 0x80,
    0x1f, 0xf8, 0x0f, 0xff, 0x03, 0xff, 0x80,
    0x3f, 0xf8, 0x0f, 0xff, 0x01, 0xff, 0x80,
    0x3f, 0xf0, 0x0f, 0xff, 0x00, 0xff, 0xc0,
    0x3f, 0xf0, 0x0f, 0xff, 0x00, 0xff, 0xc0,
    0x7f, 0xe0, 0x0f, 0xff, 0x00, 0xff, 0xc0,
    0x7f, 0xe0, 0x0f, 0xff, 0x00, 0x7f, 0xe0,
    0x7f, 0xe0, 0x0f, 0xff, 0x00, 0x7f, 0xe0,
    0x7f, 0xe0, 0x0f, 0xff, 0x00, 0x7f, 0xe0,
    0xff, 0xc0, 0x0f, 0xff, 0x00, 0x7f, 0xe0,
    0xff, 0xc0, 0x0f, 0xff, 0x00, 0x7f, 0xe0};

static const uint8_t logoCapacitor[] = {
    0x00, 0x00, 0x00, 0x78, 0x1e, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x78, 0x1e, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x78, 0x1e, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x78, 0x1e, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x78, 0x1e, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x78, 0x1e, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x78, 0x1e, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x78, 0x1e, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x78, 0x1e, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x78, 0x1e, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xf8, 0x1f, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xf8, 0x1f, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x78, 0x1e, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x78, 0x1e, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x78, 0x1e, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x78, 0x1e, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x78, 0x1e, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x78, 0x1e, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x78, 0x1e, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x78, 0x1e, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x78, 0x1e, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x78, 0x1e, 0x00, 0x00, 0x00};

static const uint8_t logoInductor[] = { // 100x18
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x1f, 0x80, 0x07, 0xf0, 0x00, 0xfe, 0x00, 0x1f, 0x80, 0x00, 0x00,
    0x00, 0x00, 0xff, 0xf0, 0x1f, 0xfc, 0x03, 0xff, 0x80, 0xff, 0xf0, 0x00, 0x00,
    0x00, 0x01, 0xf9, 0xf8, 0x3e, 0x3f, 0x0f, 0xc7, 0xc1, 0xf9, 0xf8, 0x00, 0x00,
    0x00, 0x03, 0xc0, 0x1c, 0x70, 0x07, 0x9e, 0x00, 0xe3, 0x80, 0x3c, 0x00, 0x00,
    0x00, 0x07, 0x00, 0x0e, 0xe0, 0x03, 0x9c, 0x00, 0x77, 0x00, 0x0e, 0x00, 0x00,
    0x00, 0x07, 0x00, 0x07, 0xc0, 0x01, 0xf8, 0x00, 0x3e, 0x00, 0x0e, 0x00, 0x00,
    0x00, 0x0e, 0x00, 0x07, 0xc0, 0x00, 0xf0, 0x00, 0x3e, 0x00, 0x07, 0x00, 0x00,
    0x00, 0x0e, 0x00, 0x03, 0x80, 0x00, 0xf0, 0x00, 0x1c, 0x00, 0x07, 0x00, 0x00,
    0x00, 0x0c, 0x00, 0x03, 0x80, 0x00, 0x60, 0x00, 0x1c, 0x00, 0x03, 0x00, 0x00,
    0xff, 0xfc, 0x00, 0x03, 0x80, 0x00, 0x60, 0x00, 0x1c, 0x00, 0x03, 0xff, 0xf0,
    0xff, 0xfc, 0x00, 0x01, 0x00, 0x00, 0x60, 0x00, 0x08, 0x00, 0x03, 0xff, 0xf0,
    0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xf0,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

void setup()
{
#ifdef __DEBUG__
  Serial.begin(115200);
  delay(100);
  Serial.println("Iniciando pantalla OLED");
#endif

  // Iniciar pantalla OLED en la direcci??n 0x3C
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
#ifdef __DEBUG__
    Serial.println("No se encuentra la pantalla OLED");
#endif
    while (true)
      ;
  }
  // Clear the buffer.
  display.clearDisplay();

  analogReadResolution(12);
  analogSetPinAttenuation(mideCap, ADC_11db);
  adcAttachPin(mideCap);
  pinMode(descaCap, INPUT);
  pinMode(cargaCap, OUTPUT);
  pinMode(outImpulso, OUTPUT);
  pinMode(cargaCap, OUTPUT);
  //pinMode(botonBrillo, INPUT_PULLDOWN);
  //attachInterrupt(digitalPinToInterrupt(botonBrillo), detectaBotonBrillo, RISING);

  mensajeInicio();
}

void loop()
{
 mostrarValorCapacitor(capacitancia());
  mostrarValorInductor(inductancia());
  display.invertDisplay(brillo);
}

// Funciones
void IRAM_ATTR detectaBotonBrillo(void)
{
  brillo = !brillo;
}

void mensajeInicio(void)
{
  display.clearDisplay();
  // Draw bitmap on the screen
  display.drawBitmap(0, 0, logoUTN, LOGO_UTN_ANCHO, LOGO_UTN_ALTO, SSD1306_WHITE);
  display.display();
  display.startscrolldiagright(0x00, 0x0F);
  delay(4100);
  display.stopscroll();
  display.clearDisplay();
  display.drawBitmap(76, 0, logoUTN, LOGO_UTN_ANCHO, LOGO_UTN_ALTO, SSD1306_WHITE);

  display.setTextSize(2);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(13, 10);   // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

  // Not all the characters will fit on the display. This is normal.
  // Library will draw what it can and the rest will be clipped.

  display.write("FRBB");
  display.display();
  delay(700);

  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(14, 30);   // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font
  display.write("Medidas");
  display.display();
  delay(700);

  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(1, 40);    // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font
  display.write("Electr");

  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(36, 40);   // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font
  display.write(162);

  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(42, 40);   // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font
  display.write("nicas");
  display.display();
  delay(700);

  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(32, 50);   // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font
  display.write("I");
  display.display();
  delay(700);

  display.clearDisplay();

  display.invertDisplay(true);
  display.setTextSize(2);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(18, 15);   // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font
  display.write("PROYECTO");

  display.setCursor(34, 35); // Start at top-left corner
  display.cp437(true);       // Use full 256 char 'Code Page 437' font
  display.write("FINAL");
  display.display();

  delay(800);
  display.clearDisplay();

  display.setCursor(35, 15); // Start at top-left corner
  display.cp437(true);       // Use full 256 char 'Code Page 437' font
  display.write("APAZA");

  display.setCursor(28, 35); // Start at top-left corner
  display.cp437(true);       // Use full 256 char 'Code Page 437' font
  display.write("ADOLFO");
  display.display();

  delay(1400);
  display.clearDisplay();

  display.setTextSize(2);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(30, 15);   // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font
  display.write("CHOQUE");

  display.setCursor(35, 35); // Start at top-left corner
  display.cp437(true);       // Use full 256 char 'Code Page 437' font
  display.write("AARON");
  display.display();

  delay(1400);
  display.clearDisplay();
  display.invertDisplay(false);
}

float capacitancia(void)
{
  unsigned long iniPeriodo = 0;
  unsigned long transcurrido = 0;
  const int res = 19872; // 19872  119450
  float Cx = 0;
  float acumulado = 0;

  for (int i = 0; i < 100; i++)
  {
    digitalWrite(cargaCap, HIGH);
    iniPeriodo = micros();
    while (analogRead(mideCap) < 2577)
    {
    }
    transcurrido = micros() - iniPeriodo;
    Cx = ((float)transcurrido / res) * 1000.0;
    digitalWrite(cargaCap, LOW);
    pinMode(descaCap, OUTPUT);
    digitalWrite(descaCap, LOW);
    while (analogRead(mideCap) > 0)
    {
    }
    pinMode(descaCap, INPUT);
    Serial.println(Cx);
    acumulado = acumulado + Cx;
  }
  acumulado = acumulado / 100.0;

  return acumulado;
}

float inductancia(void)
{
  float valor = 0;
  float lx = 0;
  float t = 0;
  float fo = 0;
  float cap = 0.00000001034;
  float pi = 3.14159;
  float acumulado = 0;
  int countador = 0;

  for (int i = 0; i < 50; i++)
  {
    digitalWrite(outImpulso, HIGH);
    delay(1);
    digitalWrite(outImpulso, LOW);
    t = pulseIn(inImpulso, LOW);
    Serial.println(t);
    fo = 1000000 / (2 * t);
    lx = (1 / (4 * pow(pi, 2) * pow(fo, 2) * cap)) * 1000000;
    if (lx != 0.0)
    {
      acumulado = lx + acumulado;
      countador++;
      Serial.println(lx);
    }
  }

  acumulado = acumulado / countador;

  return acumulado;
}

void mostrarValorCapacitor(float valorCapacitor)
{
  display.clearDisplay();
  // Draw bitmap on the screen
  display.drawBitmap(32, 40, logoCapacitor, 63, 22, SSD1306_WHITE);
  display.display();

  display.setTextSize(2);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(6, 2);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

  // Not all the characters will fit on the display. This is normal.
  // Library will draw what it can and the rest will be clipped.

  display.write("Capacitor:");

  char aux[12];

  if (valorCapacitor < 10.0)
  {
    sprintf(aux, "%1.2f nF", valorCapacitor);
    display.setCursor(25, 20);
    display.write(aux);
  }
  else if (valorCapacitor < 100.0)
  {
    sprintf(aux, "%2.2f nF", valorCapacitor);
    display.setCursor(18, 20);
    display.write(aux);
  }
  else if (valorCapacitor < 1000)
  {
    sprintf(aux, "%3.2f nF", valorCapacitor);
    display.setCursor(10, 20);
    display.write(aux);
  }
  else if (valorCapacitor < 10000)
  {
    valorCapacitor = valorCapacitor / 1000;
    sprintf(aux, "%1.2f ", valorCapacitor);
    display.setCursor(25, 20);
    display.write(aux);
    display.setCursor(79, 20);
    display.write(230);
    display.setCursor(89, 20);
    display.write("F");
  }
  else if (valorCapacitor < 100000)
  {
    valorCapacitor = valorCapacitor / 1000;
    sprintf(aux, "%2.2f ", valorCapacitor);
    display.setCursor(18, 20);
    display.write(aux);
    display.setCursor(87, 20);
    display.write(230);
    display.setCursor(98, 20);
    display.write("F");
  }
  else if (valorCapacitor < 1000000)
  {
    valorCapacitor = valorCapacitor / 1000;
    sprintf(aux, "%3.2f ", valorCapacitor);
    display.setCursor(12, 20);
    display.write(aux);
    display.setCursor(92, 20);
    display.write(230);
    display.setCursor(103, 20);
    display.write("F");
  }

  display.display();

  delay(3000);
  display.clearDisplay();
}

void mostrarValorInductor(float valorInductor)
{
  display.clearDisplay();
  // Draw bitmap on the screen
  display.drawBitmap(12, 42, logoInductor, 100, 18, SSD1306_WHITE);
  display.display();

  display.setTextSize(2);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(10, 2);    // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

  // Not all the characters will fit on the display. This is normal.
  // Library will draw what it can and the rest will be clipped.

  display.write("Inductor:");

  char aux[12];

  if (valorInductor < 10.0)
  {
    sprintf(aux, "%1.2f ", valorInductor);
    display.setCursor(25, 20);
    display.write(aux);
    display.setCursor(79, 20);
    display.write(230);
    display.setCursor(89, 20);
    display.write("H");
  }
  else if (valorInductor < 100.0)
  {
    sprintf(aux, "%2.2f ", valorInductor);
    display.setCursor(18, 20);
    display.write(aux);
    display.setCursor(87, 20);
    display.write(230);
    display.setCursor(98, 20);
    display.write("H");
  }
  else if (valorInductor < 1000)
  {
    sprintf(aux, "%3.2f ", valorInductor);
    display.setCursor(12, 20);
    display.write(aux);
    display.setCursor(92, 20);
    display.write(230);
    display.setCursor(103, 20);
    display.write("H");
  }
  else if (valorInductor < 10000)
  {
    valorInductor = valorInductor / 1000;
    sprintf(aux, "%1.2f mH", valorInductor);
    display.setCursor(22, 20);
    display.write(aux);
  }
  else if (valorInductor < 100000)
  {
    valorInductor = valorInductor / 1000;
    sprintf(aux, "%2.2f mH", valorInductor);
    display.setCursor(12, 20);
    display.write(aux);
  }
  else if (valorInductor < 1000000)
  {
    valorInductor = valorInductor / 1000;
    sprintf(aux, "%3.2f mH", valorInductor);
    display.setCursor(9, 20);
    display.write(aux);
  }

  display.display();

  delay(3000);
  display.clearDisplay();
}
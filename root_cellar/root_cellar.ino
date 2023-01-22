#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 5

OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensors(&oneWire);

float Celsius=0;
float Fahrenheit=0;

void setup(void)
{  
  Serial.begin(9600);
  sensors.begin();
}

void loop(void)
{ 
  Serial.println("NewSchwad2");
  sensors.requestTemperatures(); 
  Celsius=sensors.getTempCByIndex(0);
  Fahrenheit=sensors.toFahrenheit(Celsius);
  Serial.print(" C  ");
  Serial.print(Celsius);
  Serial.print(" F  ");
  Serial.println(Fahrenheit);
  delay(10);
}

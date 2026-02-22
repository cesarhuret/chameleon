
#include <Arduino.h>
#include "src/Chameleon.h"

bool stopped = false;

Chameleon chameleon;

void setup()
{
  Serial.begin(115200);
  chameleon.init();

  int8_t status = pixy.init();

  if (status == 0)
    Serial.println("SUCCESS");
  else {
    Serial.print("Pixy init failed: ");
    Serial.println(status);
  }

  Serial.println("Press 'd' to stop the program.");
}

void loop()
{
  chameleon.run();
}

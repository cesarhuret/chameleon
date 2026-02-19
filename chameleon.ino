#include "Pixy2.h"

Pixy2 pixy;

bool stopped = false;

void setup()
{
  Serial.begin(115200);

  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);

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
  if (Serial.available() > 0)
  {
    char input = Serial.read();

    if (input == 'd')
    {
      Serial.println("Program stopped.");
      stopped = true;
    }
  }

  if (stopped)
  {
    return; 
  }

  pixy.ccc.getBlocks();

  if (pixy.ccc.numBlocks)
  {
    Serial.print("Detected ");
    Serial.println(pixy.ccc.numBlocks);

    for (int i = 0; i < pixy.ccc.numBlocks; i++)
    {
      Serial.print("  block ");
      Serial.print(i);
      Serial.print(": ");
      pixy.ccc.blocks[i].print();
    }
  }
  else
  {
    Serial.println("No blocks detected");
  }

  delay(1000); 
}

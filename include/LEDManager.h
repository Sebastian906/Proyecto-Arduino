// LEDManager.h
#ifndef LEDMANAGER_H
#define LEDMANAGER_H

class LEDManager {
  public:
    LEDManager(int pin);
    void parpadear(int gramos);
  private:
    int ledPin;
};

#endif
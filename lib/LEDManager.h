// LEDManager.h
#ifndef LEDMANAGER_H
#define LEDMANAGER_H

class LEDManager {
  public:
    LEDManager();
    void parpadear(int cuenco);
  private:
    int ledPin;
};

#endif
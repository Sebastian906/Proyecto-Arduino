// LEDManager.h
#ifndef LEDMANAGER_H
#define LEDMANAGER_H

class LEDManager {
  public:
    LEDManager();
    void parpadear(int cuenco, unsigned long tiempoMs = 200);
  private:
    int ledPin;
};

#endif
#ifndef ESPWIFI_H
#define ESPWIFI_H
#include "IConection.h"
class ESPWiFi: public IConnection
{
  public:
    ESPWiFi(char* ssid, char* password);
    int conectar() override;
  private:
    ESPWiFi();
    char* ssid;
    char* password;

};
#endif
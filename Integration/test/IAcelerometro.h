#ifndef IACELEROMETRO_H
#define IACELEROMETRO_H
#include "Lectura.h"

class IAcelerometro
{
  public:
    virtual void leerDatos(Lectura* lectura) = 0;
    virtual int iniciar() = 0;
    virtual bool detectarMovimiento() = 0;
    virtual ~IAcelerometro() = default;
    
};
#endif
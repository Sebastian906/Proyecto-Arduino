#include "Mascota.h"

// Constructor por defecto
Mascota::Mascota():EEPROM() {
    nombre = "";
    rfidUID = "";
    tiempoEntreDosis = 5;  // Valor predeterminado
    tiempoEntreDosisActual = -1; 
    cantidadGramos = 5;     // Valor predeterminado
    cantidadGramosActual = cantidadGramos +1;
    idEEPROM = -1;
    totalAlimentoConsumido = 0;
    guardarEnEEPROM(); // Guarda los valores predeterminados en EEPROM
    
} 


// Constructor con parámetros
Mascota::Mascota(String nombre, String rfidUID, int cuenco):EEPROM() {
    this->nombre = nombre;
    this->rfidUID = rfidUID;
    this->idEEPROM = -1;
    this->tiempoEntreDosis = 5;  // Valor predeterminado
    this->cantidadGramos = 5;     // Valor predeterminado
    this-> cantidadGramosActual = cantidadGramos +1;
    this->totalAlimentoConsumido = 0;
    this->Cuenco = cuenco;
    guardarEnEEPROM(); // Guarda los valores predeterminados en EEPROM
}

// Método para establecer el tiempo entre dosis
bool Mascota::setTiempo(unsigned int tiempo) {
    if (tiempo >= 5 && tiempo <= 30) {
        tiempoEntreDosis = tiempo;
        return true;
    }
    return false;
}

// Método para establecer la cantidad de comida
bool Mascota::setComida(unsigned int gramos) {
    if (gramos >= 1 && gramos <= 10) {
        cantidadGramos = gramos;
        cantidadGramosActual = cantidadGramos;
        return true;
    }
    return false;
}

// Setters adicionales
void Mascota::setNombre(String nombre) {
    this->nombre = nombre;
}

void Mascota::setRFID(String rfidUID) {
    this->rfidUID = rfidUID;
}

void Mascota::setIdEEPROM(int id) {
    this->idEEPROM = id;
}

// Getters
String Mascota::getNombre() const {
    return nombre;
}

String Mascota::getRFID() const {
    return rfidUID;
}

unsigned int Mascota::getTiempo() const {
    return tiempoEntreDosis;
}

unsigned int Mascota::getComida() const {
    return cantidadGramos;
}

int Mascota::getIdEEPROM() const {
    return idEEPROM;
}

unsigned int Mascota::getCuenco() const {
    return Cuenco;
}


unsigned int Mascota::getTotalAlimentoConsumido() const {
    return totalAlimentoConsumido;
}

// Verifica si es tiempo de dispensar una dosis
bool Mascota::esTiempoDeDosis() const {
    return isDosis;
}

// Dispensa una dosis y actualiza contadores
int Mascota::dispensarDosis() {
    Serial.println(String(cantidadGramosActual));
        if(cantidadGramosActual == 1){
            isDosis = false;
            cantidadGramosActual = cantidadGramos;
            incrementarAlimento(cantidadGramos);
            CuencoEstado = true;
            return 0;
        }else{
            cantidadGramosActual = cantidadGramosActual - 1;
            return cantidadGramosActual;
        }

}

// Calcula el tiempo restante hasta la próxima dosis
unsigned int Mascota::getTiempoAnteDosis() {
        if(tiempoEntreDosisActual == tiempoEntreDosis){
            isDosis = true;
            tiempoEntreDosisActual = 0;
            return tiempoEntreDosis;
        }else{
            return tiempoEntreDosisActual ++;
        }
}


// Guarda los datos de la mascota en EEPROM
void Mascota::guardarEnEEPROM() {
idEEPROM = EEPROM.guardarParametros(15, 5, idEEPROM);

    if (idEEPROM == UINT16_MAX) {
        Serial.println("Error al guardar en EEPROM: no hay espacio disponible.");
    }


    Serial.print("Datos guardados en EEPROM en la dirección: ");
    Serial.println(idEEPROM);
}


void Mascota::cargarDeEEPROM() {
    if (idEEPROM == UINT16_MAX) {
        Serial.println("Error: dirección de EEPROM no válida.");
        return;
    }

    ParametrosMascota parametros = EEPROM.cargarParametros(idEEPROM);

    if (!parametros.exito) {
        Serial.println("Error al cargar los parámetros de la mascota desde EEPROM.");
        return;
    }

    this->tiempoEntreDosis = parametros.tiempoEntreDosis;
    this->cantidadGramos = parametros.cantidadGramos;

    Serial.print("Datos cargados: tiempoEntreDosis = ");
    Serial.print(this->tiempoEntreDosis);
    Serial.print(", cantidadGramos = ");
    Serial.println(this->cantidadGramos);
}


// Resetea a los valores predeterminados
void Mascota::resetearValoresPredeterminados() {
    cargarDeEEPROM();
    this-> cantidadGramosActual = cantidadGramos +1;
    this->totalAlimentoConsumido = 0;
}

// Verifica si el UID coincide con el de la mascota
bool Mascota::coincideRFID(String uid) const {
    return rfidUID.equals(uid);
}

// Obtiene información del consumo de la mascota
String Mascota::getInfoConsumo() const {
    return "Mascota: " + nombre + " - Alimento total: " + String(totalAlimentoConsumido) + "g";
}

// Incrementa el contador de alimento total
void Mascota::incrementarAlimento(unsigned int gramos) {
    totalAlimentoConsumido += gramos;
}
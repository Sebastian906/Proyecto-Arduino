#include "Mascota.h"

// Constructor por defecto
Mascota::Mascota() {
    nombre = "";
    rfidUID = "";
    tiempoEntreDosis = 15;  // Valor predeterminado
    cantidadGramos = 5;     // Valor predeterminado
    idEEPROM = -1;
    tiempoUltimaDosis = 0;
    totalAlimentoConsumido = 0;
}

// Constructor con parámetros
Mascota::Mascota(String nombre, String rfidUID, int idEEPROM) {
    this->nombre = nombre;
    this->rfidUID = rfidUID;
    this->idEEPROM = idEEPROM;
    this->tiempoEntreDosis = 15;  // Valor predeterminado
    this->cantidadGramos = 5;     // Valor predeterminado
    this->tiempoUltimaDosis = 0;
    this->totalAlimentoConsumido = 0;
}

// Método para establecer el tiempo entre dosis
bool Mascota::setTiempo(unsigned int tiempo) {
    if (tiempo >= MIN_TIEMPO && tiempo <= MAX_TIEMPO) {
        tiempoEntreDosis = tiempo;
        return true;
    }
    return false;
}

// Método para establecer la cantidad de comida
bool Mascota::setComida(unsigned int gramos) {
    if (gramos >= MIN_GRAMOS && gramos <= MAX_GRAMOS) {
        cantidadGramos = gramos;
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

unsigned long Mascota::getTiempoUltimaDosis() const {
    return tiempoUltimaDosis;
}

unsigned int Mascota::getTotalAlimentoConsumido() const {
    return totalAlimentoConsumido;
}

// Verifica si es tiempo de dispensar una dosis
bool Mascota::esTiempoDeDosis() const {
    if (tiempoUltimaDosis == 0) return true; // Primera vez
    return (millis() - tiempoUltimaDosis) >= (tiempoEntreDosis * 1000UL);
}

// Dispensa una dosis y actualiza contadores
void Mascota::dispensarDosis() {
    actualizarTiempoUltimaDosis();
    incrementarAlimento(cantidadGramos);
}

// Actualiza el tiempo de la última dosis
void Mascota::actualizarTiempoUltimaDosis() {
    tiempoUltimaDosis = millis();
}

// Calcula el tiempo restante hasta la próxima dosis
unsigned long Mascota::getTiempoRestante() const {
    if (tiempoUltimaDosis == 0) return 0;
    unsigned long tiempoTranscurrido = millis() - tiempoUltimaDosis;
    unsigned long tiempoTotal = tiempoEntreDosis * 1000UL;
    
    if (tiempoTranscurrido >= tiempoTotal) return 0;
    return (tiempoTotal - tiempoTranscurrido) / 1000UL; // Retorna en segundos
}

// Guarda los datos de la mascota en EEPROM
void Mascota::guardarEnEEPROM() {
    if (idEEPROM < 0) return;
    
    int direccion = idEEPROM * TAMANO_DATOS_MASCOTA;
    
    // Guardar nombre (máximo 16 caracteres)
    for (int i = 0; i < 16; i++) {
        if (i < nombre.length()) {
            EEPROM.write(direccion + i, nombre[i]);
        } else {
            EEPROM.write(direccion + i, 0);
        }
    }
    
    // Guardar RFID UID (máximo 20 caracteres)
    for (int i = 0; i < 20; i++) {
        if (i < rfidUID.length()) {
            EEPROM.write(direccion + 16 + i, rfidUID[i]);
        } else {
            EEPROM.write(direccion + 16 + i, 0);
        }
    }
    
    // Guardar tiempo entre dosis (2 bytes)
    EEPROM.write(direccion + 36, tiempoEntreDosis & 0xFF);
    EEPROM.write(direccion + 37, (tiempoEntreDosis >> 8) & 0xFF);
    
    // Guardar cantidad en gramos (2 bytes)
    EEPROM.write(direccion + 38, cantidadGramos & 0xFF);
    EEPROM.write(direccion + 39, (cantidadGramos >> 8) & 0xFF);
    
    // Guardar total de alimento consumido (4 bytes)
    EEPROM.write(direccion + 40, totalAlimentoConsumido & 0xFF);
    EEPROM.write(direccion + 41, (totalAlimentoConsumido >> 8) & 0xFF);
    EEPROM.write(direccion + 42, (totalAlimentoConsumido >> 16) & 0xFF);
    EEPROM.write(direccion + 43, (totalAlimentoConsumido >> 24) & 0xFF);
}

// Carga los datos de la mascota desde EEPROM
void Mascota::cargarDeEEPROM() {
    if (idEEPROM < 0) return;
    
    int direccion = idEEPROM * TAMANO_DATOS_MASCOTA;
    
    // Cargar nombre
    nombre = "";
    for (int i = 0; i < 16; i++) {
        char c = EEPROM.read(direccion + i);
        if (c != 0) nombre += c;
        else break;
    }
    
    // Cargar RFID UID
    rfidUID = "";
    for (int i = 0; i < 20; i++) {
        char c = EEPROM.read(direccion + 16 + i);
        if (c != 0) rfidUID += c;
        else break;
    }
    
    // Cargar tiempo entre dosis
    tiempoEntreDosis = EEPROM.read(direccion + 36) | (EEPROM.read(direccion + 37) << 8);
    
    // Cargar cantidad en gramos
    cantidadGramos = EEPROM.read(direccion + 38) | (EEPROM.read(direccion + 39) << 8);
    
    // Cargar total de alimento consumido
    totalAlimentoConsumido = EEPROM.read(direccion + 40) | 
                           (EEPROM.read(direccion + 41) << 8) |
                           (EEPROM.read(direccion + 42) << 16) |
                           (EEPROM.read(direccion + 43) << 24);
    
    // Validar los valores cargados
    if (tiempoEntreDosis < MIN_TIEMPO || tiempoEntreDosis > MAX_TIEMPO) {
        tiempoEntreDosis = 15;
    }
    if (cantidadGramos < MIN_GRAMOS || cantidadGramos > MAX_GRAMOS) {
        cantidadGramos = 5;
    }
}

// Resetea a los valores predeterminados
void Mascota::resetearValoresPredeterminados() {
    tiempoEntreDosis = 15;
    cantidadGramos = 5;
    totalAlimentoConsumido = 0;
    tiempoUltimaDosis = 0;
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
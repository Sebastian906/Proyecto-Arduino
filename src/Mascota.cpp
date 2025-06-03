#include "Mascota.h"

// Constructor por defecto
Mascota::Mascota() : EEPROM(), estaPresente(false), tiempoUltimaDeteccion(0), 
                    totalAlimentoConsumido(0), CuencoEstado(false),
                    tiempoEntreDosis(60), cantidadGramos(5), isDosis(false) {
}

// Constructor con parámetros
Mascota::Mascota(String nombre, String rfidUID, int cuenco) : 
    nombre(nombre), rfidUID(rfidUID), Cuenco(cuenco), EEPROM(),
    totalAlimentoConsumido(0), estaPresente(false), tiempoUltimaDeteccion(0),
    CuencoEstado(false), tiempoEntreDosis(60), cantidadGramos(5), isDosis(false) {
}

// Método para establecer el tiempo entre dosis
bool Mascota::setTiempo(unsigned int tiempo) {
    if (tiempo >= 10 && tiempo <= 300) { // 10 seg a 5 min
        tiempoEntreDosis = tiempo;
        return true;
    }
    return false;
}

// Método para establecer la cantidad de comida
bool Mascota::setComida(unsigned int gramos) {
    if (gramos >= 1 && gramos <= 20) {
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

unsigned int Mascota::getCuenco() const {
    return Cuenco;
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

unsigned int Mascota::getTotalAlimentoConsumido() const {
    return totalAlimentoConsumido;
}

// Implementación de control de presencia RFID
bool Mascota::estaGatoPresente() const {
    // Verificar si el gato sigue presente (no ha pasado el timeout)
    return estaPresente && (millis() - tiempoUltimaDeteccion < TIMEOUT_PRESENCIA);
}

void Mascota::marcarPresencia() {
    estaPresente = true;
    tiempoUltimaDeteccion = millis();
}

void Mascota::marcarAusencia() {
    estaPresente = false;
}

// Verifica si es tiempo de dispensar una dosis
bool Mascota::esTiempoDeDosis() const {
    static unsigned long ultimaRevision = 0;
    unsigned long ahora = millis();
    
    // Solo revisar cada segundo para evitar cálculos excesivos
    if (ahora - ultimaRevision < 1000) {
        return false;
    }
    
    // El gato DEBE estar presente para dispensar
    if (!estaGatoPresente()) {
        return false;
    }
    
    // Verificar si ha pasado el tiempo entre dosis
    if (inicioDispensacion == 0) {
        return true; // Primera dosis cuando el gato esté presente
    }
    
    // Para dosis subsecuentes, verificar tiempo
    unsigned long tiempoTranscurrido = (ahora / 1000) % (tiempoEntreDosis);
    return tiempoTranscurrido == 0;
}

// Dispensa una dosis y actualiza contadores
int Mascota::dispensarDosis() {
    if (!estaGatoPresente()) {
        return 0; // No dispensar si no está presente
    }
    
    incrementarAlimento(cantidadGramos);
    isDosis = true;
    return cantidadGramos;
}


// Calcula el tiempo restante hasta la próxima dosis
unsigned int Mascota::getTiempoAnteDosis() {
    if (!estaGatoPresente()) {
        return 9999; // Indicar que el gato no está presente
    }
    
    unsigned long ahora = millis();
    unsigned long segundosTranscurridos = (ahora / 1000) % tiempoEntreDosis;
    return tiempoEntreDosis - segundosTranscurridos;
}

// Guarda los datos de la mascota en EEPROM
void Mascota::guardarEnEEPROM() {
    // Implementación simplificada
    // El EEPROMManager se encargará de los detalles
    // Por ahora, solo un placeholder
}

void Mascota::cargarDeEEPROM() {
    // Implementación simplificada
    // Por ahora, solo un placeholder
}

void Mascota::resetearValoresPredeterminados() {
    tiempoEntreDosis = 60;
    cantidadGramos = 5;
    totalAlimentoConsumido = 0;
    estaPresente = false;
}

// Implementación de validación RFID
bool Mascota::coincideRFID(String uid) const {
    return rfidUID.equals(uid);
}

// Implementación de información de consumo
String Mascota::getInfoConsumo() const {
    return "Mascota: " + nombre + ", Total: " + String(totalAlimentoConsumido) + "g";
}

// Implementación para incrementar alimento
void Mascota::incrementarAlimento(unsigned int gramos) {
    totalAlimentoConsumido += gramos;
    guardarEnEEPROM(); // Persistir el cambio
}

// Implementación de getTiempoUltimaDosis (método faltante)
unsigned long Mascota::getTiempoUltimaDosis() const {
    return tiempoUltimaDeteccion;
}
#include "EEPROMManager.h"

// Inicialización de la variable estática
bool EEPROMManager::resetPressed = false;

// Constructor
EEPROMManager::EEPROMManager() {
    numMascotas = 0;
}

// Inicializa el manager de EEPROM
void EEPROMManager::inicializar() {
    Serial.println("Inicializando EEPROMManager...");
    
    // Verificar si la EEPROM ya está inicializada
    if (!estaInicializada()) {
        Serial.println("Primera inicializacion - configurando valores predeterminados");
        establecerValoresPredeterminados();
        marcarComoInicializada();
    } else {
        Serial.println("EEPROM ya inicializada - cargando datos existentes");
        // Cargar el número de mascotas guardadas
        numMascotas = EEPROM.read(DIRECCION_CONTADOR);
        if (numMascotas > MAX_MASCOTAS) {
            numMascotas = 0; // Valor inválido, resetear
        }
    }
    
    configurarInterrupcionReset();
    Serial.print("Numero de mascotas cargadas: ");
    Serial.println(numMascotas);
}

// Configura la interrupción para el botón de RESET
void EEPROMManager::configurarInterrupcionReset() {
    pinMode(RESET_BUTTON_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(RESET_BUTTON_PIN), manejarInterrupcionReset, FALLING);
    Serial.println("Interrupcion de RESET configurada en pin " + String(RESET_BUTTON_PIN));
}

// Agrega una nueva mascota al sistema
bool EEPROMManager::agregarMascota(Mascota& mascota) {
    if (numMascotas >= MAX_MASCOTAS) {
        Serial.println("Error: Maximo numero de mascotas alcanzado");
        return false;
    }
    
    // Asignar ID de EEPROM
    mascota.setIdEEPROM(numMascotas);
    
    // Guardar la mascota en EEPROM
    mascota.guardarEnEEPROM();
    
    // Incrementar contador
    numMascotas++;
    EEPROM.write(DIRECCION_CONTADOR, numMascotas);
    
    Serial.println("Mascota agregada: " + mascota.getNombre() + " (ID: " + String(numMascotas-1) + ")");
    return true;
}

// Elimina una mascota del sistema
bool EEPROMManager::eliminarMascota(int indice) {
    if (indice < 0 || indice >= numMascotas) {
        Serial.println("Error: Indice de mascota invalido");
        return false;
    }
    
    // Mover todas las mascotas posteriores una posición hacia atrás
    for (int i = indice; i < numMascotas - 1; i++) {
        Mascota mascotaTemporal;
        mascotaTemporal.setIdEEPROM(i + 1);
        mascotaTemporal.cargarDeEEPROM();
        mascotaTemporal.setIdEEPROM(i);
        mascotaTemporal.guardarEnEEPROM();
    }
    
    // Decrementar contador
    numMascotas--;
    EEPROM.write(DIRECCION_CONTADOR, numMascotas);
    
    Serial.println("Mascota eliminada del indice: " + String(indice));
    return true;
}

// Carga todas las mascotas desde EEPROM
void EEPROMManager::cargarTodasLasMascotas(Mascota mascotas[]) {
    for (int i = 0; i < numMascotas; i++) {
        mascotas[i].setIdEEPROM(i);
        mascotas[i].cargarDeEEPROM();
    }
    Serial.println("Cargadas " + String(numMascotas) + " mascotas desde EEPROM");
}

// Guarda todas las mascotas en EEPROM
void EEPROMManager::guardarTodasLasMascotas(Mascota mascotas[]) {
    for (int i = 0; i < numMascotas; i++) {
        mascotas[i].setIdEEPROM(i);
        mascotas[i].guardarEnEEPROM();
    }
    Serial.println("Guardadas " + String(numMascotas) + " mascotas en EEPROM");
}

// Restaura todos los valores a los predeterminados
void EEPROMManager::restaurarValoresPredeterminados() {
    Serial.println("Restaurando valores predeterminados...");
    
    // Limpiar EEPROM
    limpiarEEPROM();
    
    // Establecer valores predeterminados
    establecerValoresPredeterminados();
    
    // Marcar como inicializada
    marcarComoInicializada();
    
    Serial.println("Valores predeterminados restaurados");
}

// Establece los valores predeterminados iniciales
void EEPROMManager::establecerValoresPredeterminados() {
    numMascotas = 0;
    EEPROM.write(DIRECCION_CONTADOR, numMascotas);
    
    Serial.println("Valores predeterminados establecidos");
}

// Limpia toda la EEPROM
void EEPROMManager::limpiarEEPROM() {
    Serial.println("Limpiando EEPROM...");
    for (int i = 0; i < EEPROM.length(); i++) {
        EEPROM.write(i, 0);
    }
    Serial.println("EEPROM limpiada");
}

// Verifica si la EEPROM está inicializada
bool EEPROMManager::estaInicializada() {
    return EEPROM.read(DIRECCION_INICIALIZADA) == MARCA_INICIALIZADA;
}

// Marca la EEPROM como inicializada
void EEPROMManager::marcarComoInicializada() {
    EEPROM.write(DIRECCION_INICIALIZADA, MARCA_INICIALIZADA);
}

// Getter para el número de mascotas
int EEPROMManager::getNumMascotas() const {
    return numMascotas;
}

// Maneja la interrupción del botón de RESET (función estática)
void EEPROMManager::manejarInterrupcionReset() {
    static unsigned long ultimaInterrupcion = 0;
    unsigned long tiempoActual = millis();
    
    // Debounce de 200ms
    if (tiempoActual - ultimaInterrupcion > 200) {
        resetPressed = true;
        ultimaInterrupcion = tiempoActual;
    }
}

// Verifica si se presionó el botón de RESET
bool EEPROMManager::sePresionoReset() {
    return resetPressed;
}

// Limpia el flag de RESET
void EEPROMManager::limpiarFlagReset() {
    resetPressed = false;
}

// Imprime el estado actual de la EEPROM (para depuración)
void EEPROMManager::imprimirEstadoEEPROM() {
    Serial.println("=== Estado de EEPROM ===");
    Serial.println("Inicializada: " + String(estaInicializada() ? "Si" : "No"));
    Serial.println("Numero de mascotas: " + String(numMascotas));
    Serial.println("Direccion contador: " + String(DIRECCION_CONTADOR));
    Serial.println("Direccion inicializada: " + String(DIRECCION_INICIALIZADA));
    Serial.println("========================");
}

// Imprime los datos de una mascota específica (para depuración)
void EEPROMManager::imprimirDatosMascota(int indice) {
    if (indice < 0 || indice >= numMascotas) {
        Serial.println("Error: Indice de mascota invalido para imprimir");
        return;
    }
    
    Mascota mascota;
    mascota.setIdEEPROM(indice);
    mascota.cargarDeEEPROM();
    
    Serial.println("=== Datos Mascota " + String(indice) + " ===");
    Serial.println("Nombre: " + mascota.getNombre());
    Serial.println("RFID: " + mascota.getRFID());
    Serial.println("Tiempo entre dosis: " + String(mascota.getTiempo()) + "s");
    Serial.println("Cantidad por dosis: " + String(mascota.getComida()) + "g");
    Serial.println("Total consumido: " + String(mascota.getTotalAlimentoConsumido()) + "g");
    Serial.println("================================");
}
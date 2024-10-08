#include <Arduino.h>

// Paramètres de modulation
#define HIGH_AMPLITUDE 4095  // Amplitude élevée pour '1' (valeur max du DAC 12 bits)
#define LOW_AMPLITUDE 0      // Amplitude basse pour '0'
#define BIT_PERIOD 1000      // Temps en microsecondes pour un bit (Tb)

float bitToVoltage(bool bit) {
    float highVoltage = 3.1;  // Tension haute pour '1'
    float lowVoltage = 1.9;   // Tension basse pour '0'

    return bit == 1 ? highVoltage : lowVoltage;
}

// Fonction pour convertir une tension en valeur DAC (12 bits)
uint16_t voltageToDACValue(float voltage) {
    const float maxVoltage = 3.3;  // Tension de référence
    const uint16_t maxDACValue = HIGH_AMPLITUDE;  // Valeur maximale du DAC
    
    uint16_t dacValue = (voltage / maxVoltage) * maxDACValue;
    return constrain(dacValue, 0, maxDACValue);  // S'assurer que la valeur est entre 0 et 4095
}

void setup() {
    // Initialisation du port série pour débogage
    Serial.begin(9600);
    // Initialisation du DAC (généralement géré automatiquement dans STM32);
    Serial.println("Modulation ASK démarrée...");
     analogWriteResolution(12);
}

void loop() {
    // Choisir un octet à transmettre (par exemple 0b10101010)
    uint8_t octet = 0b11001100;

    // Lire les bits de l'octet et générer la modulation ASK
    for (int i = 0; i < 8; i++) {
        bool bit = (octet >> (7 - i)) & 0x01; // Lire les bits du plus significatif au moins significatif
        
        
        // Envoyer une valeur élevée pour '1'
        uint16_t DACvalue= voltageToDACValue(bitToVoltage(bit));

        analogWrite(A2, DACvalue );
       
        // Pause de la durée Tb pour respecter la période du bit
        delayMicroseconds(BIT_PERIOD);
    }
    
    // Répéter en boucle
}
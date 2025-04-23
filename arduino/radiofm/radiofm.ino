#include <RFM69.h>
#include <SPI.h>

// --- Configuración ---
#define NODEID 1		 // ID de este nodo
#define NETWORKID 69	 // Red común para todos los nodos
#define DESTINATIONID 10 // Nodo al que se le enviará un mensaje
#define FREQUENCY RF69_868MHZ
#define IS_RFM69HW true // true si tu módulo es RFM69HW

#define RFM69_CS 10
#define RFM69_INT 3
#define RFM69_RST 4

RFM69 radio(RFM69_CS, RFM69_INT, IS_RFM69HW);

unsigned long ultimoEnvio = 0;
const unsigned long INTERVALO_ENVIO = 5000; // 5 segundos

void setup() {
	pinMode(RFM69_RST, OUTPUT);
	digitalWrite(RFM69_RST, HIGH);
	delay(10);
	digitalWrite(RFM69_RST, LOW);
	delay(10);

	Serial.begin(9600);
	delay(100);

	if (!radio.initialize(FREQUENCY, NODEID, NETWORKID)) {
		Serial.println("❌ Error al inicializar el RFM69");
		while (1)
			;
	}

	Serial.println("✅ RFM69 inicializado correctamente");
	radio.setHighPower(IS_RFM69HW);
	radio.encrypt(NULL); // Sin cifrado

	Serial.println("Esperando datos...");
}

void loop() {
	// --- RECEPCIÓN ---
	if (radio.receiveDone()) {
		Serial.print("📥 Mensaje recibido de nodo ");
		Serial.print(radio.SENDERID);
		Serial.print(": ");
		Serial.println((char *)radio.DATA);

		if (radio.ACKRequested()) {
			radio.sendACK();
			Serial.println("✔️  ACK enviado");
		}
	}

	// --- ENVÍO periódico ---
	if (millis() - ultimoEnvio > INTERVALO_ENVIO) {
		const char *mensaje = "Hola desde nodo 1";
		if (radio.sendWithRetry(DESTINATIONID, mensaje, strlen(mensaje))) {
			Serial.println("📤 Mensaje enviado y ACK recibido");
		} else {
			Serial.println("⚠️  No se recibió ACK del nodo destino");
		}
		ultimoEnvio = millis();
	}
}

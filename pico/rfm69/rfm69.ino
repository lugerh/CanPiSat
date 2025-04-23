#include <RH_RF69.h>
#include <SPI.h>

// Pines RFM69 en Raspberry Pi Pico
#define RFM69_CS 5	 // NSS
#define RFM69_INT 15 // DIO0 (interrupción)
#define RFM69_RST 3	 // RST
#define RF69_FREQ 868.0

RH_RF69 rf69(RFM69_CS, RFM69_INT);

void setup() {
	pinMode(RFM69_RST, OUTPUT);
	digitalWrite(RFM69_RST, LOW);
	delay(10);
	digitalWrite(RFM69_RST, HIGH);
	delay(10);
	digitalWrite(RFM69_RST, LOW);
	delay(10);

	Serial.begin(9600);
	while (!Serial)
		;

	if (!rf69.init()) {
		Serial.println("❌ Error inicializando RFM69");
		while (1)
			;
	}

	if (!rf69.setFrequency(RF69_FREQ)) {
		Serial.println("❌ Error configurando frecuencia");
		while (1)
			;
	}

	rf69.setTxPower(14, true); // true si es RFM69HW
	rf69.setThisAddress(1);	   // ID de este nodo
	rf69.setHeaderFrom(1);
	rf69.setHeaderTo(2); // Nodo destino

	uint8_t syncwords[] = {0x2D, 0x69}; // Grupo lógico
	rf69.setSyncWords(syncwords, sizeof(syncwords));

	Serial.println("✅ RFM69 listo en Raspberry Pi Pico");
	rf69.setModeRx(); // Esperar recepción
}

void loop() {
	if (rf69.available()) {
		uint8_t buf[RH_RF69_MAX_MESSAGE_LEN];
		uint8_t len = sizeof(buf);

		if (rf69.recv(buf, &len)) {
			Serial.print("📥 Mensaje recibido: ");
			Serial.write(buf, len);
			Serial.println();

			// Responder
			const char *reply = "ACK desde Pico";
			rf69.setHeaderTo(rf69.headerFrom());
			rf69.send((uint8_t *)reply, strlen(reply));
			rf69.waitPacketSent();
			Serial.println("✔️ ACK enviado");
			rf69.setModeRx();
		}
	}

	// Enviar cada 5 segundos
	static unsigned long lastSend = 0;
	if (millis() - lastSend > 5000) {
		const char *msg = "Hola desde Pico!";
		rf69.setHeaderTo(2);
		rf69.send((uint8_t *)msg, strlen(msg));
		rf69.waitPacketSent();
		Serial.println("📤 Mensaje enviado");
		rf69.setModeRx();
		lastSend = millis();
	}
}
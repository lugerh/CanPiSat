#include <RH_RF69.h>
#include <SPI.h>

// Pines de conexión
#define RFM69_INT 3 // DIO0
#define RFM69_CS 10 // NSS
#define RFM69_RST 4 // RESET
#define RF69_FREQ 868.0

#define NODE_ID 2		 // Este nodo
#define DESTINATION_ID 1 // Nodo al que se envía el mensaje

RH_RF69 rf69(RFM69_CS, RFM69_INT);

void setup() {
	// Configurar pin de reset
	pinMode(RFM69_RST, OUTPUT);
	digitalWrite(RFM69_RST, LOW);
	delay(100);
	digitalWrite(RFM69_RST, HIGH);
	delay(100);
	digitalWrite(RFM69_RST, LOW);
	delay(100); // Esperar un poco después del reset

	Serial.begin(9600);
	while (!Serial)
		; // Esperar en placas con USB nativo

	if (!rf69.init()) {
		Serial.println("Fallo al iniciar el módulo RFM69");
		while (1)
			;
	}

	if (!rf69.setFrequency(RF69_FREQ)) {
		Serial.println("Fallo al configurar frecuencia");
	}

	rf69.setTxPower(14, true); // Descomenta si usas RFM69HW
	uint8_t version = rf69.spiRead(0x10);
	Serial.print("Versión RFM69 detectada: 0x");
	Serial.println(version, HEX);
	if (version == 0x24) {
		Serial.println("El módulo está funcionando correctamente ✅");
	} else {
		Serial.println("Versión inesperada. Puede haber problema de "
					   "comunicación o daño ❌");
	}

	// Identificación del nodo
	rf69.setThisAddress(NODE_ID);
	rf69.setHeaderFrom(NODE_ID);
	rf69.setHeaderTo(DESTINATION_ID);

	// SyncWords (grupo lógico, opcional)
	uint8_t syncwords[] = {0x2D, 0x69}; // Debe coincidir en ambos nodos
	rf69.setSyncWords(syncwords, sizeof(syncwords));
	rf69.setPromiscuous(true);

	Serial.println("Módulo listo. Enviando y recibiendo datos...");
	rf69.setModeRx(); // Entra en modo recepción
}

void loop() {
	// Revisión de recepción
	if (rf69.available()) {
		uint8_t buf[RH_RF69_MAX_MESSAGE_LEN];
		uint8_t len = sizeof(buf);

		if (rf69.recv(buf, &len)) {
			Serial.print("Mensaje recibido: ");
			Serial.print(rf69.headerFrom());
			Serial.print(": ");
			Serial.write(buf, len); // Mostrar el mensaje recibido
			Serial.println();

			// Responder automáticamente con un mensaje simple
			const char *respuesta = "ACK desde receptor";
			rf69.setHeaderTo(rf69.headerFrom());
			rf69.send((uint8_t *)respuesta, strlen(respuesta));
			rf69.waitPacketSent();
			Serial.println("ACK enviado");
			rf69.setModeRx();
		} else {
			Serial.println("Error al recibir mensaje");
		}
	}

	// Aquí puedes poner un envío regular también si quieres actuar como emisor
	// Por ejemplo, cada 5 segundos enviar un mensaje
	static unsigned long ultimoEnvio = 0;

	if (millis() - ultimoEnvio > 5000) {
		Serial.println("Is alive");
		ultimoEnvio = millis();
	}
}
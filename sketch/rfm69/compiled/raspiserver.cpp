#include <RH_RF69.h>
#include <bcm2835.h>
#include <cstring>
#include <iostream>
#include <signal.h>

using namespace std;
#define BOARD_LORASPI
// Pines y configuracion
#include "../RasPiBoards.h"

#define RF_FREQUENCY 433.0
#define RF_GROUP_ID 69
#define RF_NODE_ID 1

RH_RF69 rf69(RF_CS_PIN, RF_IRQ_PIN);
volatile sig_atomic_t force_exit = false;

void sig_handler(int sig) { force_exit = true; }

int main() {
	signal(SIGINT, sig_handler);

	if (!bcm2835_init()) {
		cerr << "Error inicializando bcm2835" << endl;
		return 1;
	}

	if (!rf69.init()) {
		cerr << "No se detecta m      dulo RFM69" << endl;
		return 1;
	}

	rf69.setTxPower(20);
	rf69.setModemConfig(RH_RF69::FSK_MOTEINO);
	uint8_t syncwords[2] = {0x2d, RF_GROUP_ID};
	rf69.setSyncWords(syncwords, sizeof(syncwords));
	rf69.setFrequency(RF_FREQUENCY);
	rf69.setThisAddress(RF_NODE_ID);
	rf69.setPromiscuous(true); // Captura todo lo que llegue
	rf69.setModeRx();

	cout << "Escuchando mensajes en 433MHz... NodeID=" << RF_NODE_ID << endl;
	while (!force_exit) {
		if (rf69.available()) {
			uint8_t buf[RH_RF69_MAX_MESSAGE_LEN];
			uint8_t len = sizeof(buf);

			if (rf69.recv(buf, &len)) {
				cout << "Mensaje recibido (" << (int)len << " bytes): ";
				for (uint8_t i = 0; i < len; i++) {
					cout << buf[i];
				}
				cout << endl;
			}
		}

		bcm2835_delay(10); // peque      a espera no cr      tica
	}

	bcm2835_close();
	cout << "Finalizado." << endl;
	return 0;
}

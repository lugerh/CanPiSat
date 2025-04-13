#include <RH_RF69.h>
#include <bcm2835.h>
#include <cstring>
#include <iostream>
#include <signal.h>
#include <unistd.h> // para sleep()

using namespace std;

#define BOARD_LORASPI
#include "../RasPiBoards.h"

#define RF_FREQUENCY 433.0
#define RF_GROUP_ID 69
#define RF_GATEWAY_ID 1
#define RF_NODE_ID 10

RH_RF69 rf69(RF_CS_PIN, RF_IRQ_PIN);
volatile sig_atomic_t force_exit = false;

void sig_handler(int sig) { force_exit = true; }

int main(int argc, char *argv[]) {
	signal(SIGINT, sig_handler);

	if (argc < 2) {
		cout << "Uso: sudo ./testrf69 <mensaje>" << endl;
		return 1;
	}

	const char *mensaje_base = argv[1];

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
	rf69.setHeaderFrom(RF_NODE_ID);
	rf69.setHeaderTo(RF_GATEWAY_ID);

	cout << "Enviando: " << mensaje_base << " cada 1 segundo..." << endl;

	int contador = 0;

	while (!force_exit) {
		string mensaje = string(mensaje_base) + " " + to_string(contador);

		cout << "Enviando: " << mensaje << endl;

		rf69.send((uint8_t *)mensaje.c_str(), mensaje.length());
		rf69.waitPacketSent();

		contador++;
		sleep(1);
	}

	bcm2835_close();
	cout << "Programa finalizado." << endl;
	return 0;
}

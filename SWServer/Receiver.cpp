#include "Receiver.h"
#include <winsock2.h>
#include "Player.h"
#include <thread>
#include <vector>



void Receiver::register_player(SOCKET& sock, int id, std::vector<Player>& players) {
	ths.push_back(std::thread(&Receiver::worker_rcv, this, sock, id,std::ref(players)));
}

void Receiver::worker_rcv(SOCKET sock, int id, std::vector<Player>& players) {
	while (true) {
		char cmnd;
		recv(sock, &cmnd, 1, 0);
		int z = (int)(cmnd & 0x7);
		// printf("rcvd: %d\n", z);
		players[id].ship.update_ks(z);
		// fflush(stdout);
	}

}
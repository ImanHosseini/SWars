#pragma once
class GameSession;
#include <vector>
#include <thread>
#include <WinSock2.h>
#include "Player.h"


class Receiver {
public:

	std::vector<std::thread> ths;
	void register_player(SOCKET& sock, int id, std::vector<Player>& players);
	void worker_rcv(SOCKET sock, int id, std::vector<Player>& players);
};

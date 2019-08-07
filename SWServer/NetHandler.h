#pragma once
#include <WinSock2.h>
#include <vector>
#include <thread>
#include <random>
#include "Broadcaster.h"
#include "Receiver.h"
#include "Player.h"
#include <ws2tcpip.h>
#include <stdio.h>
#pragma comment(lib, "Ws2_32.lib")

// CONSTANTS-CONFIG
#define DEFAULT_PORT "27015"

class NetHandler {
public:
	Broadcaster bcast;
	Receiver rcvr;
	std::default_random_engine generator;
	std::vector<std::thread> clientHandlers;
	WSADATA wsaData;
	SOCKET ListenSocket;
	bool running = false;
	std::string addr_to_str(SOCKADDR_IN addr) {
		unsigned long IP = (addr.sin_addr.S_un.S_un_b.s_b1 << 24) | (addr.sin_addr.S_un.S_un_b.s_b2 << 16) | (addr.sin_addr.S_un.S_un_b.s_b3 << 8) | addr.sin_addr.S_un.S_un_b.s_b4;
		auto int_to_str = [](int val) -> std::string {
			char buf[20];
			sprintf_s(buf, "%i", val);
			return buf;
		};
		return int_to_str(IP >> 24) + "." + int_to_str((IP & 16777215) >> 16) + "." +
			int_to_str((IP & 65535) >> 8) + "." + int_to_str(IP & 255);
	}

	int setup(std::vector<Player>& players);
	void clientHandler(SOCKET,std::vector<Player>&);

};


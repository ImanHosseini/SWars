// SWServer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

// Common
#include <vector>
#include <cstdlib>
#include <chrono>
#include <random>
#include <mutex>
// For game logic
#include <iostream>
#include <string.h>
#include "glm/vec2.hpp"
#include "glm.hpp"
// For networking
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#pragma comment(lib, "Ws2_32.lib")
// For threads
#include <thread>


#include "GameSession.h"









class CntrlFrame {

};




















int main()
{
	NetHandler nh;
	GameSession gs(std::ref(nh));
	std::thread t(&GameSession::run, &gs);
	gs.net_handler.setup(gs.players);

	std::cout << "Hello World!\n";
	std::string str;
	std::cin >> str;
}


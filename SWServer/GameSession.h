#pragma once
#include <chrono>
#include <mutex>
#include <vector>
#include "NetHandler.h"

class Player;
class GameSession {
public:
	std::mutex df_lock;
	DrawFrame df;
	std::vector<Player> players;
	NetHandler& net_handler;
	GameSession(NetHandler& hd): net_handler(hd) {};
	void makeDrawFrame(const GameSession* gs, DrawFrame& df);




	bool running = false;
	std::chrono::milliseconds elapsed{ 20 };
	void run();
	void update(float dt);
	void updateA();
};



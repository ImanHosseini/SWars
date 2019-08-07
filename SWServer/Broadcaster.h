#pragma once
#include <thread>
#include <mutex>
#include "DrawFrame.h"
#include <vector>
#include <WinSock2.h>

class Broadcaster {
public:
	Broadcaster();
	Broadcaster(const Broadcaster& bcast);
	DrawFrame df;
	int fnum;
	std::condition_variable cv;
	std::vector<std::thread> ths;
	std::mutex m;
	void register_player(SOCKET& sock);

	void updatef(DrawFrame ndf);

	void broadcast();

	void worker_main(int frame, SOCKET sock);
};
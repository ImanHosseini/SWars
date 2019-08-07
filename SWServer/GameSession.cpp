#include "GameSession.h"
#include <chrono>
#include <vector>
#include <mutex>



void GameSession::makeDrawFrame(const GameSession* gs, DrawFrame& df) {
	char* dat;
	int len = 1 + gs->players.size() * sizeof(float) * 3;
	dat = new char[len];
	dat[0] = (byte)(len - 1);
	auto idx = 0;
	for (auto& p : gs->players) {
		// printf("%x - %d", dat[0],len);
		// memcpy_s(dat + idx * sizeof(ShipPos) + 1, sizeof(dat),(char*) &(p.ship.pos), sizeof(ShipPos));
		const float* x = &(p.ship.pos.pos.x);
		const float* y = &(p.ship.pos.pos.y);
		const float* t = &(p.ship.pos.theta);
		memcpy(dat + idx * 12 + 1, (void*)x, sizeof(float));
		memcpy(dat + idx * 12 + 5, (void*)y, sizeof(float));
		memcpy(dat + idx * 12 + 9, (void*)t, sizeof(float));
		//	memcpy(dat + idx * sizeof(ShipPos) + 1, (char*) & (p.ship.pos), sizeof(ShipPos));
		idx++;
	}
	if (df.dat != NULL) {
		delete[] df.dat;
	}

	df.dat = dat;
}

void GameSession::update(float dt) {
	
	
	for (auto& p : players) {
		p.ship.updatePos(dt);
		p.ship.updateAcc(dt);
		p.ship.updateVel(dt);
	}
}

void GameSession::updateA() {
	std::chrono::milliseconds dt{ 10 };
	while (true) {
		auto start = std::chrono::system_clock::now();
		printf("dt:%f\n", (float)dt.count());
		for (auto& p : players) {
			p.ship.updatePos((float)dt.count());
			p.ship.updateAcc((float)dt.count());
			p.ship.updateVel((float)dt.count());
		}
		auto end = std::chrono::system_clock::now();
		dt =
			std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	}
		
}

void GameSession::run() {
	running = true;
	std::thread t(&GameSession::updateA,this);
	while (running) {
		df_lock.lock();
		makeDrawFrame(this, df);
		net_handler.bcast.updatef(df);
		df_lock.unlock();
		net_handler.bcast.broadcast();
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
	}
}
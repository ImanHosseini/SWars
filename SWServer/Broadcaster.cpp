#include "Broadcaster.h"

Broadcaster::Broadcaster() :fnum(0), m(), cv() {};

Broadcaster::Broadcaster(const Broadcaster& bcast) : fnum(0), m(), cv() { };

void Broadcaster::register_player(SOCKET& sock) {
	ths.push_back(std::thread(&Broadcaster::worker_main, this, fnum, sock));
}

void Broadcaster::updatef(DrawFrame ndf) {
	{
		std::lock_guard<std::mutex> lk(m);
		df = ndf;
	}
	cv.notify_all();
}

void Broadcaster::broadcast() {
	{
		std::lock_guard<std::mutex> lk(m);
		fnum++;
	}
	// std::cout << "broadcast() signals data ready for processing\n";
	cv.notify_all();
}


void Broadcaster::worker_main(int frame, SOCKET sock) {
	int my_frame = frame;
	while (true) {
		std::unique_lock<std::mutex> lk(m);
		cv.wait(lk, [=] {return fnum > my_frame; });

		// after the wait, we own the lock.
		send(sock, df.dat, ((int)df.dat[0] + 1), 0);

		// Send data back to main()
	//	std::cout << "Worker thread signals data processing completed\n";
		my_frame++;

		// Manual unlocking is done before notifying, to avoid waking up
		// the waiting thread only to block again (see notify_one for details)
		lk.unlock();
		cv.notify_one();
		std::this_thread::sleep_for(std::chrono::milliseconds(5));
	}
}
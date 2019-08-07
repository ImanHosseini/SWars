
#include "NetHandler.h"
#include "Player.h"

int NetHandler::setup(std::vector<Player>& players) {
	struct addrinfo* result = NULL, * ptr = NULL, hints;

	running = true;
	int iResult;
	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed: %d\n", iResult);
		return 1;
	}
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the local address and port to be used by the server
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed: %d\n", iResult);
		WSACleanup();
		return 1;
	}
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}
	// Setup the TCP listening socket
	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}
	freeaddrinfo(result);
	// Lets listen
	if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR) {
		printf("Listen failed with error: %ld\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	// Accept a client socket
	while (running) {
		SOCKET clientSock = accept(ListenSocket, NULL, NULL);
		if (clientSock == INVALID_SOCKET) {
			printf("accept failed: %d\n", WSAGetLastError());
			closesocket(ListenSocket);
			WSACleanup();
			continue;
		}
		std::thread t(&NetHandler::clientHandler, this, clientSock,std::ref(players));
		t.join();
		// clientHandlers.push_back(std::thread(&NetHandler::clientHandler, this, clientSock));
	}

}



void NetHandler::clientHandler(SOCKET clientSock,std::vector<Player>& players) {
	SOCKET cso = clientSock;
	bool connected = true;
	SOCKADDR_IN client_info = { 0 };
	char* ipstr = new char[20];
	int addrsize = sizeof(client_info);
	// std::cout << "hi";
	// or get it from the socket itself at any time
	getpeername(clientSock, (SOCKADDR*)& client_info, &addrsize);
	std::string ip = addr_to_str(client_info);

	// std::cout << "Connected to client with ip: " << ip << "\n";

	std::uniform_real_distribution<float> distribution(0.0, 1.0);
	float dx = distribution(generator) * 300;
	float dy = distribution(generator) * 300;
	players.push_back(Player{ "taghi", Ship{glm::vec2{20.0f + dx,20.0f + dy}} });
	bcast.register_player(cso);
	rcvr.register_player(cso, players.size() - 1,players);
	return;
	//printf("\n%f-%f",20.0f+dx,20.0f+dy);
	//while (connected) {
	//	if (!gs.df.dat) {
	//		continue;
	//	}
	//	gs.df_lock.lock();
	///*	if (gs.df.dat[0] > 13) {
	//		printf("now");
	//	}*/
	//	send(cso, gs.df.dat, ((int)gs.df.dat[0]+1), 0);
	//	gs.df_lock.unlock();
	//	std::this_thread::sleep_for(std::chrono::milliseconds(600));
	//}
}
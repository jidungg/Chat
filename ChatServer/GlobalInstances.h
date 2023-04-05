#pragma once

extern shared_ptr<class ClientSessionManager> GSessionManager;
extern shared_ptr<class RoomManager> GRoomManager;

class GlobalInstances
{
public:
	GlobalInstances();
	~GlobalInstances();
};


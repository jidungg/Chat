#include "pch.h"
#include "GlobalInstances.h"
#include "ClientSessionManager.h"
#include "RoomManager.h"

shared_ptr<ClientSessionManager> GSessionManager;
shared_ptr<RoomManager> GRoomManager;

GlobalInstances::GlobalInstances()
{
	GSessionManager = MakeShared<ClientSessionManager>();
	GRoomManager = MakeShared<RoomManager>();
}

GlobalInstances::~GlobalInstances()
{

}

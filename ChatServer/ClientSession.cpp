#include "pch.h"
#include "ClientSession.h"
#include "ClientPacketHandler.h"
#include <iostream>
#include "UserProfile.h"
#include "ClientSessionManager.h"
#include "Room.h"
#include "RoomManager.h"

ClientSession::~ClientSession()
{
	cout << "~ClientSession" <<endl;
}

void ClientSession::OnConnected()
{

} 

void ClientSession::OnDisconnected()
{
	if (_user == nullptr) return;

	if (auto room = _user->room.lock())
	{
		GRoomManager->LeaveRoomAsync(room->roomNumber, _user);
	}

	GSessionManager->Remove(static_pointer_cast<ClientSession>(shared_from_this()));

	_user = nullptr;
}

void ClientSession::OnRecvPacket(BYTE* buffer, int32 len)
{
	PacketSessionRef session = GetPacketSessionRef();
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
	ClientPacketHandler::HandlePacket(session, buffer, len);
}

void ClientSession::OnSend(int32 len)
{

}

void ClientSession::SetUserProfile(string name)
{
	static Atomic<uint64> idGenerator = 1;
	UserProfileRef userRef = MakeShared<UserProfile>();
	userRef->name = name;
	userRef->userId = idGenerator++;
	userRef->ownerSession = static_pointer_cast<ClientSession>( shared_from_this());
	_user = userRef;
}

#include "pch.h"
#include "ServerPacketHandler.h"
#include "BufferReader.h"
#include "Protocol.pb.h"
#include "LoginManager.h"
#include "Room.h"
#include "ConsoleManager.h"
#include "UserProfile.h"


PacketHandlerFunc GPacketHandler[UINT16_MAX];

bool Handle_INVALID(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
	//TODO
	return false;
}

bool Handle_S_LOGIN(PacketSessionRef& session, Protocol::S_LOGIN& pkt)
{
	if (pkt.success() == false)
		return false;
	GLoginManager->OnLogin(pkt);
	GRoom->WaitForRoomNumber();

	return true;
}

bool Handle_S_ENTER_ROOM(PacketSessionRef& session, Protocol::S_ENTER_ROOM& pkt)
{
	if (pkt.success() == false)
		return false;

	GConsole->StartChat();
	GRoom->OnEnterRoom(pkt);
	return true;
}

bool Handle_S_LEAVE_ROOM(PacketSessionRef& session, Protocol::S_LEAVE_ROOM& pkt)
{
	if (GRoom->isInRoom == false) return false;

	GRoom->OnLeaveRoom();
	GConsole->OnLeaveRoom();

	GRoom->WaitForRoomNumber();

	return true;
}

bool Handle_S_OTHER_ENTERED_ROOM(PacketSessionRef& session, Protocol::S_OTHER_ENTERED_ROOM& pkt)
{
	if (GRoom->isInRoom == false) return false;
	if (GRoom->GetRoomNumber() != pkt.roomnumber()) return false;
	if (pkt.user().id() == GUser->userId) return false;

	GRoom->OnOtherEnterRoom(pkt);

	return true;
}

bool Handle_S_OTHER_LEAVED_ROOM(PacketSessionRef& session, Protocol::S_OTHER_LEAVED_ROOM& pkt)
{
	if (GRoom->isInRoom == false) return false;
	if (GRoom->GetRoomNumber() != pkt.roomnumber()) return false;
	if (pkt.user().id() == GUser->userId) return false;

	GRoom->OnOtherLeaveRoom(pkt);

	return false;
}



bool Handle_S_CHAT(PacketSessionRef& session, Protocol::S_CHAT& pkt)
{
	GConsole->OnChatReceive(pkt);
	return true;
}

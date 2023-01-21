#include "pch.h"
#include "ClientPacketHandler.h"
#include "UserProfile.h"
#include "Room.h"
#include "ClientSession.h"
#include "RoomManager.h"
#include "ClientSessionManager.h"

PacketHandlerFunc GPacketHandler[UINT16_MAX];
bool Handle_INVALID(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	return false;
}

bool Handle_C_LOGIN(PacketSessionRef& session, Protocol::C_LOGIN& pkt)
{
	string name = pkt.name();
	cout << name << " has requsted login.\n";

	//TODO: 로그인 처리
	ClientSessionRef clientSession = static_pointer_cast<ClientSession>(session);
	clientSession->SetUserProfile(name);
	GSessionManager->Add(clientSession);

	Protocol::S_LOGIN loginPkt;
	loginPkt.set_success(true);
	loginPkt.set_name(name);
	loginPkt.set_id(clientSession->_user->userId);
	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(loginPkt);
	session->Send(sendBuffer);

	return true;
}

bool Handle_C_ENTER_ROOM(PacketSessionRef& session, Protocol::C_ENTER_ROOM& pkt)
{
	ClientSessionRef clientSession = static_pointer_cast<ClientSession>(session);
	uint64 roomNumber = pkt.roomnumber();
	cout << clientSession->_user->name << " has requested Enter Room no." << roomNumber << '\n';

	GRoomManager->EnterRoomAsync(roomNumber, clientSession->_user);

	return true;
}

bool Handle_C_LEAVE_ROOM(PacketSessionRef& session, Protocol::C_LEAVE_ROOM& pkt)
{
	ClientSessionRef clientSession = static_pointer_cast<ClientSession>(session);
	uint64 roomNumber = pkt.userid();

	GRoomManager->LeaveRoomAsync(roomNumber, clientSession->_user);

	return true;
}

bool Handle_C_CHAT(PacketSessionRef& session, Protocol::C_CHAT& pkt)
{
	std::cout << pkt.msg() << endl;
	ClientSessionRef clientSession = static_pointer_cast<ClientSession>(session);

	Protocol::S_CHAT chatPkt;
	chatPkt.set_userid(clientSession->_user->userId);
	chatPkt.set_name(clientSession->_user->name);
	chatPkt.set_msg(pkt.msg());
	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(chatPkt);

	clientSession->_user->room.lock()->DoAsync(&Room::Broadcast, sendBuffer);

	return true;
}

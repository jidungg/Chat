#include "pch.h"
#include "Room.h"
#include "ServerPacketHandler.h"
#include "ServerSession.h"
#include "Protocol.pb.h"
#include "UserProfile.h"
#include "ConsoleManager.h"

shared_ptr<Room> GRoom = MakeShared<Room>();

void Room::ReqeusetEnterRoom(PacketSessionRef session, uint64 roomNum)
{
	Protocol::C_ENTER_ROOM enterRoomPacket;
	enterRoomPacket.set_roomnumber(roomNum);
	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(enterRoomPacket);
	session->Send(sendBuffer);
}
void Room::OnEnterRoom(Protocol::S_ENTER_ROOM& pkt)
{
	isInRoom = true;
	roomNumber = pkt.roomnumber();
	for (int i = 0; i < pkt.users_size(); i++)
		AddUser(const_cast<Protocol::User&> (pkt.users(i)));
}
void Room::OnOtherEnterRoom(Protocol::S_OTHER_ENTERED_ROOM& pkt)
{
	Protocol::User enteredUser = pkt.user();
	AddUser(const_cast<Protocol::User&> (pkt.user()));
}
void Room::AddUser(Protocol::User& userToAdd)
{
	UserProfileRef user = MakeShared<UserProfile>();
	user->name = userToAdd.name();
	user->userId = userToAdd.id();
	users[user->userId] = user;

	GConsole->PrintChat("User " + user->name + " Added in room.\n");
}

void Room::RequesetLeaveRoom(PacketSessionRef session)
{
	Protocol::C_LEAVE_ROOM leaveRoomPacket;
	leaveRoomPacket.set_userid(GUser->userId);
	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(leaveRoomPacket);
	session->Send(sendBuffer);
}
void Room::OnOtherLeaveRoom(Protocol::S_OTHER_LEAVED_ROOM& pkt)
{
	Protocol::User leavedUser = pkt.user();
	RemoveUser(leavedUser);
}


void Room::RemoveUser(Protocol::User& user)
{
	users.erase(user.id());
	GConsole->PrintChat("User " + user.name() + " Leaved room.\n");
}

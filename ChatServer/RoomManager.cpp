#include "pch.h"
#include "RoomManager.h"
#include "Room.h"
#include "ClientPacketHandler.h"
#include "ClientSession.h"
#include "UserProfile.h"

shared_ptr<RoomManager> GRoomManager = MakeShared<RoomManager>();

void RoomManager::EnterRoomAsync(uint64 roomNum, UserProfileRef user)
{
	DoAsync(&RoomManager::TryEnterRoom, roomNum, user);
}
void RoomManager::TryEnterRoom(uint64 roomNum, UserProfileRef user)
{
	if (IsRoomExist(roomNum) == false) {
		cout << "There is no Room no." << roomNum << ". Create Room.\n";
		CreateRoom(roomNum);
	}
	EnterRoom(roomNum, user);
}
void RoomManager::EnterRoom(uint64 roomNum, UserProfileRef user)
{
	cout << "Entering room no." << roomNum << " Permitted.\n";

	_rooms[roomNum]->Enter(user);
	SendEnterRoomSuccess(roomNum, user);
	SendEnterRoomOther(roomNum, user);
}

void RoomManager::SendEnterRoomSuccess(uint64 roomNum, UserProfileRef user)
{
	Protocol::S_ENTER_ROOM enterRoomPkt;
	enterRoomPkt.set_success(true);
	enterRoomPkt.set_roomnumber(roomNum);
	for (auto iter = _rooms[roomNum]->users.begin(); iter != _rooms[roomNum]->users.end(); iter++)
	{
		auto user = enterRoomPkt.add_users();
		user->set_id(iter->first);
		user->set_name(iter->second->name);
		user->set_usertype(Protocol::PLAYER_TYPE_NORMAL);
	}
	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(enterRoomPkt);
	user->ownerSession->Send(sendBuffer);
}

void RoomManager::SendEnterRoomOther(uint64 roomNum, UserProfileRef enteredUser)
{
	Protocol::S_OTHER_ENTERED_ROOM otherEnterPkt;
	otherEnterPkt.set_roomnumber(roomNum);

	Protocol::User* u= new Protocol::User();
	u->set_id(enteredUser->userId);
	u->set_name(enteredUser->name);
	u->set_usertype(Protocol::PLAYER_TYPE_NORMAL);
	otherEnterPkt.set_allocated_user(u);
	
	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(otherEnterPkt);
	BroadCast(roomNum, sendBuffer);
}

void RoomManager::SendEnterRoomFail(UserProfileRef user)
{
	Protocol::S_ENTER_ROOM enterRoomPkt;
	enterRoomPkt.set_success(false);
	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(enterRoomPkt);
	user->ownerSession->Send(sendBuffer);
}

void RoomManager::LeaveRoomAsync(uint64 roomNum, UserProfileRef user)
{
	DoAsync(&RoomManager::TryLeaveRoom, roomNum, user);
}
void RoomManager::TryLeaveRoom(uint64 roomNum, UserProfileRef user)
{
	if (IsRoomExist(roomNum) == false) {
		cout << "There is no Room no." << roomNum << "\n";
		SendLeaveRoomFail(user);
		return;
	}
	if (_rooms[roomNum]->IsUserInRoom(user->userId) == false ||
		user->room.lock() != _rooms[roomNum])
	{
		cout << "There is no User no." << user->userId << "in Room no." << roomNum << '\n';
		SendLeaveRoomFail(user);
		return;
	}
	LeaveRoom(roomNum, user);
}
void RoomManager::LeaveRoom(uint64 roomNum, UserProfileRef user)
{
	cout << "User no." << user->userId << " leaved Room no." << roomNum << '\n';

	_rooms[roomNum]->Leave(user);
	if (_rooms[roomNum]->UserCount() == 0)
		_rooms.erase(roomNum);
	else
		SendLeaveRoomOther(roomNum, user);

	SendLeaveRoomSuccess(user);
}
void RoomManager::SendLeaveRoomSuccess(UserProfileRef user)
{
	Protocol::S_LEAVE_ROOM enterRoomPkt;
	enterRoomPkt.set_success(true);
	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(enterRoomPkt);
	user->ownerSession->Send(sendBuffer);
}
void RoomManager::SendLeaveRoomFail(UserProfileRef user)
{
	Protocol::S_LEAVE_ROOM leaveRoomPacket;
	leaveRoomPacket.set_success(false);
	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(leaveRoomPacket);
	user->ownerSession->Send(sendBuffer);
}

void RoomManager::SendLeaveRoomOther(uint64 roomNum, UserProfileRef user)
{
	Protocol::S_OTHER_LEAVED_ROOM otherEnterPkt;
	otherEnterPkt.set_roomnumber(roomNum);

	Protocol::User u;
	u.set_id(user->userId);
	u.set_name(user->name);
	u.set_usertype(Protocol::PLAYER_TYPE_NORMAL);
	otherEnterPkt.set_allocated_user(&u);

	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(otherEnterPkt);
	BroadCast(roomNum, sendBuffer);
}


void RoomManager::CreateRoom(uint64 roomNum)
{
	_rooms[roomNum] = MakeShared<Room>();
	_rooms[roomNum]->roomNumber = roomNum;
}

void RoomManager::DestroyRoom(uint64 roomNum)
{
	_rooms.erase(roomNum);
}

void RoomManager::BroadCast(uint64 roomNum, SendBufferRef buffer)
{
	_rooms[roomNum]->Broadcast(buffer);
}





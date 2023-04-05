#pragma once
#include "Protocol.pb.h"
class Room
{
public :
	void ReqeusetEnterRoom(PacketSessionRef session, uint64 roomNum);
	void OnEnterRoom(Protocol::S_ENTER_ROOM& pkt);
	void RequesetLeaveRoom(PacketSessionRef session);
	void OnOtherEnterRoom(Protocol::S_OTHER_ENTERED_ROOM& pkt);
	void OnOtherLeaveRoom(Protocol::S_OTHER_LEAVED_ROOM& pkt);
	uint64 GetRoomNumber() { return roomNumber; }

private:
	void AddUser(Protocol::User &user);
	void RemoveUser(Protocol::User& user);
public:
	bool isInRoom = false;
private:
	map<uint64, UserProfileRef> users;
	uint64 roomNumber;
};


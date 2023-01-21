#pragma once
class RoomManager : public JobQueue
{
public:
	void EnterRoomAsync(uint64 roomNum, UserProfileRef user);
	void LeaveRoomAsync(uint64 roomNum, UserProfileRef user);

private:
	void CreateRoom(uint64 roomNum);
	void DestroyRoom(uint64 roomNum);

	void TryEnterRoom(uint64 roomNum, UserProfileRef user);
	void EnterRoom(uint64 roomNum, UserProfileRef user);
	void SendEnterRoomSuccess(uint64 roomNum, UserProfileRef user);
	void SendEnterRoomOther(uint64 roomNum, UserProfileRef enteredUser );
	void SendEnterRoomFail(UserProfileRef user);

	void TryLeaveRoom(uint64 roomNum, UserProfileRef user);
	void LeaveRoom(uint64 roomNum, UserProfileRef user);
	void SendLeaveRoomSuccess(UserProfileRef user);
	void SendLeaveRoomFail(UserProfileRef user);
	void SendLeaveRoomOther(uint64 roomNum, UserProfileRef leavedUser);

	void BroadCast(uint64 roomNum, SendBufferRef buffer);

private:
	bool IsRoomExist(uint64 roomNum) { return _rooms.find(roomNum) != _rooms.end(); }
private:
	Map<uint64, RoomRef> _rooms;
};

extern shared_ptr<RoomManager> GRoomManager;
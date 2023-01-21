#pragma once
#include "Job.h"
#include "JobQueue.h"

class Room : public JobQueue
{
public :
	~Room();
public:
	// 싱글쓰레드 환경인마냥 코딩
	void Enter(UserProfileRef player);
	void Leave(UserProfileRef player);
	void Broadcast(SendBufferRef sendBuffer);
	uint64 UserCount() { return users.size(); }
	bool IsUserInRoom(uint64 userId) { return users.find(userId) != users.end(); }
	
public:
	uint64 roomNumber;
	Map<uint64, UserProfileRef> users;

};


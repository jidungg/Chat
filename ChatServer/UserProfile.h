#pragma once
class UserProfile
{
public :
	~UserProfile();
public:
	uint64 userId = 0;
	string name;
	//TODO: 상호 참조 끊기
	ClientSessionRef ownerSession;
	weak_ptr<class Room> room;
};


#pragma once
class UserProfile
{
public :
	~UserProfile();
public:
	uint64 userId = 0;
	string name;
	//TODO: ��ȣ ���� ����
	ClientSessionRef ownerSession;
	weak_ptr<class Room> room;
};


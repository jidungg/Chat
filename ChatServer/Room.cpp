#include "pch.h"
#include "Room.h"
#include "UserProfile.h"
#include "ClientSession.h"
#include "ClientPacketHandler.h"

Room::~Room()
{
	cout << "~Room no." << roomNumber<<'\n';
}

void Room::Enter(UserProfileRef user)
{
	users[user->userId] = user;
	user->room =  static_pointer_cast<Room>( shared_from_this());
}

void Room::Leave(UserProfileRef user)
{
	users.erase(user->userId);
}

void Room::Broadcast(SendBufferRef sendBuffer)
{
	for (auto& u : users)
	{
		u.second->ownerSession->Send(sendBuffer);
	}

}


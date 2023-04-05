#include "pch.h"
#include "LoginManager.h"
#include "ServerPacketHandler.h"
#include "ServerSession.h"
#include "UserProfile.h"
#include "Protocol.pb.h"



void LoginManager::RequestLogin()
{
	//Temporary
	cout << "Connected to Server. Enter your name.\n";
	string name;
	cin >> name;
	//TODO: 로그인 처리

	Protocol::C_LOGIN pkt;
	pkt.set_name(name);
	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
	serverSession->Send(sendBuffer);
}

void LoginManager::OnLogin(Protocol::S_LOGIN& pkt)
{
	cout << "Login Success.\n";
	isLogined = true;
	GUser->name = pkt.name();
	GUser->userId = pkt.id();
}

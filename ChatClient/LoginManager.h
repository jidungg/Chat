#pragma once
#include "Protocol.pb.h"

class LoginManager
{
public:
	void RequestLogin();
	void OnLogin(Protocol::S_LOGIN& pkt);
	void SetServerSession(ServerSessionRef session) { serverSession = session; }
private:
	bool isLogined;
	ServerSessionRef serverSession;
};


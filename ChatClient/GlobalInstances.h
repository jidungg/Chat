#pragma once

extern shared_ptr<class ConsoleManager> GConsole;
extern shared_ptr<class LoginManager > GLoginManager;
extern shared_ptr<class Room> GRoom;
extern shared_ptr<class UserProfile> GUser;

class GlobalInstances
{
public:
	GlobalInstances();
	~GlobalInstances();
};



#include "pch.h"
#include "GlobalInstances.h"
#include "ConsoleManager.h"
#include "LoginManager.h"
#include "Room.h"
#include "UserProfile.h"

shared_ptr<ConsoleManager> GConsole;
shared_ptr<LoginManager> GLoginManager;
shared_ptr<Room> GRoom;
shared_ptr<UserProfile> GUser;

GlobalInstances::GlobalInstances()
{
	GConsole = MakeShared<ConsoleManager>();
	GLoginManager = MakeShared<LoginManager>();
	GRoom =MakeShared<Room>();
	GUser = MakeShared<UserProfile>();
}

GlobalInstances::~GlobalInstances()
{

}



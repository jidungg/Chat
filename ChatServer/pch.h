#pragma once
#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.

#define _WINSOCKAPI_
#ifdef _DEBUG
#pragma comment(lib,"ServerCore\\Debug\\ServerCore.lib")
#pragma comment(lib,"Protobuf\\Debug\\libprotobufd.lib")
#else
#pragma comment(lib,"ServerCore\\Release\\ServerCore.lib")
#pragma comment(lib,"Protobuf\\Release\\libprotobuf.lib")

#endif

#include "CorePch.h"
#include "GlobalInstances.h"

using ClientSessionRef = shared_ptr<class ClientSession>;

using UserProfileRef = shared_ptr<class UserProfile>;
using RoomRef = shared_ptr<class Room>;
using ClientSessionManagerRef = shared_ptr<class ClientSessionManager>;
#include "pch.h"

#include <iostream>
#include "Service.h"
#include "Session.h"
#include "ServerSession.h"
#include "ThreadManager.h"
#include "BufferReader.h"
#include "ServerPacketHandler.h"
#include <tchar.h>
#include "ConsoleManager.h"

int main()
{
	std::cout << "Client\n";
	ServerPacketHandler::Init();

	wstring  ip;
	cout << "Input Server IP: ";
	wcin >> ip;

	ClientServiceRef service = MakeShared<ClientService>(
		NetAddress(ip,7777),
		MakeShared<IocpCore>(),
		MakeShared<ServerSession>,
		1);

	ASSERT_CRASH(service->Start());

	for (int32 i = 0; i < 1; i++)
	{
		GThreadManager->Launch([=]()
			{
				while (true)
				{
					service->GetIocpCore()->Dispatch();
				}
			});
	}

	GThreadManager->Join();
}


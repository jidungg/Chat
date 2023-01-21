
#include "pch.h"
#include <iostream>
#include "Service.h"
#include "Session.h"
#include "ClientSession.h"
#include "ThreadManager.h"
#include "ClientPacketHandler.h"
enum
{
	WORKER_TICK = 64
};

void DoWorkerJob(ServerServiceRef& service)
{
	while (true)
	{
		LEndTickCount = ::GetTickCount64() + WORKER_TICK;

		service->GetIocpCore()->Dispatch(10);

		//타이머 Job 분배
		ThreadManager::DistributeReservedJobs();

		// 글로벌 큐
		ThreadManager::DoGlobalQueueWork();
	}
}
int main()
{
	ClientPacketHandler::Init();
	std::cout << "Server\n";

	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(7777);

	ServerServiceRef service = MakeShared<ServerService>(
		NetAddress(addr),
		MakeShared<IocpCore>(),
		MakeShared<ClientSession>,
		1);
	ASSERT_CRASH( service->Start());

	for (int32 i = 0; i < 5; i++)
	{
		GThreadManager->Launch([&service]()
			{
				DoWorkerJob(service);
			});
	}

	// Main Thread
	DoWorkerJob(service);

	GThreadManager->Join();
}


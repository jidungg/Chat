//전역으로 사용할 매니저 클래스들 포함.
#pragma once

extern class ThreadManager*			GThreadManager;
extern class SendBufferManager*		GSendBufferManager;
extern class GlobalQueue* GGlobalQueue;
extern class JobTimer* GJobTimer;

extern class DeadLockProfiler*		GDeadLockProfiler;
extern class DBConnectionPool*		GDBConnectionPool;
extern class ConsoleLog*			GConsoleLogger;

class CoreGlobal
{
public:
	CoreGlobal();
	~CoreGlobal();
	static void Instantiate();
};
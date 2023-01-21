#pragma once
class ClientSession;



class ClientSessionManager
{
public:
	void Add(ClientSessionRef session);
	void Remove(ClientSessionRef session);
	void BroadCast(SendBufferRef sendBuffer);

private:
	USE_LOCK;
	Set<ClientSessionRef> _sessions;
};

using ClientSessionManagerRef = shared_ptr<ClientSessionManager>;
extern shared_ptr<ClientSessionManager> GSessionManager;
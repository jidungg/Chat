#pragma once
class ServerSession : public PacketSession
{
public:
	~ServerSession()
	{
		cout << "~ServerSession" << endl;
	}

	virtual void OnConnected() override;
	virtual void OnDisconnected() override;
	virtual void OnRecvPacket(BYTE* buffer, int32 len);
	virtual void OnSend(int32 len) override;
};

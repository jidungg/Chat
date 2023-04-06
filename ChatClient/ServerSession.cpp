#include "pch.h"
#include "ServerSession.h"
#include "ServerPacketHandler.h"
#include "LoginManager.h"
#include "ConsoleManager.h"
#include "Room.h"

void ServerSession::OnConnected()
{
	GLoginManager->SetServerSession(static_pointer_cast<ServerSession>(shared_from_this()));
	GConsole->SetServerSession(static_pointer_cast<ServerSession>(shared_from_this()));
	GRoom->SetServerSession(static_pointer_cast<ServerSession>(shared_from_this()));
	GLoginManager->RequestLogin();
}

void ServerSession::OnDisconnected()
{
	if (GRoom->isInRoom != false)
	{
		GRoom->OnLeaveRoom();
		GConsole->OnLeaveRoom();
	}

	cout << "Server Disconnected. Please try later.\n";

}

void ServerSession::OnRecvPacket(BYTE* buffer, int32 len)
{
	PacketSessionRef session = GetPacketSessionRef();
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);

	ServerPacketHandler::HandlePacket(session, buffer, len);
}

void ServerSession::OnSend(int32 len)
{

}

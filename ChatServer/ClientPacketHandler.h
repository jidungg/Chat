#pragma once
#include "Protocol.pb.h"

using PacketHandlerFunc = std::function<bool(PacketSessionRef&, BYTE*, int32)>;
extern PacketHandlerFunc GPacketHandler[UINT16_MAX];


enum : uint16
{
	PKT_C_LOGIN =1000,
	PKT_S_LOGIN =1001,
	PKT_C_ENTER_ROOM =1002,
	PKT_S_ENTER_ROOM =1003,
	PKT_C_LEAVE_ROOM =1004,
	PKT_S_LEAVE_ROOM =1005,
	PKT_S_OTHER_ENTERED_ROOM =1006,
	PKT_S_OTHER_LEAVED_ROOM =1007,
	PKT_C_CHAT =1008,
	PKT_S_CHAT =1009,
};

bool Handle_INVALID(PacketSessionRef& session, BYTE* buffer, int32 len);
bool Handle_C_LOGIN(PacketSessionRef& session, Protocol::C_LOGIN&pkt);
bool Handle_C_ENTER_ROOM(PacketSessionRef& session, Protocol::C_ENTER_ROOM&pkt);
bool Handle_C_LEAVE_ROOM(PacketSessionRef& session, Protocol::C_LEAVE_ROOM&pkt);
bool Handle_C_CHAT(PacketSessionRef& session, Protocol::C_CHAT&pkt);


class ClientPacketHandler
{
public:
	static void Init()
	{
		for (int32 i = 0; i < UINT16_MAX; i++)
		{
			GPacketHandler[i] = Handle_INVALID;
		}
		GPacketHandler[PKT_C_LOGIN] = [](PacketSessionRef& session, BYTE* buffer, int32 len) {return HandlePacket<Protocol::C_LOGIN >(Handle_C_LOGIN, session, buffer, len); };
		GPacketHandler[PKT_C_ENTER_ROOM] = [](PacketSessionRef& session, BYTE* buffer, int32 len) {return HandlePacket<Protocol::C_ENTER_ROOM >(Handle_C_ENTER_ROOM, session, buffer, len); };
		GPacketHandler[PKT_C_LEAVE_ROOM] = [](PacketSessionRef& session, BYTE* buffer, int32 len) {return HandlePacket<Protocol::C_LEAVE_ROOM >(Handle_C_LEAVE_ROOM, session, buffer, len); };
		GPacketHandler[PKT_C_CHAT] = [](PacketSessionRef& session, BYTE* buffer, int32 len) {return HandlePacket<Protocol::C_CHAT >(Handle_C_CHAT, session, buffer, len); };
		
	}
	static bool HandlePacket(PacketSessionRef& session, BYTE* buffer, int32 len)
	{
		PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
		return GPacketHandler[header->id](session, buffer, len);
	}
	static SendBufferRef MakeSendBuffer(Protocol::S_LOGIN& pkt) { return MakeSendBuffer(pkt, PKT_S_LOGIN); }
	static SendBufferRef MakeSendBuffer(Protocol::S_ENTER_ROOM& pkt) { return MakeSendBuffer(pkt, PKT_S_ENTER_ROOM); }
	static SendBufferRef MakeSendBuffer(Protocol::S_LEAVE_ROOM& pkt) { return MakeSendBuffer(pkt, PKT_S_LEAVE_ROOM); }
	static SendBufferRef MakeSendBuffer(Protocol::S_OTHER_ENTERED_ROOM& pkt) { return MakeSendBuffer(pkt, PKT_S_OTHER_ENTERED_ROOM); }
	static SendBufferRef MakeSendBuffer(Protocol::S_OTHER_LEAVED_ROOM& pkt) { return MakeSendBuffer(pkt, PKT_S_OTHER_LEAVED_ROOM); }
	static SendBufferRef MakeSendBuffer(Protocol::S_CHAT& pkt) { return MakeSendBuffer(pkt, PKT_S_CHAT); }
private:
	template<typename PacketType, typename ProcessFunc>
	static bool HandlePacket(ProcessFunc func, PacketSessionRef& session, BYTE* buffer, int32 len)
	{
		PacketType pkt;
		if (pkt.ParseFromArray(buffer + sizeof(PacketHeader), len - sizeof(PacketHeader)) == false)
			return false;

		return func(session, pkt);
	}

	template<typename T>
	static SendBufferRef MakeSendBuffer(T& pkt, uint16 pktId)
	{
		const uint16 dataSize = static_cast<uint16>(pkt.ByteSizeLong());
		const uint16 packetSize = dataSize + sizeof(PacketHeader);

		SendBufferRef sendBuffer = GSendBufferManager->Open(packetSize);

		PacketHeader* header = reinterpret_cast<PacketHeader*>(sendBuffer->Buffer());
		header->size = packetSize;
		header->id = pktId;

		ASSERT_CRASH(pkt.SerializeToArray(&header[1], dataSize));

		sendBuffer->Close(packetSize);
		return sendBuffer;
	}

};
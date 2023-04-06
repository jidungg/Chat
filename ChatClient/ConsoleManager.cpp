#include "pch.h"
#include "ConsoleManager.h"
#include "ThreadManager.h"
#include "Protocol.pb.h"
#include "ServerPacketHandler.h"
#include "ServerSession.h"
#include <conio.h>
#include "Protocol.pb.h"
#include "Room.h"


void ConsoleManager::StartChat()
{
	Initialize();
	DrawChatScreen();
	StartPrompting();
}
void ConsoleManager::Initialize()
{
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO* csbiInfo = GetScreenBufferInfo();

	chatSpaceHeight = csbiInfo->srWindow.Bottom - csbiInfo->srWindow.Top - 1;
	chatSpaceWidth = csbiInfo->srWindow.Right - csbiInfo->srWindow.Left;
	screenHeight = csbiInfo->srWindow.Bottom - csbiInfo->srWindow.Top;
	topPos = csbiInfo->srWindow.Top;
	dividingLineRow = chatSpaceHeight + 1;
	dividingLineWidth = chatSpaceWidth;
	currentLine = 1;
	promptPos = new Point(dividingLineRow + 1, 1);
	isPrompting = false;

	delete csbiInfo;
}

void ConsoleManager::DrawChatScreen()
{
	system("cls");
	DrawDividingLine(dividingLineRow);
}

void ConsoleManager::DrawDividingLine(int32 row)
{
	MoveCursorToRow(row);
	for (int i = 0; i < dividingLineWidth; i++)
	{
		printf("%c", dividingLineChar);
	}
	MoveCursorToNextRow();

}

void ConsoleManager::StartPrompting()
{
	isPrompting = true;

	GThreadManager->Launch([=]()
		{
			ProcessInput();
		});
}
void ConsoleManager::ProcessInput()
{
	char inputChar;
	while (isPrompting)
	{
		inputChar = _getch();
		MoveCursorToPos(promptPos);
		if (IsEnter(inputChar))
		{
			SendChat();
		}
		else if (IsDel(inputChar))
		{
			DeletePromptChar();
		}
		else if (IsESC(inputChar))
		{
			LeaveChat();
		}
		else if (IsChattableChar(inputChar))
		{
			PromptInputChar(inputChar);
		}

	}
}
void ConsoleManager::SendChat()
{
	SendChatPacket(promptString);
	promptPos->Reset();
	promptString = "";
	EraseLine(promptPos->row);
}
void ConsoleManager::SendChatPacket(string msg)
{
	Protocol::C_CHAT chatPkt;
	chatPkt.set_msg(msg);
	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(chatPkt);
	serverSession->Send(sendBuffer);
}
void ConsoleManager::DeletePromptChar()
{
	promptString.erase(promptString.size() - 1, 1);
	promptPos->col--;
	MoveCursorToPos(promptPos);
	cout << ' ';
	MoveCursorToPos(promptPos);
}
void ConsoleManager::LeaveChat()
{
	GRoom->RequesetLeaveRoom(serverSession);
}
void ConsoleManager::EndPrompting()
{
	isPrompting = false;
	promptPos->Reset();
	promptString.clear();
}
void ConsoleManager::PromptInputChar(char input)
{
	promptString += input;
	MoveCursorToPos(promptPos);
	cout << input;
	promptPos->col++;
}

CONSOLE_SCREEN_BUFFER_INFO* ConsoleManager::GetScreenBufferInfo()
{
	CONSOLE_SCREEN_BUFFER_INFO* csbiInfo = new CONSOLE_SCREEN_BUFFER_INFO();
	if (!GetConsoleScreenBufferInfo(hStdout, csbiInfo))
	{
		printf("GetConsoleScreenBufferInfo (%d)\n", GetLastError());
		return nullptr;
	}
	return csbiInfo;
}




void ConsoleManager::OnChatReceive(Protocol::S_CHAT& pkt)
{
	string chat = pkt.name() + ':' + pkt.msg();
	PrintChat(chat);
}

void ConsoleManager::OnLeaveRoom()
{
	EndPrompting();
	system("cls");
}

void ConsoleManager::PrintChat(string chat)
{
	if (IsChatSpaceFull())
	{
		RepositioningUI();
		PrintLine(dividingLineRow - 1,chat);
	}
	else
	{
		PrintLine(currentLine++, chat);
	}
}
void ConsoleManager::RepositioningUI()
{
	EraseUI();
	ScrollScreen(-1);
	DrawDividingLine(dividingLineRow);
	PrintLine(promptPos->row, promptString);
}
void ConsoleManager::PrintLine(int32 row, string text)
{
	MoveCursorToRow(row);
	cout << text;
	MoveCursorToPos(promptPos);
}

void ConsoleManager::EraseUI()
{
	CONSOLE_SCREEN_BUFFER_INFO* csbiInfo = GetScreenBufferInfo();

	int currentScreenHeight = csbiInfo->srWindow.Bottom - csbiInfo->srWindow.Top;
	delete csbiInfo;

	if (screenHeight > currentScreenHeight)
	{
		int currentDividingLineRow = currentScreenHeight - 1;
		int currentPromptRow = currentDividingLineRow + 1;
		EraseLine(currentDividingLineRow);
		EraseLine(currentPromptRow);
	}
	else
	{
		EraseLine(dividingLineRow);
		EraseLine(promptPos->row);
	}
}
int32 ConsoleManager::ScrollScreen(int iRows)
{
	//TODO: 화면 크기 조정 시 이상 작동 해결
	CONSOLE_SCREEN_BUFFER_INFO* csbiInfo;
	csbiInfo = GetScreenBufferInfo();
	SMALL_RECT srctWindow;
	srctWindow = csbiInfo->srWindow;

	// Check whether the window is too close to the screen buffer top
	if (csbiInfo->srWindow.Top >= iRows)
	{
		delete csbiInfo;
		srctWindow.Top = topPos - (SHORT)iRows;     // move top up
		srctWindow.Bottom = topPos + screenHeight - (SHORT)iRows;  // move bottom up
		srctWindow.Left = 0;         // no change
		srctWindow.Right = chatSpaceWidth;        // no change

		if (!SetConsoleWindowInfo(
			hStdout,          // screen buffer handle
			TRUE,            // relative coordinates
			&srctWindow))     // specifies new location
		{
			printf("SetConsoleWindowInfo (%d)\n", GetLastError());
			return 0;
		}
		topPos = srctWindow.Top;
		return iRows;
	}
	else
	{
		delete csbiInfo;
		printf("\nCannot scroll; the window is too close to the top.\n");
		return 0;
	}
}

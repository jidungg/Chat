#pragma once
#include "Protocol.pb.h"

struct Point
{
public :
	Point(int32 r, int32 c) {
		row = r;
		col = c;
		resetRow = r;
		resetCol = c;
	}
	void Reset()
	{
		row = resetRow;
		col = resetCol;
	}
	int32 row;
	int32 col;

private:
	int32 resetRow;
	int32 resetCol;
};
class ConsoleManager
{
public:
	void SetServerSession(ServerSessionRef session) { serverSession = session; }
	void StartChat();
	void OnS_CHAT(Protocol::S_CHAT& pkt);
	void OnS_LEAVE_ROOM();
	void PrintChat(string chat);

private:
	void Initialize();
	void DrawChatScreen();
	void DrawDividingLine(int32 row);
	void StartPrompting();
	void Prompting();
	void SendChat();
	void SendChatPacket(string msg);
	void DeletePromptChar();
	void LeaveChat();
	void EndPrompting();
	void PromptInputChar(char input);


private:
	void RepositioningUI();
	void EraseUI();
	int32 ScrollScreen(int iRows);
	void PrintLine(int32 row, string text);
	CONSOLE_SCREEN_BUFFER_INFO* GetScreenBufferInfo();

private:
	void MoveCursorToRow(int32 row) {printf("\x1b[%d;%df", row,0);}
	void MoveCursorToNextRow(){printf("\n");}
	void MoveCursorToPos(Point* pos) { printf("\x1b[%d;%df", pos->row, pos->col); }
	void EraseLine(int32 row) { MoveCursorToRow(row); printf("\x1b[K"); }
	void EraseToEnd(){ printf("\x1b[J"); }
	bool IsEnter(char c) { return c == 13; }
	bool IsChattableChar(char c) {return c >= 32 && c < 127;}
	bool IsDel(char c) { return c == 8; }
	bool IsESC(char c) { return c == 27; }
	bool IsChatSpaceFull() { return currentLine >= dividingLineRow; }

private:
	int32 chatSpaceHeight;
	int32 chatSpaceWidth;
	int32 screenHeight;
	int32 topPos;
	int32 dividingLineWidth;
	int32 dividingLineRow;
	char dividingLineChar = '=';

private:
	int32 currentLine;
	bool isPrompting = false;
	Point* promptPos;
	string promptString;
	HANDLE hStdout;

	ServerSessionRef serverSession;
};


# Description
This is a chatting program. with console UI.

# Environment
* Operating System : Windows11
* IDE : Visual Studio 2022
* Language: C++ 17

# How to Build
1. Download ServerCore([ ServerCore github](https://github.com/jidungg/GameServer)) library.
2. Put **ServerCore.lib** file in Libraries\ServerCore\Debug(Release) directory.
3. Download **protobuf** and get lib files.
4. Put  **libprotobufd.lib** in Libraries\Protobuf\Debug directory, And **libprotobuf.lib** in Libraries\Protobuf\Release directory.
5. Build each projects.

# How to Use
 - Server
	 - Just Start ChatServer.exe.
 - Client : Follow the following steps.
	 - Input Server IP : Enter server computer's IP address. We use 7777 port.
	 - Enter your name : Enter any nick name you want.
	 - Write Room Number : Enter any room number(or code) you want. If a room with the same name already exists, it will automatically enter the room. Otehrwise, create the room.
	 - Chatting room : Chat anyting.
	 - Escape the room : Press ESC to escape.


# Files
- ChatClient : Source files about ChatClient.
- ChatServer : Sources files about ChatServer.
- Libraries : Library files used.
- Protobuf : Protocol files.

# Used Libraries

 - ServerCore(https://github.com/jidungg/GameServer)
 - protobuf(c++)


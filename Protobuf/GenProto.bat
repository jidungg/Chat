pushd %~dp0 
protoc.exe -I=./ --cpp_out=./ ./Protocol.proto
protoc.exe -I=./ --cpp_out=./ ./Struct.proto
protoc.exe -I=./ --cpp_out=./ ./Enum.proto

GenPackets.exe --path=./Protocol.proto --output=ClientPacketHandler --recv=C_ --send=S_
GenPackets.exe --path=./Protocol.proto --output=ServerPacketHandler --recv=S_ --send=C_
IF ERRORLEVEL 1 PAUSE

XCOPY /Y Enum.pb.h "../ChatServer"
XCOPY /Y Struct.pb.h "../ChatServer"
XCOPY /Y Protocol.pb.h "../ChatServer"
XCOPY /Y Enum.pb.cc "../ChatServer"
XCOPY /Y Struct.pb.cc "../ChatServer"
XCOPY /Y Protocol.pb.cc "../ChatServer"
XCOPY /Y ClientPacketHandler.h "../ChatServer"

XCOPY /Y Enum.pb.h "../ChatClient"
XCOPY /Y Struct.pb.h "../ChatClient"
XCOPY /Y Protocol.pb.h "../ChatClient"
XCOPY /Y Enum.pb.cc "../ChatClient"
XCOPY /Y Struct.pb.cc "../ChatClient"
XCOPY /Y Protocol.pb.cc "../ChatClient"
XCOPY /Y ServerPacketHandler.h "../ChatClient"

DEL /S /Q /F *.pb.h
DEL /S /Q /F *.pb.cc
DEL /Q /F *.h

PAUSE
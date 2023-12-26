pushd %~dp0

protoc.exe -I=./ --cpp_out=./ ./Enum.proto
protoc.exe -I=./ --cpp_out=./ ./Struct.proto
protoc.exe -I=./ --cpp_out=./ ./Protocol.proto

IF ERRORLEVEL 1 PAUSE

XCOPY /Y Enum.pb.h "../../../Server"
XCOPY /Y Enum.pb.cc "../../../Server"
XCOPY /Y Struct.pb.h "../../../Server"
XCOPY /Y Struct.pb.cc "../../../Server"
XCOPY /Y Protocol.pb.h "../../../Server"
XCOPY /Y Protocol.pb.cc "../../../Server"

XCOPY /Y Enum.pb.h "../../../Client/GameCoding"
XCOPY /Y Enum.pb.cc "../../../Client/GameCoding"
XCOPY /Y Struct.pb.h "../../../Client/GameCoding"
XCOPY /Y Struct.pb.cc "../../../Client/GameCoding"
XCOPY /Y Protocol.pb.h "../../../Client/GameCoding"
XCOPY /Y Protocol.pb.cc "../../../Client/GameCoding"

DEL /Q /F *.pb.h
DEL /Q /F *.pb.cc
DEL /Q /F *.h

PAUSE
#pragma once

#include "CorePch.h"
#include "Types.h"
#include "Enums.h"
#include "Defines.h"
#include "Utils.h"
#include "Values.h"

// Protobuf 자동 생성
#include "Protocol.pb.h"
#include "Enum.pb.h"
#include "Struct.pb.h"

#include <windows.h>
#include <format>
#include <filesystem>
namespace fs = std::filesystem;

#pragma comment (lib, "msimg32.lib")

// IME
#include <Imm.h>
#pragma comment(lib, "Imm32.lib")

// 사운드
#include <mmsystem.h>
#include <dsound.h>
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dsound.lib")

#ifdef _DEBUG
#pragma comment(lib, "ServerCore\\Debug\\ServerCore.lib")
#pragma comment(lib, "Protobuf\\Debug\\libprotobufd.lib")
#else
#pragma comment(lib, "ServerCore\\Release\\ServerCore.lib")
#pragma comment(lib, "Protobuf\\Release\\libprotobuf.lib")
#endif

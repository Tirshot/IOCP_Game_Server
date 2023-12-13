#pragma once

#include "Types.h"
#include "Enums.h"
#include "Defines.h"
#include "Utils.h"
#include "Values.h"

#include <windows.h>
#include <vector>
#include <set>
#include <unordered_set>
#include <list>
#include <map>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <format>
#include <filesystem>
#include <functional>
#include <assert.h>
#include <queue>
namespace fs = std::filesystem;

#include <cmath>

#pragma comment (lib, "msimg32.lib")

using namespace std;

// 사운드
#include <mmsystem.h>
#include <dsound.h>
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dsound.lib")

#include "CorePch.h"

// Protobuf 자동 생성
#include "Protocol.pb.h"
#include "Enum.pb.h"
#include "Struct.pb.h"

#ifdef _DEBUG
#pragma comment(lib, "ServerCore\\Debug\\ServerCore.lib")
#pragma comment(lib, "Protobuf\\Debug\\libprotobufd.lib")
#else
#pragma comment(lib, "ServerCore\\Release\\ServerCore.lib")
#pragma comment(lib, "Protobuf\\Release\\libprotobuf.lib")
#endif

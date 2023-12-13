#pragma once
#include <windows.h>
#include <string>
using namespace std;

class Utils
{
	// Typo ÇÔ¼ö
public:

	static void DrawText(HDC hdc, Pos pos, const wstring& str);

	static void DrawRect(HDC hdc, Pos pos, int32 w, int32 h);

	static void DrawCircle(HDC hdc, Pos pos, int32 radius);

	static void DrawLine(HDC hdc, Pos from, Pos to);

};
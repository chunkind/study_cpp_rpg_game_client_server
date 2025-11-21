#include "pch.h"
#include "CoreUtil.h"
#include <string>
#include <windows.h>

std::mt19937 CoreUtil::s_generator = std::mt19937(std::random_device{}());

int32 CoreUtil::GetRandom(int32 min, int32 max)
{
	std::uniform_int_distribution<int32> distribution(min, max);
	return distribution(s_generator);
}

wstring CoreUtil::TrnWstr(const std::string& text)
{
    int sizeNeeded = MultiByteToWideChar(CP_UTF8, 0, text.c_str(), (int)text.size(), nullptr, 0);
    std::wstring wstr(sizeNeeded, 0);
    MultiByteToWideChar(CP_UTF8, 0, text.c_str(), (int)text.size(), &wstr[0], sizeNeeded);
    return wstr;
}

std::string CoreUtil::TrnStr(const std::wstring& wtext)
{
    int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, wtext.c_str(), (int)wtext.size(), nullptr, 0, nullptr, nullptr);
    std::string str(sizeNeeded, 0);
    WideCharToMultiByte(CP_UTF8, 0, wtext.c_str(), (int)wtext.size(), &str[0], sizeNeeded, nullptr, nullptr);
    return str;
}
#pragma once

class CoreUtil
{
public:
	// min과 max 사이의 랜덤한 정수를 반환 (min, max 포함)
	static int32 GetRandom(int32 min, int32 max);
	static wstring TrnWstr(const std::string& text);
	static std::string TrnStr(const std::wstring& wtext);

private:
	static std::mt19937 s_generator;
};

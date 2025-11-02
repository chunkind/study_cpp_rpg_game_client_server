#pragma once

class Res
{
public:
	Res();
	virtual ~Res();

	virtual void LoadFile(const wstring& path);
	virtual void SaveFile(const wstring& path);

};


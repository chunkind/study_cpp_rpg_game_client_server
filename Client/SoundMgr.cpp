#include "pch.h"
#include "SoundMgr.h"
#include "ResMgr.h"
#include "Sound.h"

SoundMgr::SoundMgr()
{

}

SoundMgr::~SoundMgr()
{
	if (_soundDevice) {
		_soundDevice->Release();
		_soundDevice = nullptr;
	}
		
}

void SoundMgr::Init(HWND hwnd)
{
	// 사운드 디바이스 생성
	if (FAILED(::DirectSoundCreate(NULL, &_soundDevice, NULL)))
	{
		::MessageBox(NULL, L"사운드디바이스생성실패", L"SYSTEM ERROR", MB_OK);
		return;
	}

	// 사운드 디바이스 협조레벨 설정
	if (FAILED(_soundDevice->SetCooperativeLevel(hwnd, DSSCL_PRIORITY)))
	{
		::MessageBox(NULL, L"사운드디바이스 협조레벨 설정", L"SYSTEM ERROR", MB_OK);
		return;
	}
}

void SoundMgr::Play(const wstring& key, bool loop /*= false*/)
{
	Sound* sound = GET(ResMgr)->GetSound(key);
	if (sound == nullptr)
		return;

	sound->Play(loop);
}

#include "pch.h"
#include "SceneMgr.h"
#include "DevScene.h"
#include "EditScene.h"
#include "MyPlayer.h"

void SceneMgr::Init()
{

}

void SceneMgr::Update()
{
	if (_scene)
		_scene->Update();
}

void SceneMgr::Render(HDC hdc)
{
	if (_scene)
		_scene->Render(hdc);
}

void SceneMgr::Clear()
{
	SAFE_DELETE(_scene);
}

void SceneMgr::ChangeScene(SceneType sceneType)
{
	if (_sceneType == sceneType)
		return;

	Scene* newScene = nullptr;

	switch (sceneType)
	{
	case SceneType::DevScene:
		newScene = new DevScene();
		break;
	case SceneType::EditScene:
		newScene = new EditScene();
		break;
	}

	SAFE_DELETE(_scene);

	_scene = newScene;
	_sceneType = sceneType;

	newScene->Init();
}

class DevScene* SceneMgr::GetDevScene()
{
	return dynamic_cast<DevScene*>(GetCurrentScene());
}

uint64 SceneMgr::GetMyPlayerId()
{
	return _myPlayer ? _myPlayer->info.objectid() : 0;
}

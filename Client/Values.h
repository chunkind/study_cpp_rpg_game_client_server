#pragma once
// 게임 좌표당 크기
const int32 GPixcelWidth = 32;
const int32 GPixcelHeight = 32;

// 맵 크기
const int32 GMapWidth = 100;
const int32 GMapHeight = 100;

// 게임 가로, 세로 크기값
const int32 GWinSizeX = 800;
const int32 GWinSizeY = 600;

const int32 GMinimapSizeX = 200;
const int32 GMinimapSizeY = 128;
const float PI = 3.1415926f;

// 위, 아래, 왼쪽, 오른쪽 보여줄 변수
const int32 DU = 0;
const int32 DD = 0;
const int32 DL = 0;
const int32 DR = 0;
const Vec2Int front[4] =
{
	{0, -1},
	{0, 1},
	{-1, 0},
	{1, 0},
};
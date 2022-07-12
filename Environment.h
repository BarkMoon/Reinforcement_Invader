#pragma once
#include <utility>
#include <tuple>

constexpr unsigned int Width = 5;
constexpr unsigned int EnemyHeight = 3;
constexpr unsigned int Sep = 2;
constexpr unsigned int Height = EnemyHeight + Sep + 1;
constexpr unsigned int ActionNum = 4;
constexpr unsigned int FrameCycle = 60;
constexpr unsigned int BulletStep = 15;
constexpr bool ClearRewardFixed = false;

using EnemyBool = unsigned int;		//右下から左上に数え、そこに敵がいるならそのビットを1にする。
using BatteryPlace = int;		// 0~Width-1
using BulletPlace = int;		// Width*Height: なし、0~Width*Height-1。左上から数える。
using NowFrame = unsigned int;		// 0~FrameCycle-1、敵の移動までを1サイクル。
using State = std::tuple<EnemyBool, BatteryPlace, BulletPlace, NowFrame>;
using Action = unsigned int;		// 0: 何もしない 1, 2: 左右移動 3: 弾が画面上に無いなら発射。あるなら何もしない。
using StateAction = std::pair<State, Action>;
using Value = double;
using Ratio = double;
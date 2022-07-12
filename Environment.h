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

using EnemyBool = unsigned int;		//�E�����獶��ɐ����A�����ɓG������Ȃ炻�̃r�b�g��1�ɂ���B
using BatteryPlace = int;		// 0~Width-1
using BulletPlace = int;		// Width*Height: �Ȃ��A0~Width*Height-1�B���ォ�琔����B
using NowFrame = unsigned int;		// 0~FrameCycle-1�A�G�̈ړ��܂ł�1�T�C�N���B
using State = std::tuple<EnemyBool, BatteryPlace, BulletPlace, NowFrame>;
using Action = unsigned int;		// 0: �������Ȃ� 1, 2: ���E�ړ� 3: �e����ʏ�ɖ����Ȃ甭�ˁB����Ȃ牽�����Ȃ��B
using StateAction = std::pair<State, Action>;
using Value = double;
using Ratio = double;
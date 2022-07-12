#include <string>
#include <sstream>
#include "DxLib.h"
#include "Environment.h"
#include "Functions.h"
#include "Reinforcement.h"

int Key[256];
int enemy_img, battery_img, bullet_img;
int frame = 0, score = 0;

extern int gameover_times;

int GetHitKeyStateAll_2(int GetHitKeyStateAll_InputKey[]){
	char GetHitKeyStateAll_Key[256];
	GetHitKeyStateAll(GetHitKeyStateAll_Key);
	for (int i = 0; i<256; i++){
		if (GetHitKeyStateAll_Key[i] == 1) GetHitKeyStateAll_InputKey[i]++;
		else                            GetHitKeyStateAll_InputKey[i] = 0;
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
	ChangeWindowMode(TRUE);//ウィンドウモード
	SetGraphMode(800, 600, 16);
	SetAlwaysRunFlag(TRUE);
	if (DxLib_Init() == -1 || SetDrawScreen(DX_SCREEN_BACK) != 0) return -1;//初期化と裏画面化


	std::string str = "", filename;
	int input_ret;
	std::stringstream ss;
	char input_buf[50];
	enemy_img = LoadGraph("IMAGE/Enemy.png");
	battery_img = LoadGraph("IMAGE/Battery.png");
	bullet_img = LoadGraph("IMAGE/Bullet.png");

	State state;
	const State init_state = std::make_tuple(32736, 0, Width * Height, 0);
	Agent agent;
	int rep;

	enum Mode {SELECT, PLAY, REP_INPUT, REINFORCEMENT, COMPUTE, SAVE, LOAD_INPUT, LOAD} mode = SELECT;

	while (ProcessMessage() == 0 && ClearDrawScreen() == 0 && GetHitKeyStateAll_2(Key) == 0){
		//↑ﾒｯｾｰｼﾞ処理       　  ↑画面をｸﾘｱ    　　　    ↑入力状態を保存

		switch (mode){
			case SELECT:
				DrawFormatString(100, 300, GetColor(255, 255, 255), "Space: Play, R: Reinforcement Learning, C: Compute, S: Save, L: Load");
				if (Key[KEY_INPUT_SPACE] == 1){
					frame = score = 0;
					state = init_state;
					mode = PLAY;
				}
				if (Key[KEY_INPUT_R] == 1){
					score = 0;
					state = init_state;
					ss.str("");
					mode = REP_INPUT;
				}
				if (Key[KEY_INPUT_C] == 1){
					frame = score = 0;
					state = init_state;
					mode = COMPUTE;
				}
				if (Key[KEY_INPUT_S] == 1){
					str = "now saving...";
					mode = SAVE;
				}
				if (Key[KEY_INPUT_L] == 1){
					ss.str("");
					mode = LOAD_INPUT;
				}
				DrawFormatString(300, 316, GetColor(255, 255, 255), "%s", str.c_str());
				break;
			case PLAY:
				if (PlayEpisode(state) == 0){
					mode = SELECT;
				}
				break;
			case REP_INPUT:
				DrawFormatString(100, 300, GetColor(255, 255, 255), "Repeat Num (Episodes): ");
				input_ret = KeyInputString(400, 300, 9, input_buf, TRUE);
				if (input_ret == 1){
					ss << input_buf << std::endl;
					ss >> rep;
					mode = REINFORCEMENT;
				}
				else{
					mode = SELECT;
				}
				break;
			case REINFORCEMENT:
				agent.OffPolicyEveryVisitWeighted(state, rep, 0.9, 0.1, true);
				--rep;
				if (rep == 0)
					mode = SELECT;
				break;
			case COMPUTE:
				if (ComputeEpisode(state, agent) == 0){
					mode = SELECT;
				}
				break;
			case SAVE:
				agent.SavePolicy();
				str = "finished saving!";
				mode = SELECT;
				break;
			case LOAD_INPUT:
				DrawFormatString(200, 300, GetColor(255, 255, 255), "Load File: ");
				input_ret = KeyInputString(400, 300, 49, input_buf, TRUE);
				if (input_ret == 1){
					ss << input_buf << std::endl;
					ss >> filename;
					str = "now loading...";
					mode = LOAD;
				}
				else{
					mode = SELECT;
				}
				DrawFormatString(300, 316, GetColor(255, 255, 255), "%s", str.c_str());
				break;
			case LOAD:
				agent.LoadPolicy(filename);
				str = "finished loading!";
				mode = SELECT;
				break;
		}
		DrawFormatString(0, 0, GetColor(255, 255, 255), "%d", score);
		DrawFormatString(0, 16, GetColor(255, 255, 255), "%d", gameover_times);
		ScreenFlip();
	}

	DxLib_End();
	return 0;
}
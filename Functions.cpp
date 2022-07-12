#include "DxLib.h"
#include "Functions.h"

extern int Key[256];
extern int enemy_img, battery_img, bullet_img;
extern int frame, score;

unsigned int blt_timer = 0;

int PlayEpisode(State& state){
	EnemyBool& enb = std::get<0>(state);
	BatteryPlace& btp = std::get<1>(state);
	BulletPlace& blp = std::get<2>(state);
	NowFrame& nf = std::get<3>(state);

	if (Key[KEY_INPUT_LEFT] == 1 && btp > 0)
		--btp;
	if (Key[KEY_INPUT_RIGHT] == 1 && btp < Width - 1)
		++btp;
	if (Key[KEY_INPUT_Z] == 1 && blp == Width * Height){
		blp = Width * (Height - 2) + btp;
		blt_timer = 0;
	}

	for (int i = 0; i < EnemyHeight; ++i){
		for (int j = 0; j < Width; ++j){
			if (enb & (1 << (Width*(EnemyHeight - 1 - i) + Width - 1 - j)))
				DrawRotaGraph(400 + 100 * (j - Width / 2), 150 + 50 * i, 5.0, 0.0, enemy_img, TRUE);
		}
	}
	DrawRotaGraph(400 + 100 * (btp - Width / 2), 400, 5.0, 0.0, battery_img, FALSE);
	if (blp != Width * Height){
		int i = blp / Width;
		int j = blp % Width;
		DrawRotaGraph(400 + 100 * (j - Width / 2), 150 + 50 * i, 5.0, 0.0, bullet_img, TRUE);
	}

	++frame;
	nf = frame % FrameCycle;
	if (frame % FrameCycle == 0){
		if (enb % 2 == 1){
			score -= 10000;
			return 0;
		}
		else
			enb >>= 1;
	}
	++blt_timer;
	if (blp != Width * Height && blt_timer % BulletStep == 0){
		blp -= Width;
		if (blp < 0)
			blp = Width * Height;
	}
	if (blp != Width * Height){
		int shift = Width * EnemyHeight - 1 - blp;
		if (shift >= 0){
			if (enb & (1 << shift)){
				enb -= (1 << shift);
				blp = Width * Height;
				score += 200;
			}
		}
	}
	if (enb == 0){
		if(ClearRewardFixed)
			score += 1000;
		else
			score += (1000 - frame) * 10;
		return 0;
	}
	return 1;
}

int ComputeEpisode(State& state, Agent& agent){
	EnemyBool& enb = std::get<0>(state);
	BatteryPlace& btp = std::get<1>(state);
	BulletPlace& blp = std::get<2>(state);
	NowFrame& nf = std::get<3>(state);

	Action a = agent.TargetPolicy(state);


	if (a == 1 && btp > 0)
		--btp;
	if (a == 2 && btp < Width - 1)
		++btp;
	if (a == 3 && blp == Width * Height){
		blp = Width * (Height - 2) + btp;
		blt_timer = 0;
	}

	for (int i = 0; i < EnemyHeight; ++i){
		for (int j = 0; j < Width; ++j){
			if (enb & (1 << (Width*(EnemyHeight - 1 - i) + Width - 1 - j)))
				DrawRotaGraph(400 + 100 * (j - Width / 2), 150 + 50 * i, 5.0, 0.0, enemy_img, TRUE);
		}
	}
	DrawRotaGraph(400 + 100 * (btp - Width / 2), 400, 5.0, 0.0, battery_img, FALSE);
	if (blp != Width * Height){
		int i = blp / Width;
		int j = blp % Width;
		DrawRotaGraph(400 + 100 * (j - Width / 2), 150 + 50 * i, 5.0, 0.0, bullet_img, TRUE);
	}

	++frame;
	nf = frame % FrameCycle;
	if (frame % FrameCycle == 0){
		if (enb % 2 == 1){
			score -= 10000;
			return 0;
		}
		else
			enb >>= 1;
	}
	++blt_timer;
	if (blp != Width * Height && blt_timer % BulletStep == 0){
		blp -= Width;
		if (blp < 0)
			blp = Width * Height;
	}
	if (blp != Width * Height){
		int shift = Width * EnemyHeight - 1 - blp;
		if (shift >= 0){
			if (enb & (1 << shift)){
				enb -= (1 << shift);
				blp = Width * Height;
				score += 200;
			}
		}
	}
	if (enb == 0){
		if (ClearRewardFixed)
			score += 1000;
		else
			score += (1000 - frame) * 10;
		return 0;
	}
	return 1;
}
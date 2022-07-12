#include "DxLib.h"
#include "Reinforcement.h"

extern int score;

int memo_rep, memo_frame, memo_score, memo_size_s, memo_size_a, memo_size_r;
int gameover_times = 0;
double gameover_rate = 0.0;

void Agent::InitAgent(){
	target_policy.clear();
	behavior_policy.clear();
	Q.clear();
	C.clear();
}

int Agent::OffPolicyEveryVisitWeighted(State& init_state, int rep, double Gamma, double epsilon, bool mu_improve){
	int frame, blt_timer;
	std::vector<State> S;
	std::vector<Action> A;
	std::vector<Value> R;
	std::random_device rnd;
	if(rep > 0){
		frame = 0;
		S.clear();
		S.shrink_to_fit();
		A.clear();
		A.shrink_to_fit();
		R.clear();
		R.shrink_to_fit();

		// ここからエピソード開始！

		frame = score = blt_timer = 0;
		State state = init_state;

		while (1){
			EnemyBool& enb = std::get<0>(state);
			BatteryPlace& btp = std::get<1>(state);
			BulletPlace& blp = std::get<2>(state);
			NowFrame& nf = std::get<3>(state);

			S.push_back(state);

			Action a;
			if (behavior_policy.find(state) != behavior_policy.end()){
				int random_d = rnd() % 100;
				if (random_d < 100 * (1 - epsilon))
					a = behavior_policy[state];
				else
					a = rnd() % ActionNum;
			}
			else
				a = rnd() % ActionNum;
			A.push_back(a);

			Value r = 0.0;

			if (a == 1 && btp > 0){
				//r -= 1.0;
				--btp;
			}
			if (a == 2 && btp < Width - 1){
				//r -= 1.0;
				++btp;
			}
			if (a == 3 && blp == Width * Height){
				blp = Width * (Height - 2) + btp;
				blt_timer = 0;
			}

			++frame;

			nf = frame % FrameCycle;

			bool end_flag = false;

			if (frame % FrameCycle == 0){
				if (enb % 2 == 1){
					score -= 10000;
					r -= 10000.0;
					end_flag = true;
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
						r += 200.0;
					}
				}
			}
			if (enb == 0){
				if (ClearRewardFixed){
					score += 1000;
					r += 1000.0;
				}
				else{
					score += (1000 - frame) * 10;
					r += (1000 - frame) * 10.0;
				}
				end_flag = true;
			}

			R.push_back(r);

			if (end_flag)
				break;
		}

		// ここまでエピソード

		memo_rep = rep;
		if (rep % 100 == 0){
			memo_frame = frame;
			memo_score = score;
			memo_size_s = S.size();
			memo_size_a = A.size();
			memo_size_r = R.size();
		}

		if (rep % 1000 == 0){
			gameover_rate = gameover_times / 1000.0;
			gameover_times = 0;
		}
		if (score < 0){
			++gameover_times;
		}

		Value G = 0.0;
		Ratio W = 1.0;
		for (int t = frame-1; t >= 0; --t){
			G = Gamma * G + R[t];
			
			if (C.find(make_pair(S[t], A[t])) != C.end())
				C[make_pair(S[t], A[t])] = C[make_pair(S[t], A[t])] + W;
			else
				C[make_pair(S[t], A[t])] = W;
			
			if (Q.find(make_pair(S[t], A[t])) != Q.end())
				Q[make_pair(S[t], A[t])] = Q[make_pair(S[t], A[t])] + W * (G - Q[make_pair(S[t], A[t])]) / C[make_pair(S[t], A[t])];
			else
				Q[make_pair(S[t], A[t])] = W * (G - Q[make_pair(S[t], A[t])]) / C[make_pair(S[t], A[t])];

			target_policy[S[t]] = 0;
			int max_q = Q[make_pair(S[t], 0)];
			for (int i = 1; i < ActionNum; ++i){
				if (Q[make_pair(S[t], i)] > max_q){
					max_q = Q[make_pair(S[t], i)];
					target_policy[S[t]] = i;
				}
			}

			double mu_as;

			if (behavior_policy.find(S[t]) != behavior_policy.end()){
				if (behavior_policy[S[t]] == A[t])
					mu_as = 1 - epsilon + epsilon / ActionNum;
				else
					mu_as = epsilon / ActionNum;
			}
			else
				mu_as = 1.0 / ActionNum;

			if (mu_improve){
				behavior_policy[S[t]] = target_policy[S[t]];
			}

			W = W / mu_as;
			if (W <= 1e-10){
				break;
			}
		}
	}
	DrawFormatString(400, 300, GetColor(255, 255, 255), "remain_rep = %d", memo_rep);
	DrawFormatString(400, 316, GetColor(255, 255, 255), "frame = %d, score = %d", memo_frame, memo_score);
	DrawFormatString(400, 332, GetColor(255, 255, 255), "S.size: %d, A.size: %d, R.size: %d", memo_size_s, memo_size_a, memo_size_r);
	DrawFormatString(400, 348, GetColor(255, 255, 255), "Game Over rate: %lf", gameover_rate);
	return 0;
}

int Agent::SavePolicy(){
	std::ofstream ofs("SAVED_POLICY/saved_policy.txt");
	if (!ofs){
		return -1;
	}
	
	for (auto& p : target_policy){
		ofs << std::get<0>(p.first) << " " << std::get<1>(p.first) << " " << std::get<2>(p.first) << " " << std::get<3>(p.first) << " " << p.second << std::endl;
	}
	return 0;
}

int Agent::LoadPolicy(std::string filename){
	if (filename == "")
		filename = "SAVED_POLICY/saved_policy.txt";
	else
		filename = "SAVED_POLICY/" + filename;
	std::ifstream ifs(filename.c_str());
	if (!ifs){
		return -1;
	}

	while (!ifs.eof()){
		EnemyBool enb;
		BatteryPlace btp;
		BulletPlace blp;
		NowFrame nf;
		Action a;
		ifs >> enb >> btp >> blp >> nf >> a;
		State state = std::make_tuple(enb, btp, blp, nf);
		target_policy[state] = a;
		behavior_policy[state] = a;
	}
	
	return 0;
}
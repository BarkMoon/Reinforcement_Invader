en / [ja](README.ja.md)

## What is Reinforcement Invader?
I made a simple environment for the Invader game, and your machine learns to solve the game using reinforcement learning!<br>
I used ε-greedy Off-Policy Monte Carlo method for this program.

## How to execute immediately
Execute **強化インベーダー.exe** in **Release** folder. This is for Windows.<br>
If you are using UNIX environment, please build a program with the source files.

## Rules of the Invader game
If a missile hits an invader, you get 200 points, and if you wipe out the invaders, you get (1000 - *elapsed frame*) × 10 points.<br>
Invaders move to the right grid in every 60 frames, and if they reach the end, they move to the 1 column of the next row.<br>
If they reach the 4th row, your game is over. -10000 points.

## How to play the Invader game for yourself
Press Space key in the title, and the Invader game begins immediately.<br>
Right or Left key:   Battery moves to right or left<br>
Z key:               Shoot a missile.

## How to conduct reinforcement learning
Press R key in the title, and input how many episodes your machine conduct reinforcement learning. (ex: 1000)<br>
Then, the learning begins. (in the Left-Up in the window, the score of the current episode is shown)<br>
The learned data is placed in the memory.

## How to see the result of the learning
Press C key in the title, and AI will play the Invader game!<br>
In the initial state, AI does not move at all. Please do reinforcement learning with R key or load the learned data with L key in advance!

## How to save the result of the learning
Press S key in the title. The learned data will be saved in **SAVED_POLICY** folder as **saved_policy.txt**.<br>
Please be careful for overwriting.

## How to load the result of the learning from a file
Press L key in the title, and input the file path under **SAVED_POLICY** folder.<br>
(ex: SAVED_POLICY/abc/policy.txt → abc/policy.txt)
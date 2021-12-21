#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <unordered_map>

using namespace std;

enum class EGameState
{
	PLAYER_1_TURN,
	PLAYER_2_TURN,
	GAME_OVER,
};

class Game
{
public:
	Game(int player1, int player2) 
	: Player1(player1), Player2(player2) {}

	bool Step();
	int GetLoserScore() const { return min(Score1,Score2); }
	int GetNumRolls() const { return NumRolls; }
private:
	int Roll() {
		++NumRolls;
		int retVal = Dice;
		Dice = (Dice % 100) + 1;
		return retVal;

	}

	void MovePlayer1(int numPlaces)
	{
		Player1 = (Player1-1 + numPlaces) % 10 + 1;
		Score1 += Player1;
		if (Score1 >= 1000)
		{
			GameState = EGameState::GAME_OVER;
		}
		else {
			GameState = EGameState::PLAYER_2_TURN;
		}
	}

	void MovePlayer2(int numPlaces)
	{
		Player2 = (Player2 - 1 + numPlaces) % 10 + 1;
		Score2 += Player2;
		if (Score2 >= 1000)
		{
			GameState = EGameState::GAME_OVER;
		}
		else {
			GameState = EGameState::PLAYER_1_TURN;
		}
	}
	int Score1 = 0;
	int Player1;
	int Score2 = 0;
	int Player2;

	int Dice = 1;
	int NumRolls = 0;
	EGameState GameState = EGameState::PLAYER_1_TURN;
};



bool Game::Step()
{
	if (GameState == EGameState::GAME_OVER) {
		return false;
	}
	int NumMoves = Roll() + Roll() + Roll();
	switch (GameState)
	{
		case EGameState::PLAYER_1_TURN:
			MovePlayer1(NumMoves);
			break;
		case EGameState::PLAYER_2_TURN:
			MovePlayer2(NumMoves);
			break;
	}
	return GameState != EGameState::GAME_OVER;
}

int64_t partA(Game& game)
{
	while(game.Step()) {}
	int64_t retVal = game.GetLoserScore();
	retVal *= game.GetNumRolls();
	return retVal;
}

enum class EGameState2
{
	PLAYER_1_TURN_ROLL1,
	PLAYER_1_TURN_ROLL2,
	PLAYER_1_TURN_ROLL3,
	PLAYER_2_TURN_ROLL1,
	PLAYER_2_TURN_ROLL2,
	PLAYER_2_TURN_ROLL3,
};

struct EntryKey
{
	EntryKey(int loc1, int score1, int loc2, int score2, EGameState2 turn,int roll1, int roll2)
		: Loc1(loc1), Score1(score1), Loc2(loc2), Score2(score2), Turn(turn), Roll1(roll1), Roll2(roll2) {}

	int Loc1;
	int Score1;
	int Loc2;
	int Score2;
	EGameState2 Turn;
	int Roll1;
	int Roll2;
	
	bool operator==(const EntryKey& rhs) const {
		return Loc1 == rhs.Loc1 &&
		Score1 == rhs.Score1 &&
		Loc2 == rhs.Loc2 &&
		Score2 == rhs.Score2 &&
		Turn == rhs.Turn &&
		Roll1 == rhs.Roll1 &&
		Roll2 == rhs.Roll2;
	}
};
namespace std {

	template <>
	struct hash<EntryKey>
	{
		std::size_t operator()(const EntryKey& k) const
		{
			return (hash<int>()(k.Loc1)
				^ ((hash<int>()(k.Score1) << 1) >> 1)
				^ ((hash<int>()(k.Loc2) << 2) >> 2)
				^ ((hash<int>()(k.Score2) << 3) >> 3)
				^ ((hash<int>()((int)k.Turn) << 4) >> 4)
				^ ((hash<int>()((int)k.Roll1) << 5) >> 5)
				^ ((hash<int>()((int)k.Roll2) << 6) >> 6));
		}
	};
}

struct WinLoss
{
	WinLoss& operator+=(const WinLoss& rhs) {
		Wins += rhs.Wins;
		Losses += rhs.Losses;
		return *this;
	}
	int64_t Wins = 0;
	int64_t Losses = 0;
};

unordered_map<EntryKey,WinLoss> Memo;

WinLoss dirac(int loc1, int score1, int loc2, int score2, EGameState2 turn, int roll1, int roll2)
{
	EntryKey entryKey{loc1,score1,loc2,score2,turn, roll1, roll2};
	auto iter = Memo.find(entryKey);
	if (iter != Memo.end()) {
		return iter->second;
	}
	WinLoss record;
	if (turn == EGameState2::PLAYER_1_TURN_ROLL1)
	{
		for (int r1 = 1; r1 <= 3; ++r1) {
			record += dirac(loc1, score1, loc2, score2, EGameState2::PLAYER_1_TURN_ROLL2, r1, 0);
		}
	}
	else if (turn == EGameState2::PLAYER_1_TURN_ROLL2)
	{
		for (int r2 = 1; r2 <= 3; ++r2) {
			record += dirac(loc1, score1, loc2, score2, EGameState2::PLAYER_1_TURN_ROLL3, roll1, r2);
		}
	}
	else if (turn == EGameState2::PLAYER_1_TURN_ROLL3)
	{
		for (int r3 = 1; r3 <= 3; ++r3) {
			int sum = roll1 + roll2 + r3;
			int newLoc1 = (loc1 - 1 + sum) % 10 + 1;
			int newScore1 = score1 + newLoc1;
			if (newScore1 >= 21)
			{
				record.Wins += 1;
			}
			else {
				record += dirac(newLoc1, newScore1, loc2, score2, EGameState2::PLAYER_2_TURN_ROLL1, 0, 0);
			}
		}
	}
	else if (turn == EGameState2::PLAYER_2_TURN_ROLL1)
	{
		for (int r1 = 1; r1 <= 3; ++r1) {
			record += dirac(loc1, score1, loc2, score2, EGameState2::PLAYER_2_TURN_ROLL2, r1, 0);
		}
	}
	else if (turn == EGameState2::PLAYER_2_TURN_ROLL2)
	{
		for (int r2 = 1; r2 <= 3; ++r2) {
			record += dirac(loc1, score1, loc2, score2, EGameState2::PLAYER_2_TURN_ROLL3, roll1, r2);
		}
	}
	else {
		assert(turn == EGameState2::PLAYER_2_TURN_ROLL3);
		for (int r3 = 1; r3 <= 3; ++r3) {
			int sum = roll1 + roll2 + r3;
			int newLoc2 = (loc2 - 1 + sum) % 10 + 1;
			int newScore2 = score2 + newLoc2;
			if (newScore2 >= 21)
			{
				record.Losses += 1;
			}
			else {
				record += dirac(loc1, score1, newLoc2, newScore2, EGameState2::PLAYER_1_TURN_ROLL1, 0, 0);
			}
		}
	}
	Memo.insert(make_pair(entryKey,record));
	return record;
}

int64_t partB(int p1, int p2)
{
	WinLoss record = dirac(p1,0,p2,0,EGameState2::PLAYER_1_TURN_ROLL1,0,0);
	return max(record.Wins,record.Losses);
}

int main()
{
	Game game(10, 7);

	cout << "Part A: " << partA(game) << '\n';
	cout << "Part B: " << partB(10,7) << '\n';
	return 0;
}
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <limits>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

// Stores tournament statistics for one player.
struct PlayerStats
{
	int gamesPlayed = 0;
	int wins = 0;
	int chipsWon = 0;
};

// Stores the result of one completed LCR game.
struct GameResult
{
	string winnerName;
	int winningChips = 0;
	int centerPot = 0;
};

// Stores information for one LCR player.
class Player
{
private:
	string m_name;
	int m_chips;

public:
	Player(const string& name)
	{
		m_name = name;
		m_chips = 3;
	}

	const string& getName() const
	{
		return m_name;
	}

	int getChips() const
	{
		return m_chips;
	}

	void addChip()
	{
		m_chips++;
	}

	bool removeChip()
	{
		if (m_chips > 0)
		{
			m_chips--;
			return true;
		}

		return false;
	}
};

// Generates one result from an LCR die.
class Dice
{
public:
	char rollDie() const
	{
		int rollResult = rand() % 6 + 1;

		if (rollResult == 1)
		{
			return 'L';
		}
		else if (rollResult == 2)
		{
			return 'R';
		}
		else if (rollResult == 3)
		{
			return 'C';
		}

		return '*';
	}
};

// Reads and displays the LCR rules text file.
bool displayRules()
{
	const string fileName = "LCRRules.txt";
	string ruleLine;

	ifstream rulesFile(fileName);

	if (!rulesFile)
	{
		cout << "Unable to open the rules file: "
			<< fileName
			<< endl;

		return false;
	}

	cout << "\nLEFT CENTER RIGHT RULES\n";
	cout << "=======================\n\n";

	while (getline(rulesFile, ruleLine))
	{
		cout << ruleLine << endl;
	}

	rulesFile.close();

	return true;
}

// Displays each player's current number of chips.
void displayPlayerChips(const vector<Player>& players, int centerPot)
{
	cout << "\nCurrent Chip Counts\n";
	cout << "-------------------\n";

	for (int playerIndex = 0;
		playerIndex < static_cast<int>(players.size());
		playerIndex++)
	{
		cout << players[playerIndex].getName()
			<< ": "
			<< players[playerIndex].getChips()
			<< " chips"
			<< endl;
	}

	cout << "Center pot: "
		<< centerPot
		<< " chips\n";
}

// Counts how many players currently have at least one chip.
int countPlayersWithChips(const vector<Player>& players)
{
	int playersWithChips = 0;

	for (int playerIndex = 0;
		playerIndex < static_cast<int>(players.size());
		playerIndex++)
	{
		if (players[playerIndex].getChips() > 0)
		{
			playersWithChips++;
		}
	}

	return playersWithChips;
}

// Finds and returns the index of the player who still has chips.
int findWinnerIndex(const vector<Player>& players)
{
	for (int playerIndex = 0;
		playerIndex < static_cast<int>(players.size());
		playerIndex++)
	{
		if (players[playerIndex].getChips() > 0)
		{
			return playerIndex;
		}
	}

	return -1;
}

// Plays one complete game of LCR and returns the game's result.
GameResult playGame(const vector<string>& playerNames)
{
	vector<Player> players;

	// Create a new Player object for each participant.
	for (const string& playerName : playerNames)
	{
		players.push_back(Player(playerName));
	}

	Dice gameDice;
	int centerPot = 0;
	int currentPlayerIndex = 0;
	int numberOfPlayers = static_cast<int>(players.size());

	cout << "\nThe game is starting. "
		<< "Every player begins with three chips.\n";

	displayPlayerChips(players, centerPot);

	// Continue gameplay until only one player still has chips.
	while (countPlayersWithChips(players) > 1)
	{
		Player& currentPlayer = players[currentPlayerIndex];

		cout << "\n----------------------------------------\n";
		cout << currentPlayer.getName()
			<< "'s turn\n";

		if (currentPlayer.getChips() == 0)
		{
			cout << currentPlayer.getName()
				<< " has no chips and skips this turn.\n";
		}
		else
		{
			int numberOfDice;

			if (currentPlayer.getChips() >= 3)
			{
				numberOfDice = 3;
			}
			else
			{
				numberOfDice = currentPlayer.getChips();
			}

			char diceResults[3] = { '*', '*', '*' };

			cout << currentPlayer.getName()
				<< " rolls "
				<< numberOfDice
				<< " die/dice.\n";

			// Roll and store each result in the dice array.
			for (int diceIndex = 0;
				diceIndex < numberOfDice;
				diceIndex++)
			{
				diceResults[diceIndex] = gameDice.rollDie();

				cout << "Die "
					<< diceIndex + 1
					<< ": "
					<< diceResults[diceIndex]
					<< endl;
			}

			// Apply chip movement for each die result.
			for (int diceIndex = 0;
				diceIndex < numberOfDice;
				diceIndex++)
			{
				char result = diceResults[diceIndex];

				if (result == 'L' && currentPlayer.removeChip())
				{
					int leftPlayerIndex =
						(currentPlayerIndex - 1 + numberOfPlayers)
						% numberOfPlayers;

					players[leftPlayerIndex].addChip();

					cout << currentPlayer.getName()
						<< " passes one chip left to "
						<< players[leftPlayerIndex].getName()
						<< ".\n";
				}
				else if (result == 'R' && currentPlayer.removeChip())
				{
					int rightPlayerIndex =
						(currentPlayerIndex + 1)
						% numberOfPlayers;

					players[rightPlayerIndex].addChip();

					cout << currentPlayer.getName()
						<< " passes one chip right to "
						<< players[rightPlayerIndex].getName()
						<< ".\n";
				}
				else if (result == 'C' && currentPlayer.removeChip())
				{
					centerPot++;

					cout << currentPlayer.getName()
						<< " places one chip in the center pot.\n";
				}
				else if (result == '*')
				{
					cout << currentPlayer.getName()
						<< " keeps their chip.\n";
				}
			}

			displayPlayerChips(players, centerPot);
		}

		currentPlayerIndex =
			(currentPlayerIndex + 1) % numberOfPlayers;
	}

	int winnerIndex = findWinnerIndex(players);

	GameResult result;
	result.winnerName = players[winnerIndex].getName();
	result.winningChips = players[winnerIndex].getChips();
	result.centerPot = centerPot;

	cout << "\n========================================\n";
	cout << "Game Over!\n";

	cout << result.winnerName
		<< " wins LCR with "
		<< result.winningChips
		<< " chip(s) remaining.\n";

	cout << "Chips in the center pot: "
		<< result.centerPot
		<< endl;

	return result;
}

// Updates tournament statistics after one completed game.
void updateTournamentStats(
	const vector<string>& playerNames,
	const GameResult& gameResult,
	unordered_map<string, PlayerStats>& tournamentStats)
{
	for (const string& playerName : playerNames)
	{
		tournamentStats[playerName].gamesPlayed++;
	}

	tournamentStats[gameResult.winnerName].wins++;
	tournamentStats[gameResult.winnerName].chipsWon +=
		gameResult.winningChips;
}

// Displays tournament standing sorted by wins and chips won.
void displayLeaderboard(
	const unordered_map<string, PlayerStats>& tournamentStats)
{
	vector<pair<string, PlayerStats>> leaderboard;

	// Copy map entries into a vector so they can be sorted.
	for (const auto& playerEntry : tournamentStats)
	{
		leaderboard.push_back(playerEntry);
	}

	// Sort wins first, then by chips won, then alphabetically.
	sort(
		leaderboard.begin(),
		leaderboard.end(),
		[](const pair<string, PlayerStats>& firstPlayer,
			const pair<string, PlayerStats>& secondPlayer)
		{
			if (firstPlayer.second.wins != secondPlayer.second.wins)
			{
				return firstPlayer.second.wins >
					secondPlayer.second.wins;
			}

			if (firstPlayer.second.chipsWon !=
				secondPlayer.second.chipsWon)
			{
				return firstPlayer.second.chipsWon >
					secondPlayer.second.chipsWon;
			}

			return firstPlayer.first < secondPlayer.first;
		});

	cout << "\nTournament Leaderboard\n";
	cout << "======================\n";

	for (int playerIndex = 0;
		playerIndex < static_cast<int>(leaderboard.size());
		playerIndex++)
	{
		const string& playerName =
			leaderboard[playerIndex].first;

		const PlayerStats& stats =
			leaderboard[playerIndex].second;

		cout << playerIndex + 1
			<< ". "
			<< playerName
			<< endl;

		cout << "  Games played: "
			<< stats.gamesPlayed
			<< endl;

		cout << "  Wins: "
			<< stats.wins
			<< endl;

		cout << "  Chips won: "
			<< stats.chipsWon
			<< endl
			<< endl;
	}
}

int main()
{
	srand(static_cast<unsigned int>(time(0)));

	if (!displayRules())
	{
		return 1;
	}

	int numberOfPlayers;
	string numberInput;
	bool validNumber = false;

	while (!validNumber)
	{
		cout << "\nEnter the number of players: ";
		getline(cin, numberInput);

		stringstream inputStream(numberInput);
		char extraCharacter;

		if (inputStream >> numberOfPlayers &&
			numberOfPlayers >= 3 &&
			!(inputStream >> extraCharacter))
		{
			validNumber = true;
		}
		else
		{
			cout << "Invalid input. Enter a whole number of at least 3.\n";
		}
	}

	vector<string> playerNames;
	unordered_map<string, PlayerStats> tournamentStats;

	char playAgain = 'Y';

	// Collect player names and initialize their statistics.
	for (int playerIndex = 0;
		playerIndex < numberOfPlayers;
		playerIndex++)
	{
		string playerName;

		cout << "Enter the name of player "
			<< playerIndex + 1
			<< ": ";

		getline(cin, playerName);

		playerNames.push_back(playerName);
		tournamentStats[playerName] = PlayerStats();
	}

	while (toupper(playAgain) == 'Y')
	{
		GameResult gameResult = playGame(playerNames);

		updateTournamentStats(
			playerNames,
			gameResult,
			tournamentStats);

		cout << "\nPlay another game? (Y/N): ";
		cin >> playAgain;
		cout << endl;
	}

	displayLeaderboard(tournamentStats);

	return 0;
}
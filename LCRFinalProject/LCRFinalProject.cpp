/*
	Name: August Moews
	Course: IT 312
	Date: June 20, 2026
	Assignment: Final Project - Left Center Right Dice Game

	Code Description:
	This porgram creates a command-line version of the Left Center Right (LCR)
	dice game. The program reads and displays the game rules from a text file, 
	asks for the player names, gives each player three chips, and continues through
	player turns until only one player has chips remaining.

	Special Features:
	This program uses a Player class to track each player's name and chips and
	a Dice class to generate L, R, C, or neutral dice results. A vector stores
	the players, and an array stores the results of each turn's dice rolls.
*/

#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

using namespace std;

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

// Generates one result from LCR die.
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
		cout << "Unable to open the rules file: " << fileName << endl;
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

// Displays player's current number of chips.
void displayPlayerChips(const vector<Player>& players, int centerPot)
{
	cout << "\nCurrent Chips Counts\n";
	cout << "------------------\n";

	for (int playerIndex = 0; playerIndex < players.size(); playerIndex++)
	{
		cout << players[playerIndex].getName()
			<< ": "
			<< players[playerIndex].getChips()
			<< " chips" << endl;
	}

	cout << "Center pot: " << centerPot << " chips\n";
}

// Counts how many players currently have at least one chip.
int countPlayersWithChips(const vector<Player>& players)
{
	int playersWithChips = 0;

	for (int playerIndex = 0; playerIndex < players.size(); playerIndex++)
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
	for (int playerIndex = 0; playerIndex < players.size(); playerIndex++)
	{
		if (players[playerIndex].getChips() > 0)
		{
			return playerIndex;
		}
	}

	return -1;
}

int main()
{
	srand(static_cast<unsigned int>(time(0)));

	if (!displayRules())
	{
		return 1;
	}

	int numberOfPlayers;

	cout << "\nEnter the number of players: ";
	cin >> numberOfPlayers;

	while (numberOfPlayers < 3)
	{
		cout << "LCR requires at least three players.\n";
		cout << "Enter the number of players: ";
		cin >> numberOfPlayers;
	}

	cin.ignore(numeric_limits<streamsize>::max(), '\n');

	vector<Player> players;

	// Collect player names and create a Player object for each participant.
	for (int playerIndex = 0; playerIndex < numberOfPlayers; playerIndex++)
	{
		string playerName;

		cout << "Enter the name of player "
			<< playerIndex + 1
			<< ": ";

		getline(cin, playerName);

		players.push_back(Player(playerName));
	}

	Dice gameDice;
	int centerPot = 0;
	int currentPlayerIndex = 0;

	cout << "\nThe game is starting. Every player begins with three chips.\n";
	displayPlayerChips(players, centerPot);

	// Continue gameplay until only one player still has chips.
	while (countPlayersWithChips(players) > 1)
	{
		Player& currentPlayer = players[currentPlayerIndex];

		cout << "\n----------------------------------------\n";
		cout << currentPlayer.getName() << "'s turn\n";

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

			// Roll and store each results in the dice array
			for (int diceIndex = 0; diceIndex < numberOfDice; diceIndex++)
			{
				diceResults[diceIndex] = gameDice.rollDie();

				cout << "Die "
					<< diceIndex + 1
					<< ": "
					<< diceResults[diceIndex]
					<< endl;
			}

			// Apply chip movement for each die result.
			for (int diceIndex = 0; diceIndex < numberOfDice; diceIndex++)
			{
				char result = diceResults[diceIndex];

				if (result == 'L' && currentPlayer.removeChip())
				{
					int leftPlayerIndex =
						(currentPlayerIndex - 1 + numberOfPlayers) % numberOfPlayers;

					players[leftPlayerIndex].addChip();

					cout << currentPlayer.getName()
						<< " passes one chip left to "
						<< players[leftPlayerIndex].getName()
						<< ".\n";
				}
				else if (result == 'R' && currentPlayer.removeChip())
				{
					int rightPlayerIndex =
						(currentPlayerIndex + 1) % numberOfPlayers;

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

		currentPlayerIndex = (currentPlayerIndex + 1) % numberOfPlayers;
	}

	int winnerIndex = findWinnerIndex(players);

	cout << "\n========================================\n";
	cout << "Game Over!\n";
	cout << players[winnerIndex].getName()
		<< " wins LCR with "
		<< players[winnerIndex].getChips()
		<< " chip(s) remaining.\n";

	cout << "Chips in the center pot: "
		<< centerPot
		<< endl;

	return 0;
}
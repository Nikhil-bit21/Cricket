#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
#include <limits>

using namespace std;

struct Player {
    string name;
    int runs;
    int balls;
    int status;
    int fours;
    int sixes;
    float strikeRate;
};

struct Team {
    string name;
    vector<Player> players;
};

bool doToss() {
    int tossResult = rand() % 2;
    return tossResult == 0;
}

bool whoWinsToss() {
    cout << "Enter your choice for toss (0 for Heads, 1 for Tails): ";
    int userChoice;
    cin >> userChoice;

    bool tossResult = doToss();

    if (userChoice == tossResult) {
        cout << "Congratulations! You won the toss." << endl;
        return true;
    } else {
        cout << "Sorry! You lost the toss." << endl;
        return false;
    }
}

void initializePlayers(Team& team, int numPlayers, bool isUserTeam) {
    for (int i = 0; i < numPlayers; ++i) {
        string playerName;
        if (isUserTeam) {
            cout << "Enter name of player " << i + 1 << " for team " << team.name << ": ";
            cin >> playerName;
        } else {
            playerName = "Player " + to_string(i + 1);
        }
        Player newPlayer = {playerName, 0, 0, 1, 0, 0, 0.0f};
        team.players.push_back(newPlayer);
    }
}

int getNumericInput(const string& prompt, int minVal, int maxVal) {
    int num;
    while (true) {
        cout << prompt;
        if (!(cin >> num) || num < minVal || num > maxVal) {
            cout << "Invalid input! Please enter a number between " << minVal << " and " << maxVal << ".\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        } else {
            break;
        }
    }
    return num;
}

bool chooseBatOrBall(bool userWinsToss) {
    int compChoice = rand() % 2;

    if (userWinsToss) {
        cout << "Choose your option: \n1. Bat\n2. Bowl\nEnter your choice (1/2): ";
        int userChoice;
        cin >> userChoice;

        if (userChoice == 1) {
            cout << "You chose to bat first." << endl;
            return true;
        } else {
            cout << "You chose to bowl first." << endl;
            return false;
        }
    } else {
        if (compChoice == 0) {
            cout << "Computer chose to bat first." << endl;
            return false;
        } else {
            cout << "Computer chose to bowl first." << endl;
            return true;
        }
    }
}

int chooseNumberOfOvers() {
    int numOvers;
    while (true) {
        cout << "Choose the number of overs (2/5/10): ";
        numOvers = getNumericInput("", 2, 10);
        if (numOvers == 2 || numOvers == 5 || numOvers == 10) {
            break;
        } else {
            cout << "Invalid option! Please try again." << endl;
        }
    }
    return numOvers;
}

void showScorecard(const Team& team) {
    cout << "----------------- Scorecard -----------------" << endl;
    cout << "Player\t\tRuns\tBalls\tFours\tSixes\tStrike Rate\tStatus" << endl;
    cout << "---------------------------------------------" << endl;
    for (const auto& player : team.players) {
        cout << setw(10) << left << player.name << "\t"
             << setw(6) << right << player.runs << "\t"
             << setw(5) << right << player.balls << "\t"
             << setw(5) << right << player.fours << "\t"
             << setw(5) << right << player.sixes << "\t";
        if (player.balls == 0) {
            cout << setw(11) << right << "0.00" << "\t";
        } else {
            cout << setw(11) << right << fixed << setprecision(2) << player.strikeRate << "\t";
        }
        if (player.status == -1) {
            cout << "Out";
        } else if (player.status == 0) {
            cout << "Not out";
        } else {
            cout << "Not played yet (DNP)";
        }
        cout << endl;
    }
}

void simulateBatting(Team& battingTeam, int numOvers) {
    cout << battingTeam.name << "'s team is batting." << endl;

    int totalRuns = 0;
    int totalBalls = 0;
    int overs = 0;
    int balls = 0;
    int currentBatsmanIndex = 0; // Index of the current batsman in the batting team

    // Batting logic
    while (overs < numOvers && currentBatsmanIndex < battingTeam.players.size()) {
        Player& currentBatsman = battingTeam.players[currentBatsmanIndex];

        string hit;
        cout << "Press 'hit' to play a shot: ";
        cin >> hit;

        if (hit == "hit") {
            int runs = rand() % 7; // Generate a random number between 0 and 6
            if (runs == 0) {
                cout << currentBatsman.name << " is out!" << endl;
                currentBatsman.status = -1; // Mark player as out
                currentBatsman.balls++; // Increment balls faced even when out
                currentBatsmanIndex++; // Move to the next batsman
            } else {
                cout << currentBatsman.name << " scores " << runs << " runs." << endl;
                currentBatsman.runs += runs; // Add runs to player's total runs
                currentBatsman.balls++; // Increment balls faced
                totalRuns += runs; // Add runs to total score
                totalBalls++; // Increment total balls faced

                if (runs == 4) {
                    currentBatsman.fours++; // Increment number of fours
                } else if (runs == 6) {
                    currentBatsman.sixes++; // Increment number of sixes
                }
                currentBatsman.status = 0; // Mark player as not out
            }
            balls++;

            if (balls == 6) {
                balls = 0;
                overs++;
                cout << "End of over " << overs << endl;
                cout << "Total score: " << totalRuns << " runs for " << overs << " overs." << endl;

                // Show the scorecard for the batting team
                showScorecard(battingTeam);
            }
        } else {
            cout << "Invalid input! Press 'hit' to play a shot." << endl;
        }

        // Update strike rate
        if (currentBatsman.balls > 0) {
            currentBatsman.strikeRate = (static_cast<float>(currentBatsman.runs) / currentBatsman.balls) * 100;
        }
    }

    // Display the batting team's final scorecard
    cout << "Batting innings completed for team " << battingTeam.name << ". Final scorecard:" << endl;
    showScorecard(battingTeam);
}

void simulateBowling(Team& battingTeam, int numOvers) {
    cout << battingTeam.name << "'s team is batting." << endl;

    int totalRuns = 0;
    int totalBalls = 0;
    int overs = 0;
    int balls = 0;
    int currentBatsmanIndex = 0; // Index of the current batsman in the batting team

    // Batting logic
    while (overs < numOvers && currentBatsmanIndex < battingTeam.players.size()) {
        Player& currentBatsman = battingTeam.players[currentBatsmanIndex];

        string hit;
        cout << "Press 'bowl' to play a shot: ";
        cin >> hit;

        if (hit == "bowl") {
            int runs = rand() % 7; // Generate a random number between 0 and 6
            if (runs == 0) {
                cout << currentBatsman.name << " is out!" << endl;
                currentBatsman.status = -1; // Mark player as out
                currentBatsman.balls++; // Increment balls faced even when out
                currentBatsmanIndex++; // Move to the next batsman
            } else {
                cout << currentBatsman.name << " scores " << runs << " runs." << endl;
                currentBatsman.runs += runs; // Add runs to player's total runs
                currentBatsman.balls++; // Increment balls faced
                totalRuns += runs; // Add runs to total score
                totalBalls++; // Increment total balls faced

                if (runs == 4) {
                    currentBatsman.fours++; // Increment number of fours
                } else if (runs == 6) {
                    currentBatsman.sixes++; // Increment number of sixes
                }
                currentBatsman.status = 0; // Mark player as not out
            }
            balls++;

            if (balls == 6) {
                balls = 0;
                overs++;
                cout << "End of over " << overs << endl;
                cout << "Total score: " << totalRuns << " runs for " << overs << " overs." << endl;

                // Show the scorecard for the batting team
                showScorecard(battingTeam);
            }
        } else {
            cout << "Invalid input! Press 'hit' to play a shot." << endl;
        }

        // Update strike rate
        if (currentBatsman.balls > 0) {
            currentBatsman.strikeRate = (static_cast<float>(currentBatsman.runs) / currentBatsman.balls) * 100;
        }
    }

    // Display the batting team's final scorecard
    cout << "Batting innings completed for team " << battingTeam.name << ". Final scorecard:" << endl;
    showScorecard(battingTeam);
}

int main() {
    srand(time(0));

    Team userTeam;
    cout << "Enter the name of your team: ";
    cin >> userTeam.name;
    int numUserPlayers = getNumericInput("Enter the number of players for your team (max 5): ", 1, 5);
    initializePlayers(userTeam, numUserPlayers, true);

    Team computerTeam;
    computerTeam.name = "Computer";
    int numComputerPlayers = numUserPlayers;
    initializePlayers(computerTeam, numComputerPlayers, false);

    bool userWinsToss = whoWinsToss();
    bool userBatsFirst = chooseBatOrBall(userWinsToss);

    int numOvers = chooseNumberOfOvers();
    cout << "You chose to play " << numOvers << " overs match." << endl;

    if (userBatsFirst) {
        simulateBatting(userTeam, numOvers);
        simulateBowling(computerTeam, numOvers);
    } else {
        simulateBowling(computerTeam, numOvers);
        simulateBatting(userTeam, numOvers);
    }

    return 0;
}

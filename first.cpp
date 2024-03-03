#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
#include <limits> // For numeric_limits

using namespace std;

// Define a struct to hold player information
struct Player {
    string name;
    int runs;
    int balls;
    int status; // -1 for out, 0 for not out, 1 for not played yet (DNP)
    int fours;
    int sixes;
    float strikeRate; // Strike rate of the player
};

// Define a struct to hold team information
struct Team {
    string name;
    vector<Player> players;
};

// Function to perform toss
bool doToss() {
    // Randomly generate 0 or 1 for heads or tails
    int tossResult = rand() % 2;
    return tossResult == 0; // 0 for heads, 1 for tails
}

// Function to determine who wins the toss
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

// Function to initialize players for a team
void initializePlayers(Team& team, int numPlayers, bool isUserTeam) {
    for (int i = 0; i < numPlayers; ++i) {
        string playerName;
        if (isUserTeam) {
            cout << "Enter name of player " << i + 1 << " for team " << team.name << ": ";
            cin >> playerName;
        } else {
            playerName = "Player " + to_string(i + 1);
        }
        Player newPlayer = {playerName, 0, 0, 1, 0, 0, 0.0f}; // Initialize player with DNP status and strike rate 0.0
        team.players.push_back(newPlayer);
    }
}

// Function to validate and get numeric input within a range
int getNumericInput(const string& prompt, int minVal, int maxVal) {
    int num;
    while (true) {
        cout << prompt;
        if (!(cin >> num) || num < minVal || num > maxVal) {
            cout << "Invalid input! Please enter a number between " << minVal << " and " << maxVal << ".\n";
            cin.clear(); // Clear error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
        } else {
            break;
        }
    }
    return num;
}

// Function to choose batting or bowling based on toss result
void chooseBatOrBall(bool userWinsToss) {
    int compChoice = rand() % 2; // Randomly choose 0 for batting, 1 for bowling

    if (userWinsToss) {
        cout << "Choose your option: \n1. Bat\n2. Bowl\nEnter your choice (1/2): ";
        int userChoice;
        cin >> userChoice;

        if (userChoice == 1) {
            cout << "You chose to bat first." << endl;
            // Simulate batting innings
            cout << "Your batting innings starts..." << endl;
            // You can add batting logic here
        } else {
            cout << "You chose to bowl first." << endl;
            // Simulate bowling innings
            cout << "Your bowling innings starts..." << endl;
            // You can add bowling logic here
        }
    } else {
        if (compChoice == 0) {
            cout << "Computer chose to bat first." << endl;
            // Simulate batting innings
            cout << "Computer's batting innings starts..." << endl;
            // You can add batting logic here
        } else {
            cout << "Computer chose to bowl first." << endl;
            // Simulate bowling innings
            cout << "Computer's bowling innings starts..." << endl;
            // You can add bowling logic here
        }
    }
}

// Function to choose number of overs
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

// Function to show the scorecard for a team
void showScorecard(const Team& team) {
    cout << "----------------- Scorecard -----------------" << endl;
    cout << "Player\t\tRuns\tBalls\tFours\tSixes\tStrike Rate\tStatus" << endl;
    cout << "---------------------------------------------" << endl;
    for (const auto& player : team.players) {
        cout << setw(10) << left << player.name << "\t"
             << setw(6) << right << player.runs << "\t"
             << setw(5) << right << player.balls << "\t"
             << setw(5) << right << player.fours << "\t"
             << setw(5) << right << player.sixes << "\t"
             << setw(11) << right << fixed << setprecision(2) << player.strikeRate << "\t";
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

// Function to simulate batting innings for a team
void simulateBatting(Team& battingTeam, Team& bowlingTeam, int numOvers) {
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

                // Update strike rate
currentBatsman.strikeRate = (static_cast<float>(currentBatsman.runs) / currentBatsman.balls) * 100;

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
    }

    // Display the batting team's final scorecard
    cout << "Batting innings completed for team " << battingTeam.name << ". Final scorecard:" << endl;
    showScorecard(battingTeam);
}



int main() {
    // Seed the random number generator
    srand(time(0));

    // Initialize user's team
    Team userTeam;
    cout << "Enter the name of your team: ";
    cin >> userTeam.name;
    int numUserPlayers = getNumericInput("Enter the number of players for your team (max 5): ", 1, 5);
    initializePlayers(userTeam, numUserPlayers, true); // True indicates user's team

    // Initialize computer team
    Team computerTeam;
    computerTeam.name = "Computer";
    int numComputerPlayers = numUserPlayers; // Use the same number of players as the user's team
    initializePlayers(computerTeam, numComputerPlayers, false); // False indicates computer's team

    // Perform toss and choose bat or ball
    bool userWinsToss = whoWinsToss();
    chooseBatOrBall(userWinsToss);

    // Choose number of overs
    int numOvers = chooseNumberOfOvers();
    cout << "You chose to play " << numOvers << " overs match." << endl;

    // Simulate batting innings for user's team
    simulateBatting(userTeam, computerTeam, numOvers);

    return 0;
}

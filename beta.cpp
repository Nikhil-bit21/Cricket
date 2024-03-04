#include<iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
#include <limits>
#include<iomanip>

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
        float strikeRate = (player.balls == 0) ? 0 : (static_cast<float>(player.runs) / player.balls) * 100;
        cout << setw(10) << left << player.name << "\t"
             << setw(6) << right << player.runs << "\t"
             << setw(5) << right << player.balls << "\t"
             << setw(5) << right << player.fours << "\t"
             << setw(5) << right << player.sixes << "\t"
             << setw(11) << right << fixed << setprecision(2) << strikeRate << "\t";
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

void showMidInningScorecard(const Team& battingTeam, int totalRuns, int totalWickets, int overs, int balls) {
    double totalOvers = overs + static_cast<double>(balls) / 10; // Combine overs and balls

    int targetScore = totalRuns + 1; // Calculate the target score for the chasing team

    cout << "==============================================================" << endl;
    cout << "|                     MID-INNING SCORECARD                   |" << endl;
    cout << "|------------------------------------------------------------|" << endl;
    cout << "| Team Name:       | " << setw(33) << left << battingTeam.name.substr(0, 15) << setfill(' ') << "|" << endl;
    cout << "| Total Runs:      | " << setw(33) << left << totalRuns << setfill(' ') << "|" << endl;
    cout << "| Total Wickets:   | " << setw(33) << left << totalWickets << setfill(' ') << "|" << endl;
    cout << "| Overs Played:    | " << setw(33) << left << fixed << setprecision(1) << totalOvers << setfill(' ') << "|" << endl;
    cout << "|------------------------------------------------------------|" << endl;
    cout << "|                       Batting Scorecard                   |" << endl;
    cout << "|------------------------------------------------------------|" << endl;
    // Show the detailed scorecard for the batting team
    showScorecard(battingTeam);
    cout << "|------------------------------------------------------------|" << endl;
    cout << "|                  End of Mid-Inning Scorecard               |" << endl;
    cout << "|------------------------------------------------------------|" << endl;
    cout << "|              Target for Chasing Team: " << setw(17) << left << targetScore << setfill(' ') << "|" << endl;
    cout << "==============================================================" << endl;
}

void showEndOfGameScorecard(const Team& team1, const Team& team2, int team1Runs, int team2Runs) {
    // Determine the match winner
    string matchWinner = (team1Runs > team2Runs) ? team1.name : ((team2Runs > team1Runs) ? team2.name : "Match Drawn");

    cout << "==============================================================" << endl;
    cout << "|                        END OF GAME                         |" << endl;
    cout << "|------------------------------------------------------------|" << endl;
    cout << "|                      Match Summary                         |" << endl;
    cout << "|------------------------------------------------------------|" << endl;
    cout << "| " << setw(13) << left << "Team" << "| " << setw(20) << left << "Runs Scored" << "|" << endl;
    cout << "|------------------------------------------------------------|" << endl;
    cout << "| " << setw(13) << left << team1.name << "| " << setw(20) << left << team1Runs << "|" << endl;
    cout << "| " << setw(13) << left << team2.name << "| " << setw(20) << left << team2Runs << "|" << endl;
    cout << "|------------------------------------------------------------|" << endl;
    cout << "|                    Match Winner                            |" << endl;
    cout << "|------------------------------------------------------------|" << endl;
    cout << "| " << setw(33) << left << matchWinner << "|" << endl;
    cout << "==============================================================" << endl;

    // Display scorecards for both teams
    cout << endl << "========================= Scorecards =========================" << endl;
    cout << "------------------- " << team1.name << " -------------------" << endl;
    showScorecard(team1);
    cout << "------------------- " << team2.name << " -------------------" << endl;
    showScorecard(team2);
    cout << "==============================================================" << endl;
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

            if (balls == 6 && overs < numOvers && currentBatsmanIndex < battingTeam.players.size()) {
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

    cout << "Mid-Inning Scorecard" << endl;
    showMidInningScorecard(battingTeam, totalRuns, currentBatsmanIndex, overs, balls);
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
        cout << "Press 'bowl' to bowl: ";
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

            if (balls == 6 && overs < numOvers && currentBatsmanIndex < battingTeam.players.size()) {
                balls = 0;
                overs++;
                cout << "End of over " << overs << endl;
                cout << "Total score: " << totalRuns << " runs for " << overs << " overs." << endl;

                // Show the scorecard for the batting team
                showScorecard(battingTeam);
            }
        } else {
            cout << "Invalid input! Press 'bowl' to bowl." << endl;
        }
    }

    cout << "Mid-Inning Scorecard" << endl;
    showMidInningScorecard(battingTeam, totalRuns, currentBatsmanIndex, overs, balls);
}

void simulateBattingWithTarget(Team& battingTeam, int numOvers, int targetScore) {
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

                if (runs == 4) {
                    currentBatsman.fours++; // Increment number of fours
                } else if (runs == 6) {
                    currentBatsman.sixes++; // Increment number of sixes
                }
                currentBatsman.status = 0; // Mark player as not out
            }
            balls++;

            if (balls == 6 && currentBatsmanIndex < battingTeam.players.size() && overs < numOvers) {
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

        // Check if total runs have reached or exceeded the target score
        if (totalRuns >= targetScore) {
            // Display the batting team's final scorecard
            cout << "Batting innings completed for team " << battingTeam.name << ". Final scorecard:" << endl;
            showScorecard(battingTeam);
            // If the total runs are greater than or equal to the target score, the team wins
            cout << battingTeam.name << " won the match by chasing the target!" << endl;
            return;
        }
    }

    // If the total runs are equal to the target score, it's a tie
        cout << "Batting innings completed for team " << battingTeam.name << ". Final scorecard:" << endl;
        showScorecard(battingTeam);

    // Check for tie
    if (totalRuns == targetScore-1) {
        
        cout<<"tie"<<endl;
    } else {
        
        cout << battingTeam.name << " Loss the match as they are not able to Chase the target!" << endl;
    }
}

void simulateBowlingWithTarget(Team& battingTeam, int numOvers, int targetScore) {
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
        cout << "Press 'bowl' to bowl: ";
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

                if (runs == 4) {
                    currentBatsman.fours++; // Increment number of fours
                } else if (runs == 6) {
                    currentBatsman.sixes++; // Increment number of sixes
                }
                currentBatsman.status = 0; // Mark player as not out
            }
            balls++;

            if (balls == 6 && currentBatsmanIndex < battingTeam.players.size() && overs < numOvers) {
                balls = 0;
                overs++;
                cout << "End of over " << overs << endl;
                cout << "Total score: " << totalRuns << " runs for " << overs << " overs." << endl;

                // Show the scorecard for the batting team
                showScorecard(battingTeam);
            }
        } else {
            cout << "Invalid input! Press 'bowl' to play a shot." << endl;
        }

        // Check if total runs have reached or exceeded the target score
        if (totalRuns >= targetScore) {
            // Display the batting team's final scorecard
            cout << "Batting innings completed for team " << battingTeam.name << ". Final scorecard:" << endl;
            showScorecard(battingTeam);
            // If the total runs are greater than or equal to the target score, the team wins
            cout << battingTeam.name << " won the match by chasing the target!" << endl;
            return;
        }
    }

    // If the total runs are equal to the target score, it's a tie
        cout << "Batting innings completed for team " << battingTeam.name << ". Final scorecard:" << endl;
        showScorecard(battingTeam);

    // Check for tie
    if (totalRuns == targetScore-1) {
        
        cout<<"tie"<<endl;
    } else {
        
        cout << battingTeam.name << " Loss the match as they are not able to chase the target!" << endl;
    }
}


int main() {
    srand(time(0));

    // Display welcome message
    cout << "---------------------------------------" << endl;
    cout << "|=========== GULLY-CRICKET ===========|" << endl;
    cout << "|                                     |" << endl;
    cout << "|   Welcome to our GULLY CRICKET..!   |" << endl;
    cout << "---------------------------------------" << endl;

    // Display menu
    cout << "Choose a game mode:" << endl;
    cout << "1. Player vs Computer" << endl;
    cout << "2. Player vs Player" << endl;
    cout << "Enter your choice (1/2): ";

    int choice;
    cin >> choice;

    switch (choice) {
        case 1: {
            // Player vs Computer mode
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
                int targetScore = 0;
                for (const auto& player : userTeam.players) {
                targetScore += player.runs;
                    }
                targetScore += 1; // Increment by 1 to ensure the target score is higher than the user's team score

                simulateBowlingWithTarget(computerTeam, numOvers,targetScore);
            } else {
                simulateBowling(computerTeam, numOvers);
                int targetScore = 0;
                for (const auto& player : computerTeam.players) {
                targetScore += player.runs;
                    }
                targetScore += 1; // Increment by 1 to ensure the target score is higher than the user's team score

                simulateBattingWithTarget(userTeam, numOvers,targetScore);
            }

            break;
        }
        case 2: {
            // Player vs Player mode
            // Implement player vs player logic
            cout << "Player vs Player mode is not implemented yet." << endl;
            break;
        }
        default:
            cout << "Invalid choice! Exiting the game." << endl;
            break;
    }

    return 0;
}


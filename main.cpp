#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <random>
#include <unordered_set>
#include <chrono>
#include <thread>
#include <future>

using namespace std;

// Class to hold each question and its options
class Question {
public:
    Question(const string& questionText, const vector<string>& options, int correctOption)
        : questionText(questionText), options(options), correctOption(correctOption) {}

    void display(int questionNumber) const {
        cout << "Question " << questionNumber + 1 << ": " << questionText << endl;
        for (size_t i = 0; i < options.size(); ++i) {
            cout << i + 1 << ". " << options[i] << endl;
        }
    }

    bool isCorrect(int answer) const {
        return answer == correctOption;
    }

    const string& getCorrectOption() const {
        return options[correctOption];
    }

    void addOption(const string& option) {
        options.push_back(option);
    }

    void setCorrectOption(int correctOptionIndex) {
        correctOption = correctOptionIndex;
    }

private:
    string questionText;
    vector<string> options;
    int correctOption; // Index of the correct option
};

// Class to hold player scores for the leaderboard
class PlayerScore {
public:
    PlayerScore(const string& name, int score)
        : name(name), score(score) {}

    const string& getName() const { return name; }
    int getScore() const { return score; }

private:
    string name;
    int score;
};

// Class to manage the quiz game
class QuizGame {
public:
    QuizGame(const string& questionsFile, const string& leaderboardFile)
        : questionsFile(questionsFile), leaderboardFile(leaderboardFile) {}

    void loadQuestions(int numberOfQuestions) {
        unordered_set<int> selectedIndices = generateRandomNumbers(100, numberOfQuestions);
        ifstream file(questionsFile);
        string line;
        int questionIndex = -1;
        int currentQuestion = 0;

        while (getline(file, line)) {
            if (line.empty()) continue; // Skip empty lines

            if (currentQuestion % 6 == 0) { // Start of a new question block
                questionIndex++;
            }

            if (selectedIndices.find(questionIndex) != selectedIndices.end()) {
                if (currentQuestion % 6 == 0) {
                    questions.push_back(Question(line, {}, 0));
                } else if (currentQuestion % 6 <= 4) {
                    questions.back().addOption(line);
                } else {
                    questions.back().setCorrectOption(stoi(line) - 1); // Adjust for 0-based index
                }
            }

            currentQuestion++;
        }

        if (questions.size() < numberOfQuestions) {
            throw runtime_error("Not enough questions loaded. Please check the file.");
        }
    }

    void startGame() {
        cout << "Enter your name: ";
        string playerName;
        cin >> playerName;

        int score = 0;
        const int timeLimitSeconds = 10; // Time limit for each question in seconds

        for (size_t i = 0; i < questions.size(); ++i) {
            questions[i].display(i);
            int userAnswer = getAnswerWithTimer(timeLimitSeconds);
            if (questions[i].isCorrect(userAnswer)) {
                cout << "Correct!" << endl;
                score++;
            } else if (userAnswer == -1) {
                cout << "No answer provided in time." << endl;
            } 
            else {
                cout << "Wrong! The correct answer is: " << questions[i].getCorrectOption() << endl;
            }
            cout << endl;
        }

        cout << "Quiz finished!" << endl;
        cout << "Your final score is: " << score << " out of " << questions.size() << endl;

        saveScoreToLeaderboard(playerName, score);
        displayLeaderboard();
    }

private:
    string questionsFile;
    string leaderboardFile;
    vector<Question> questions;

    static int getAnswerWithTimer(int timeLimitSeconds) {
        int answer = -1;
        auto future = std::async(std::launch::async, [&answer]() {
            cin >> answer;
        });

        if (future.wait_for(std::chrono::seconds(timeLimitSeconds)) == std::future_status::timeout) {
            cout << "Time's up!" << endl;
            return -1; // Indicate that no answer was provided in time
        }

        return answer - 1; // Adjusting for 0-based index
    }

    static unordered_set<int> generateRandomNumbers(int range, int count) {
        unordered_set<int> randomNumbers;
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(0, range - 1);

        while (randomNumbers.size() < count) {
            randomNumbers.insert(dis(gen));
        }

        return randomNumbers;
    }

    void saveScoreToLeaderboard(const string& playerName, int score) {
        ofstream file(leaderboardFile, ios::app);
        file << playerName << " " << score << endl;
    }

    void displayLeaderboard() const {
        ifstream file(leaderboardFile);
        string line;
        vector<PlayerScore> scores;

        while (getline(file, line)) {
            istringstream iss(line);
            string name;
            int score;
            iss >> name >> score;
            scores.emplace_back(name, score);
        }

        sort(scores.begin(), scores.end(), [](const PlayerScore& a, const PlayerScore& b) {
            return b.getScore() < a.getScore(); // Sort in descending order of scores
        });

        cout << "Leaderboard:" << endl;
        for (const auto& ps : scores) {
            cout << ps.getName() << ": " << ps.getScore() << endl;
        }
    }
};

int main() {
    try {
        QuizGame game("questions.bin", "leaderboard.bin");
        game.loadQuestions(5);
        game.startGame();
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}

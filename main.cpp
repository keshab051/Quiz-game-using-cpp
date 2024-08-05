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

class Question
{
public:
    Question(const std::string &questionText, const std::vector<std::string> &options, int correctOption)
        : questionText(questionText), options(options), correctOption(correctOption) {}

    void display(int questionNumber) const
    {
        std::cout << "Question " << questionNumber + 1 << ": " << questionText << std::endl;
        for (size_t i = 0; i < options.size(); ++i)
        {
            std::cout << i + 1 << ". " << options[i] << std::endl;
        }
    }

    bool isCorrect(int answer) const
    {
        return answer == correctOption;
    }

    const std::string &getCorrectOption() const
    {
        return options[correctOption];
    }

    void addOption(const std::string &option)
    {
        options.push_back(option);
    }

    void setCorrectOption(int correctOptionIndex)
    {
        correctOption = correctOptionIndex;
    }

private:
    std::string questionText;
    std::vector<std::string> options;
    int correctOption; // Index of the correct option
};

// Class to hold player scores for the leaderboard
class PlayerScore
{
public:
    PlayerScore(const std::string &name, int score)
        : name(name), score(score) {}

    const std::string &getName() const { return name; }
    int getScore() const { return score; }

private:
    std::string name;
    int score;
};

// Class to manage the quiz game
class QuizGame
{
public:
    QuizGame(const std::string &questionsFile, const std::string &leaderboardFile)
        : questionsFile(questionsFile), leaderboardFile(leaderboardFile) {}

    void loadQuestions(int numberOfQuestions)
    {
        std::vector<int> questionIndices = generateRandomNumbers(100, numberOfQuestions);
        std::ifstream file(questionsFile);
        std::string line;
        int questionIndex = -1;
        int currentQuestion = 0;
        int selectedQuestionIndex = 0;

        while (std::getline(file, line))
        {
            if (line.empty())
                continue; // Skip empty lines

            if (currentQuestion % 6 == 0)
            { // Start of a new question block
                questionIndex++;
            }

            if (selectedQuestionIndex < questionIndices.size() && questionIndex == questionIndices[selectedQuestionIndex])
            {
                if (currentQuestion % 6 == 0)
                {
                    questions.push_back(Question(line, {}, 0));
                }
                else if (currentQuestion % 6 <= 4)
                {
                    questions.back().addOption(line);
                }
                else
                {
                    questions.back().setCorrectOption(std::stoi(line) - 1); // Adjust for 0-based index
                    selectedQuestionIndex++;
                }
            }

            currentQuestion++;
        }

        if (questions.size() < numberOfQuestions)
        {
            throw std::runtime_error("Not enough questions loaded. Please check the file.");
        }
    }

    void startGame()
    {
        std::cout << "Enter your full name: ";
        std::string playerName;
        std::getline(std::cin, playerName);

        int score = 0;
        const int timeLimitSeconds = 10; // Time limit for each question in seconds

        for (size_t i = 0; i < questions.size(); ++i)
        {
            questions[i].display(i);
            int userAnswer = getAnswerWithTimer(timeLimitSeconds);
            if (questions[i].isCorrect(userAnswer))
            {
                std::cout << "Correct!" << std::endl;
                score++;
            }
            else if (userAnswer == -1)
            {
                std::cout << "No answer provided in time." << std::endl;
            }
            else
            {
                std::cout << "Wrong! The correct answer is: " << questions[i].getCorrectOption() << std::endl;
            }
            std::cout << std::endl;
        }

        std::cout << "Quiz finished!" << std::endl;
        std::cout << "Your final score is: " << score << " out of " << questions.size() << std::endl;

        saveScoreToLeaderboard(playerName, score);
        displayLeaderboard();
    }

private:
    std::string questionsFile;
    std::string leaderboardFile;
    std::vector<Question> questions;

    static int getAnswerWithTimer(int timeLimitSeconds)
    {
        int answer = -1;
        auto future = std::async(std::launch::async, [&answer]()
                                 { std::cin >> answer; });

        if (future.wait_for(std::chrono::seconds(timeLimitSeconds)) == std::future_status::timeout)
        {
            std::cout << "Time's up!" << std::endl;
            return -1; // Indicate that no answer was provided in time
        }

        return answer - 1; // Adjusting for 0-based index
    }

    static std::vector<int> generateRandomNumbers(int range, int count)
    {
        std::vector<int> randomNumbers;
        std::unordered_set<int> uniqueNumbers;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, range - 1);

        while (uniqueNumbers.size() < count)
        {
            int num = dis(gen);
            if (uniqueNumbers.find(num) == uniqueNumbers.end())
            {
                uniqueNumbers.insert(num);
                randomNumbers.push_back(num);
            }
        }

        std::sort(randomNumbers.begin(), randomNumbers.end());
        return randomNumbers;
    }

    void saveScoreToLeaderboard(const std::string &playerName, int score)
    {
        std::ofstream file(leaderboardFile, std::ios::app);
        file << playerName << " " << score << std::endl;
    }

    void displayLeaderboard() const
    {
        std::ifstream file(leaderboardFile);
        std::string line;
        std::vector<PlayerScore> scores;

        while (std::getline(file, line))
        {
            std::istringstream iss(line);
            std::string name;
            int score;
            iss >> name >> score;
            scores.emplace_back(name, score);
        }

        std::sort(scores.begin(), scores.end(), [](const PlayerScore &a, const PlayerScore &b)
             {
                 return b.getScore() < a.getScore(); // Sort in descending order of scores
             });

        std::cout << "Leaderboard:" << std::endl;
        for (const auto &ps : scores)
        {
            std::cout << ps.getName() << ": " << ps.getScore() << std::endl;
        }
    }
};

int main()
{
    try
    {
        QuizGame game("questions.txt", "leaderboard.txt");
        game.loadQuestions(5);
        game.startGame();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

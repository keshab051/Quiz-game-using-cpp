#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <conio.h> // For _getch()

class Question
{
public:
    std::string questionText;
    std::string options[4];
    int correctOption;

    void inputQuestion()
    {
        std::cin.ignore(); // Ignore any leftover newline characters in the input buffer
        std::cout << "Enter the question: ";
        std::getline(std::cin, questionText);

        for (int i = 0; i < 4; ++i)
        {
            std::cout << "Enter option " << i + 1 << ": ";
            std::getline(std::cin, options[i]);
        }

        std::cout << "Enter the correct option number (1-4): ";
        std::cin >> correctOption;
        std::cin.ignore(); // Clear the newline character left by std::cin
    }

    void saveQuestion(std::ofstream &outFile) const
    {
        outFile << questionText << std::endl;
        for (const auto &option : options)
        {
            outFile << option << std::endl;
        }
        outFile << correctOption << std::endl;
        outFile << std::endl; // Add an empty line after each question
    }

    void loadQuestion(std::ifstream &inFile)
    {
        std::getline(inFile, questionText);
        for (auto &option : options)
        {
            std::getline(inFile, option);
        }
        inFile >> correctOption;
        inFile.ignore(); // Ignore the newline character after the integer
        inFile.ignore(); // Ignore the empty line
    }
};

class QuizManager
{
private:
    std::string password;
    std::vector<Question> questions;

    bool checkPassword(const std::string &inputPassword) const
    {
        return inputPassword == password;
    }

    void updatePassword()
    {
        std::cout << "Enter new password: ";
        password = getMaskedInput();
        std::ofstream passwordFile("password.txt");
        if (!passwordFile)
        {
            std::cerr << "Error opening password file for writing." << std::endl;
            return;
        }
        passwordFile << password;
        passwordFile.close();
        std::cout << "Password updated successfully." << std::endl;
    }

    void saveAllQuestions() const
    {
        std::ofstream outFile("questions.txt");
        if (!outFile)
        {
            std::cerr << "Error opening file for writing." << std::endl;
            return;
        }
        for (const auto &q : questions)
        {
            q.saveQuestion(outFile);
        }
        outFile.close();
    }

    void loadQuestions()
    {
        std::ifstream inFile("questions.txt");
        if (!inFile)
        {
            std::cerr << "Error opening file for reading." << std::endl;
            return;
        }
        questions.clear();
        Question q;
        while (inFile.peek() != EOF)
        {
            q.loadQuestion(inFile);
            questions.push_back(q);
        }
        inFile.close();
    }

    std::string getMaskedInput() const
    {
        std::string input;
        char ch;
        while ((ch = _getch()) != 13) // Enter key is pressed
        {
            if (ch == 8) // Backspace is pressed
            {
                if (!input.empty())
                {
                    std::cout << "\b \b";
                    input.pop_back();
                }
            }
            else
            {
                std::cout << '*';
                input.push_back(ch);
            }
        }
        std::cout << std::endl;
        return input;
    }

public:
    QuizManager()
    {
        std::ifstream passwordFile("password.txt");
        if (!passwordFile)
        {
            std::ofstream outFile("password.txt");
            password = "admin"; // Default password
            outFile << password;
            outFile.close();
        }
        else
        {
            std::getline(passwordFile, password);
            passwordFile.close();
        }
        loadQuestions();
    }

    void addQuestion()
    {
        Question q;
        q.inputQuestion();
        questions.push_back(q);
        saveAllQuestions();
    }

    void deleteQuestion()
    {
        if (questions.empty())
        {
            std::cout << "No questions available to delete." << std::endl;
            return;
        }

        for (size_t i = 0; i < questions.size(); ++i)
        {
            std::cout << i + 1 << ". " << questions[i].questionText << std::endl;
        }

        int questionNumber;
        std::cout << "Enter the number of the question to delete: ";
        std::cin >> questionNumber;

        if (questionNumber < 1 || questionNumber > questions.size())
        {
            std::cout << "Invalid question number." << std::endl;
            return;
        }

        questions.erase(questions.begin() + (questionNumber - 1));
        saveAllQuestions();
        std::cout << "Question deleted successfully." << std::endl;
    }

    void adminPanel()
    {
        std::string inputPassword;
        std::cout << "Enter password to access admin panel: ";
        inputPassword = getMaskedInput();

        if (!checkPassword(inputPassword))
        {
            std::cout << "Incorrect password. Access denied." << std::endl;
            return;
        }

        int choice;
        do
        {
            std::cout << "\nWelcome Back, Chief!\n";
            std::cout << "1. Add a new question\n";
            std::cout << "2. Delete a question\n";
            std::cout << "3. Update password\n";
            std::cout << "4. Exit\n";
            std::cout << "Enter your choice: ";
            std::cin >> choice;

            switch (choice)
            {
            case 1:
                addQuestion();
                break;
            case 2:
                deleteQuestion();
                break;
            case 3:
                updatePassword();
                break;
            case 4:
                std::cout << "Exiting..." << std::endl;
                break;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
                break;
            }
        } while (choice != 4);
    }
};

int main()
{
    QuizManager quizManager;
    quizManager.adminPanel();
    return 0;
}
 
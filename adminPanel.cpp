#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class Question
{
public:
    string questionText;
    string options[4];
    int correctOption;

    void inputQuestion()
    {
        cin.ignore(); // Ignore any leftover newline characters in the input buffer
        cout << "Enter the question: ";
        getline(cin, questionText);

        for (int i = 0; i < 4; ++i)
        {
            cout << "Enter option " << i + 1 << ": ";
            getline(cin, options[i]);
        }

        cout << "Enter the correct option number (1-4): ";
        cin >> correctOption;
        cin.ignore(); // Clear the newline character left by cin
    }

    void saveQuestion(ofstream &outFile) const
    {
        outFile << questionText << endl;
        for (const auto &option : options)
        {
            outFile << option << endl;
        }
        outFile << correctOption << endl;
        outFile << endl; // Add an empty line after each question
    }

    void loadQuestion(ifstream &inFile)
    {
        getline(inFile, questionText);
        for (auto &option : options)
        {
            getline(inFile, option);
        }
        inFile >> correctOption;
        inFile.ignore(); // Ignore the newline character after the integer
        inFile.ignore(); // Ignore the empty line
    }
};

class QuizManager
{
private:
    string password;
    vector<Question> questions;

    bool checkPassword(const string &inputPassword) const
    {
        return inputPassword == password;
    }

    void updatePassword()
    {
        cout << "Enter new password: ";
        cin >> password;
        ofstream passwordFile("password.txt");
        if (!passwordFile)
        {
            cerr << "Error opening password file for writing." << endl;
            return;
        }
        passwordFile << password;
        passwordFile.close();
        cout << "Password updated successfully." << endl;
    }

    void saveAllQuestions() const
    {
        ofstream outFile("questions.txt");
        if (!outFile)
        {
            cerr << "Error opening file for writing." << endl;
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
        ifstream inFile("questions.txt");
        if (!inFile)
        {
            cerr << "Error opening file for reading." << endl;
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

public:
    QuizManager()
    {
        ifstream passwordFile("password.txt");
        if (!passwordFile)
        {
            ofstream outFile("password.txt");
            password = "admin"; // Default password
            outFile << password;
            outFile.close();
        }
        else
        {
            getline(passwordFile, password);
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
            cout << "No questions available to delete." << endl;
            return;
        }

        for (size_t i = 0; i < questions.size(); ++i)
        {
            cout << i + 1 << ". " << questions[i].questionText << endl;
        }

        int questionNumber;
        cout << "Enter the number of the question to delete: ";
        cin >> questionNumber;

        if (questionNumber < 1 || questionNumber > questions.size())
        {
            cout << "Invalid question number." << endl;
            return;
        }

        questions.erase(questions.begin() + (questionNumber - 1));
        saveAllQuestions();
        cout << "Question deleted successfully." << endl;
    }

    void adminPanel()
    {
        string inputPassword;
        cout << "Enter password to access admin panel: ";
        cin >> inputPassword;

        if (!checkPassword(inputPassword))
        {
            cout << "Incorrect password. Access denied." << endl;
            return;
        }

        int choice;
        do
        {
            cout << "\nWelcome Back, Chief!\n";
            cout << "1. Add a new question\n";
            cout << "2. Delete a question\n";
            cout << "3. Update password\n";
            cout << "4. Exit\n";
            cout << "Enter your choice: ";
            cin >> choice;

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
                cout << "Exiting..." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
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

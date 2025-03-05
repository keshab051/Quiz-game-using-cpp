# Quiz Arena
Welcome to Quiz Arena, a C++-based quiz game developed by Team Brainy Bunch. This game tests your knowledge with timed questions, tracks your score, and displays a leaderboard to compare your performance with others.

## Features
- Timed Questions: Answer questions within a time limit (default: 10 seconds per question).

- Randomized Questions: Questions are randomly selected from a pool of 100 questions.

- Leaderboard: Tracks player scores and displays them in descending order.

- User-Friendly Interface: Simple and intuitive interface for players to interact with.

- Customizable Questions: Questions and options are loaded from a text file (questions.txt), making it easy to add or modify  questions.

# How It Works
## 1 Question Loading:

- **Questions are loaded from a file (questions.txt) in a specific format**:

- **Each question block consists of**:

   -  1 line for the question text.

    - 4 lines for the options.

    - 1 line for the correct option index (1-based).

## 2. Gameplay:
-  Players enter their name and answer a series of randomly 
      selected questions.

  - Each question has a time limit, and players must answer before time runs out.

## 3. Scoring:
- Correct answers increase the player's score.
    
- Incorrect or unanswered questions do not contribute to the score.

## 4. Leaderboard:

- Player scores are saved to a file (leaderboard.txt).

- The leaderboard is displayed at the end of the game, sorted in descending order of scores.

# Requirements
 - C++ Compiler: Ensure you have a C++ compiler installed (e.g., GCC, Clang, or MSVC).

 - Input File: A questions.txt file containing questions in the required format.

- Output File: A leaderboard.txt file to store player scores (created automatically if it doesn't exist).


# How to Run
- Clone or download the repository.

- Ensure questions.txt is populated with questions.

- Compile the code using a C++ compiler:

``` bash 
g++ -o quiz_arena main.cpp
```
- Run the executable:

```bash
./quiz_arena
```

## Developed By
  - Apekshya Koirala

- Anuja Katuwal

- Keshab Neupane

- Arun Aryal

## License
This project is open-source and available under the MIT License. Feel free to modify and distribute it as needed.

   Enjoy the game and challenge yourself with Quiz Arena! 🎮


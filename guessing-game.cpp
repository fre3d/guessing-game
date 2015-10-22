#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <algorithm>
#include <ctime>

using namespace std;

void initValues();
void welcomePlayer();
void welcomeDoubleOrNothing(int randomVectorSize, int vectorSize);
void printHighlightArray(vector<int> inputArray, vector<int> currentSessionVector, string message);
void printArray(vector<int> inputArray, int number, string message);
void clearCin();
void clearScreen();
int inputNumber(string message, int max_range);
int generateRandom(int maxRandom, int offset);
int getRandomElement(vector<int> inputArray);
int calculatePoints(int countGuesses);
bool inputChoice(string message, string firstChoice, string secondChoice);
bool inputChoice(string message, const vector<string>& firstChoice, const vector<string>& secondChoice);
bool checkMatching(int firstNumber, int secondNumber);
bool checkLower(int firstNumber, int secondNumber);
bool elementExists(vector<int> inputArray, int number);
vector<int> appendToArray(vector<int> inputArray, int num);
vector<int> makeArray(vector<int> inputArray, int size, int maxRandom);

const vector<string> yesSet = {"y","yes","sure","affirmative"};
const vector<string> noSet = {"n","no","negative"};

int main(){

    initValues();

    clearScreen();

    welcomePlayer();

    int defaultRange = 50;

    bool useCustomRange = inputChoice("Do you want to choose a range?", yesSet, noSet);

    int range;
    if(useCustomRange){
        range = inputNumber("Choose a range:", 0);
    }else{
        range = defaultRange;
    }

    int gameOver = false;
    int points = 0;

    vector<int> allGamesVector;

    while(gameOver==false){

        bool currentGameRunning = true;

        vector<int> currentSessionVector;

        int countGuesses = 0;
        int random = generateRandom(range, 1);

        while(currentGameRunning){

            bool playDoubleOrNothing = false;
            bool restart = false;

            if(currentSessionVector.size()>0){
                printArray(currentSessionVector, random, "These are the guesses you made so far:");
            }

            cout << "Random number: " << random<< endl;
            int number;
            bool checkDuplicate = true;
            while(checkDuplicate){
                cout << "Guess which number between 0 and " << range << " I am thinking about: ";
                number = inputNumber("", range);
                bool duplicateExist = elementExists(currentSessionVector, number);
                if(duplicateExist){
                cout << "Error, you have already guessed on this number!" << endl;
                printArray(currentSessionVector, random, "These are the guesses you made before:");
                }
                else{
                    checkDuplicate = false;
                }
            }

            currentSessionVector = appendToArray(currentSessionVector, number);
            allGamesVector = appendToArray(allGamesVector, number);

            countGuesses++;

            if(checkMatching(number, random)){
                cout << endl << "You figured out the number I was thinking about. Well done!" << endl;
                int calcPoints = calculatePoints(countGuesses);
                points = points + calcPoints;
                cout << "Your points: " << points << endl << endl;
                if(points>0){
                    playDoubleOrNothing = inputChoice("Do you want to play double or nothing?", "y", "n");
                    if(!playDoubleOrNothing){
                        bool continuePlaying = inputChoice("Do you want to keep playing?", "y", "n");
                        if(!continuePlaying){
                            gameOver=true;
                        }else{
                            restart=true;
                        }
                    }
                }else{
                    gameOver=true;
                }
            }
            else if(checkLower(number, random)){
                cout << endl << "The number I am thinking about is larger..." << endl << endl;
            }
            else{
                cout << endl << "The number I am thinking about is smaller..." << endl << endl;
            }

            if(playDoubleOrNothing){
                int randomVectorSize = 5;
                int vectorSize = currentSessionVector.size();
                vector<int> randomVector;
                randomVector = makeArray(allGamesVector, randomVectorSize, range);
                int randomNumber = getRandomElement(randomVector);
                int chosenNum;
                bool elementFound = false;
                cout << "Random number is: " << randomNumber << endl;
                welcomeDoubleOrNothing(randomVectorSize, vectorSize);
                while(!elementFound){
                    printHighlightArray(randomVector, allGamesVector, "Alternatives from what you guessed on before:");
                    cout << endl;
                    chosenNum = inputNumber("Enter one of the numbers: ", range);
                    elementFound = elementExists(randomVector, chosenNum);
                    if(!elementFound){
                        cout << "ERROR, does not exist!" << endl;
                    }
                }
                bool match = checkMatching(chosenNum, randomNumber);
                if(match){
                    points = points*2;
                    cout << ">>>>>>>>>>>>>>WINNER<<<<<<<<<<<<<<<" << endl << endl;

                }else{
                    points = 0;
                    cout << ">>>>>>>>>>>>>>Loser<<<<<<<<<<<<<<<<" << endl << endl;
                    cout << "Sorry, you chose the wrong number. You should have chosen: " << randomNumber << endl;
                }
                cout << "Your points: " << points << endl;
                if(points>0){
                    bool continuePlaying = inputChoice("Do you want to guess again?, all points and previous guesses will be saved.", "y", "n");
                    if(!continuePlaying){
                        gameOver = true;
                    }else{
                        restart = true;
                    }
                }else{
                    gameOver = true;
                }
            }
            if(gameOver){
                cout << "Game Over" << endl;
                if(points>0){
                    cout << "Your highscore is: " << points << " points" << endl;
                }
                currentGameRunning=false;
            }
            if(restart){
                cout << "Restarting..." << endl;
                currentGameRunning=false;
            }
        }
    }

return 0;

}

//Initiate some values
void initValues(){
    srand(time(NULL));
}

/*Prints a message welcoming the player to the game*/
void welcomePlayer(){
    cout << "Welcome to the game" << endl;
    bool showRules = inputChoice("Do you want to see the rules?", "y", "n");
    if(showRules){
        cout << "Guess which number the computer is thinking about." << endl;
        cout << "Points will be awarded depending on how many guesses ";
        cout << "it took before a correct guess was made." << endl << endl;
        cout << "The points are as following:" << endl;
        cout << "10p for 1 guess, ";
        cout << "5p for 2 guesses, ";
        cout << "1p for between 3 and under 10 guesses." << endl << endl;

    }
}

/*
Prints a message welcoming the player to the gamemode "Double or Nothing".
*/
void welcomeDoubleOrNothing(int randomVectorSize, int vectorSize){
    cout << "Welcome to Double or Nothing, " << endl;
    cout << randomVectorSize << " numbers will be chosen from your previous guesses.";
    cout << endl << "Guess which of these " << randomVectorSize;
    cout << " numbers I am thinking about." << endl;
    cout << "If you guess correctly you double your points, otherwise you lose";
    cout << " all your points." << endl;
    if(vectorSize<randomVectorSize){
        cout << endl << "Since you guessed correctly within " << randomVectorSize;
        cout << " times, the rest of the list will be random numbers" << endl;
        cout << "(Random numbers have a paranthesis around them)";
    }
    cout << endl << endl;
}

/*
Function for input of a number with a set range, prints a message before asking
for input. Return number if valid.
*/
int inputNumber(string message, int max_range){
    int input;
    bool validInput = false;
    while(!validInput){
        cout << message;
        cout << endl << "> ";
        cin >> input;
        clearCin();
        clearScreen();
        if(!(input>0)){
            cout << endl << "ERROR, input must be a positive number between: ";
            cout << "[ 0-" << max_range << " ]." << endl;
        }
        else{
            if((input <= max_range) || (max_range==0)){
                validInput = true;
            }
            else{
                cout << endl << "ERROR, value is not within correct range." << endl;
                cout << "Correct range is: [ 0-" << max_range << " ]." << endl;
            }
        }
    }
    return input;
}

/*
Handles input from the player, the choices that can be made are custom.
Returns true or false depending on what the player chose if the input was
valid.
*/
bool inputChoice(string message, string positive, string negative){
    vector<string> positiveChoices;
    positiveChoices.push_back(positive);
    vector<string> negativeChoices;
    negativeChoices.push_back(negative);
    return inputChoice(message, positiveChoices, negativeChoices);
}

bool inputChoice(string message, const vector<string>& positiveChoices, const vector<string>& negativeChoices){
    bool validAnswer = false;
    bool answer;

    if (positiveChoices.size() == 0 || negativeChoices.size() == 0)
    {
        cerr << "Error: inputChoice called with zero length vectors." << endl;
        exit(0);
    }

    while(!validAnswer){
        string input;
        cout << message << ", [" << *positiveChoices.begin() << "] or [" << *negativeChoices.begin() << "]" << endl << "> ";
        cin >> input;
        clearCin();
        clearScreen();
        if( find(positiveChoices.begin(), positiveChoices.end(), input) != positiveChoices.end() ){
            answer = true;
            validAnswer = true;
        }
        else if( find(negativeChoices.begin(), negativeChoices.end(), input) != negativeChoices.end() ){
            answer = false;
            validAnswer = true;
        }
        else{
            cout << endl << "ERROR, Not a valid choice." << endl << endl;
        }
    }
    return answer;
}

int generateRandom(int maxRandom, int offset)
{
  int numberRan = rand() % maxRandom + offset;
  return numberRan;
}

bool checkMatching(int firstNumber, int secondNumber){
    return (firstNumber==secondNumber);
}

bool checkLower(int firstNumber, int secondNumber){
    return (firstNumber < secondNumber);
}

int getRandomElement(vector<int> inputArray){
    int randomIndex = generateRandom(inputArray.size(), 0);
    int randomElement = inputArray.at(randomIndex);
    return randomElement;
}

vector<int> appendToArray(vector<int> inputArray, int num){
    inputArray.push_back(num);
    if(inputArray.size()>1){
        sort(inputArray.begin(), inputArray.end());
    }
    return inputArray;
}

bool elementExists(vector<int> inputArray, int number){
    bool foundElement = false;
    for(int i=0; i<inputArray.size(); i++){
        if(inputArray.at(i)==number){
            foundElement = true;
        }
    }
    return foundElement;
}

vector<int> makeArray(vector<int> inputArray, int size, int maxRandom){
    vector<int> outputArray(size);
    if(inputArray.size() <= size){
        for(int i=0; i < inputArray.size(); i++){
            outputArray.at(i) = inputArray.at(i);
        }
        for(int j=inputArray.size(); j < size; j++){
            bool checkDuplicate = true;
            while(checkDuplicate){
                int random = generateRandom(maxRandom, 1);
                int isDuplicate = elementExists(outputArray, random);
                if(!isDuplicate){
                    outputArray.at(j) = random;
                    checkDuplicate = false;
                }
            }

        }
    }else{
        for(int k=0; k<size; k++){
            bool checkDuplicate = true;
            while(checkDuplicate){
                int randomIndex = generateRandom(inputArray.size(), 0);
                int chosenNumber = inputArray.at(randomIndex);
                int isDuplicate = elementExists(outputArray, chosenNumber);
                if(!isDuplicate){
                    outputArray.at(k) = chosenNumber;
                    checkDuplicate = false;
                }
            }
        }
    }
    sort(outputArray.begin(), outputArray.end());
    return outputArray;

}

void printHighlightArray(vector<int> inputArray, vector<int> currentSessionVector, string message){
    cout << message << endl;
    if(inputArray.size()>0){
        for(int i=0; i<inputArray.size(); i++){
            bool checkChosen = false;
            for(int j=0; j<currentSessionVector.size(); j++){
                if(inputArray.at(i)==currentSessionVector.at(j)){
                    cout << inputArray.at(i) << " ";
                    checkChosen = true;
                }
            }
            if(!checkChosen){
                cout << "(" << inputArray.at(i) << ") ";
            }
        }
    }
    cout << endl;
}

void printArray(vector<int> inputArray, int number, string message){
    cout << message << endl;
    if(inputArray.size()>0){
        for(int i=0; i<inputArray.size(); i++){
            if(inputArray.at(i)>number){
                cout << " <-[" << inputArray.at(i) << "] ";
            }
            else if(inputArray.at(i)<number){
                cout << " [" << inputArray.at(i) << "]-> ";
            }
            else{
                cout << "[" << inputArray.at(i) << "] ";
            }
        }
    }
    cout << endl << endl;
}

int calculatePoints(int count){
    int points = 0;
    if(count==1){
        points = 10;
    }
    else if(1 < count && count < 3){
        points = 5;
    }
    else if(3 <= count && count < 10){
        points = 1;
    }
    return points;
}

void clearCin()
{
    cin.clear();
    cin.ignore(5000, '\n');
}

void clearScreen(){
  cout << string( 150, '\n' );
}

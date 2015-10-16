#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

void initValues();
void welcomePlayer();
int inputNumber(string message, int max_range);
bool inputChoice(string message, string firstChoice, string secondChoice);
int inputRange(int defaultRange, bool customRange, string message);
int chooseRange();
bool checkMatching(int firstNumber, int secondNumber);
bool checkLower(int firstNumber, int secondNumber);
int returnCaseResult(int firstNumber, int secondNumber);
int generateRandom(int maxRandom, int offset);
vector<int> appendToArray(vector<int> inputArray, int num);
vector<int> makeArray(vector<int> inputArray, int size, int maxRandom);
vector<int> sortArray(vector<int> inputArray);
bool elementExists(vector<int> inputArray, int number);
int calculatePoints(int countGuesses);
int getRandomElement(vector<int> inputArray);
void clearCin();
void printArray(vector<int> vector, string message);
void printEasyArray(vector<int> vector, int number, string message);
void clearScreen();

int main(){

    initValues();

    welcomePlayer();

    bool useCustomRange = inputChoice("Do you want to choose a range? [y] or [n]", "y", "n");

    int range = inputRange(50, useCustomRange, "Choose a range:");

    int gameOver = false;

    vector<int> allGamesVector;
    int gameMode = 1;

    while(gameOver==false){

        int countGuesses = 0;
        int random = generateRandom(range, 1);
        vector<int> currentSessionVector;

        while(gameMode == 1){

            //cout << endl << "Random number: " << random << endl;
            if(currentSessionVector.size()>0){
                printEasyArray(currentSessionVector, random, "These are the guesses you made before:");
            }

            int number;
            bool checkDuplicate = true;
            while(checkDuplicate){
                number = inputNumber("Guess which number i am thinking about:", range);
                bool duplicateExist = elementExists(currentSessionVector, number);
                if(duplicateExist){
                cout << "Error, you have already guessed on this number!" << endl;
                printEasyArray(currentSessionVector, random, "These are the guesses you made before:");
                }
                else{
                    checkDuplicate = false;
                }
            }

            currentSessionVector = appendToArray(currentSessionVector, number);
            //currentSessionVector.push_back(number);
            countGuesses++;

            if(checkMatching(number, random)){
                cout << endl << "You figured out the number i was thinking about. Well done!" << endl;
                int points = calculatePoints(countGuesses);
                cout << "Your points: " << points << endl << endl;
                if(points>0){
                    bool playDoubleOrNothing = inputChoice("Do you want to play double or nothing? [y] or [n]", "y", "n");
                    if(playDoubleOrNothing){
                        gameMode=2;
                    }
                }else{
                    bool restart = inputChoice("Game Over, do you want to play again? [y] or [n]", "y", "n");
                    if(restart){
                        cout << endl << "Restarting..." << endl;
                        //Fix later
                    }
                    else{
                        gameMode=0;
                        gameOver=true;
                    }
                }
            }
            else if(checkLower(number, random)){
                cout << endl << "The number i am thinking about is larger..." << endl << endl;
            }
            else{
                cout << endl << "The number i am thinking about is smaller..." << endl << endl;
            }
        }
        while(gameMode==2){
            cout << "GAMEMODE 2" << endl;
            vector<int> testV = makeArray(currentSessionVector, 5, range);
            int randomNumber = getRandomElement(testV);
            int chosenNum;
            bool elementFound = false;
            while(!elementFound){
                printArray(testV, "Alternatives from what you guessed on before:");
                chosenNum = inputNumber("Enter one of the numbers: ", range);
                elementFound = elementExists(testV, chosenNum);
                if(!elementFound){
                    cout << "ERROR, does not exist!" << endl;
                }
            }
            bool match = checkMatching(chosenNum, randomNumber);
            if(match){
                cout << ">>>>>>>>>>>>>>WINNER<<<<<<<<<<<<<<<" << endl << endl;
            }else{
                cout << ">>>>>>>>>>>>>>Loser<<<<<<<<<<<<<<<" << endl << endl;
            }
            //gameMode=0;
            //gameOver=true;
            //merge vectors
        }

    }

}

void initValues(){
    srand(time(NULL));
}

void welcomePlayer(){
    cout << "Welcome to the game" << endl;
}

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

bool inputChoice(string message, string firstChoice, string secondChoice){
    bool validAnswer = false;
    bool answer;
    while(!validAnswer){
        string input;
        cout << message << endl << "> ";
        cin >> input;
        clearCin();
        clearScreen();
        if(input==firstChoice){
            answer = true;
            validAnswer = true;
        }
        else if(input==secondChoice){
            answer = false;
            validAnswer = true;
        }
        else{
            cout << endl << "ERROR, Not a valid choice." << endl << endl;
        }
    }
    return answer;
}

int inputRange(int defaultRange, bool customRange, string message){
    int input;
    if(customRange){
        input = inputNumber(message, 0);
    }
    else{
        input = defaultRange;
    }
    return input;
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

void printArray(vector<int> vector, string message){
    cout << message << endl;
    if(vector.size()>0){
        for(int i=0; i<vector.size(); i++){
            cout << "[" << vector.at(i) << "] ";
        }
    }
    cout << endl;
}

void printEasyArray(vector<int> vector, int number, string message){
    cout << message << endl;
    if(vector.size()>0){
        for(int i=0; i<vector.size(); i++){
            if(vector.at(i)>number){
                cout << " <-[" << vector.at(i) << "] ";
            }
            else if(vector.at(i)<number){
                cout << " [" << vector.at(i) << "]-> ";
            }
            else{
                cout << "[" << vector.at(i) << "] ";
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

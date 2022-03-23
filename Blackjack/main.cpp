#include <iostream>
#include <time.h>
#include <cstdlib>

int drawFromDeck();
void shuffleDeck();
void betMoney(int &bet, int playerMoney, int dealerMoney);
void printOverview(int playerMoney, int dealerMoney);
void playerTurn(int &points);
void dealerTurn(int &points, int playerPoints);
void playerDraws(int &playerPoints);
void askForKeepPlaying(char &playAgain);

// I look at the deck of cards as points, and never as actual cards.
// By doing that, I don't need to "convert" the cards to points.
// How I manage my deck in the code is exactly how we would do it in real life.
// I initialize the deck as a "newly" bought deck of cards (where the cards are
// all nicely ordered). Then I shuffle the cards and start drawing from the top.
// When I reach the bottom, the game ends (that will never happen here though).
// At the start of a new round, I reshuffle the already messy deck.

// I could use for loops to assign these values for the array,
// but I figured it would be more readable like this.
int deckOfCards[52]{1,1,1,1,      2,2,2,2,      3,3,3,3,      4,4,4,4,
                    5,5,5,5,      6,6,6,6,      7,7,7,7,      8,8,8,8,
                    9,9,9,9,    10,10,10,10,   10,10,10,10,  10,10,10,10,   10,10,10,10};

// I will use this as the index for deckOfCards[]
// to keep track of where in the deck we are.
// Example: positionInDeck = 51, means we are at the top.
//          positionInDeck = 0, means we are at the last card.
int positionInDeck{};

int main()
{
    int playerMoney{100};
    int dealerMoney{100};
    int bet{};
    char keepPlaying{}; // at the end of each round, you can input 'y' to continue or 'n' to exit game

    std::cout << "WELCOME TO BLACK JACK!\n\n";
    std::cout << "RULES:\n"
                 "- You are playing against the dealer.\n"
                 "- The goal is to reach the highest score.\n"
                 "- You can't exceed 21 points.\n"
                 "- You obtain points by drawing cards.\n"
                 "- You both start with 100$ each.\n\n"
                 "CARD VALUES:\n"
                 "- A card's value is equal to its corresponding number.\n"
                 "- Example: six of clubs = 6 points.\n"
                 "- Ace = 1 point or 11 points (you decide).\n"
                 "- Jack, Queen and King = 10 points.\n\n";
    std::cout << "--------------------------------\n";
    system("PAUSE");

    do{ // this do while loop is the whole game
    int playerPoints{0};
    int dealerPoints{0};

    shuffleDeck();
    betMoney(bet, playerMoney, dealerMoney);

    system("CLS");
    std::cout << "YOU BET " << bet << "$\n\n";
    std::cout << "The dealer matched your bet.\n";
    std::cout << "You are first to draw cards.\n\n";
    std::cout << "--------------------------------\n";
    system("PAUSE");

    playerTurn(playerPoints);

    // checks if the player lost by drawing more than 21 points.
    if (playerPoints > 21)
    {
        std::cout << "Player points : " << playerPoints << "\n";
        std::cout << "\nYou got more than 21 points, which caused you to lose the round!\n";
        playerMoney -= bet;
        dealerMoney += bet;
        std::cout << "You hand over " << bet << "$ to the dealer.\n\n";
        printOverview(playerMoney, dealerMoney);

        if (playerMoney < 10){ // player needs at least 10$ to continue
            goto gameIsOver; // game is over and the game will exit
        } else {
            goto roundIsOver; // player got enough money to continue, so only the round is over.
        }
    }

    // at this point, playerPoints HAS to be below 22 points (becaues of the if statement above).
    // I'm keeping this in mind while making the logic for my "win" conditions later on.

    std::cout << "You now have " << playerPoints << " points\n";
    std::cout << "Next up is the dealer's turn.\n\n";
    std::cout << "--------------------------------\n";
    system("PAUSE");

    dealerTurn(dealerPoints, playerPoints);

    if (playerPoints == dealerPoints) {
        std::cout << "IT'S A DRAW!\n\n";
        std::cout << "The pot was discarded.\n\n";
        printOverview(playerMoney, dealerMoney);
    } else if (dealerPoints <= 21 && dealerPoints > playerPoints) {
        std::cout << "THE DEALER WON THE ROUND!\n\n";
        playerMoney -= bet;
        dealerMoney += bet;
        std::cout << "You hand over " << bet << "$ to the dealer.\n\n";
        printOverview(playerMoney, dealerMoney);
        if (playerMoney < 10) goto gameIsOver; // player don't got enough money to continue
    } else {
        std::cout << "YOU WON THE ROUND!\n\n";
        playerMoney += bet;
        dealerMoney -= bet;
        std::cout << "The dealer gives you the " << bet << "$ as promised.\n\n";
        printOverview(playerMoney, dealerMoney);
        if (dealerMoney < 10) goto gameIsOver; // the dealer don't got enough money to continue
    };

    roundIsOver:
    askForKeepPlaying(keepPlaying);
    } while (keepPlaying == 'y');

    gameIsOver:
    if (keepPlaying == 'n'){
        system("CLS");
        std::cout << "You decided to leave with " << playerMoney << "$ in your pocket.\n\n";
    } else if (playerMoney < 10){
        std::cout << "You have " << playerMoney << "$ left.\nNot enough to continue...\n\n";
    } else{
        // If the game is over, and you did not input 'n',
        // and you still have enough money to continue,
        // the only option left is that the dealer is too poor.
        std::cout << "He then realises that he doesn't have enough money to continue!\n"
                     "You leave happily with " << playerMoney << "$ in your pocket.\n\n";
    }
    std::cout << "--------------------------------------\n";

    return 0;
}

void shuffleDeck()
{
    int temp;
    int randomIndex;
    unsigned int seed;

    seed = static_cast<unsigned int>(time(nullptr));
    srand(seed);

    // My shuffle logic is to swap the position between two cards
    // (a card at position [i] and a random card), 52 times.
    for (int i{0}; i < 52; i++)
    {
        randomIndex = rand() % 52; // 52 = size of the deck
        temp = deckOfCards[i]; // I make a copy of the value stored in deckOfCards[i] to not lose its value when I overwrite it.
        deckOfCards[i] = deckOfCards[randomIndex]; // I overwrite the deckOfCards[i] with a random value in the array.
        deckOfCards[randomIndex] = temp; // I set the value that was stored in deckOfCards[i] to now be deckOfCards[randomIndex}.
    }

    positionInDeck = 51; // will now start to draw from the top after every shuffle
}

void betMoney(int &bet, int playerMoney, int dealerMoney)
{
    int counter{0}; // this is used to keep track of how many times the do while loop has looped

    do{
    system("CLS");
    printOverview(playerMoney, dealerMoney);
    std::cout << "-------------------------------------\n";

    if (counter == 0){ // if it's the first time in the do while loop
        std::cout << "How much money would you like to bet?\n(min 10$) : ";
        counter ++;
    } else{
        std::cout << "Bet has to be minimum 10$.\n"
                     "Bet can't exceed dealer's capital.\n"
                     "Maybe you don't have enough money?\n\n(min 10$) : ";
    }

    std::cin >> bet;
    } while (bet > dealerMoney || bet < 10 || bet > playerMoney);
    // bet has to be equal or below the dealer's capital
    // bet can't be below 10
    // bet has to be equal or below the player's capital
}

void printOverview(int playerMoney, int dealerMoney)
{
    std::cout << "OVERVIEW\n";
    std::cout << "Player : " << playerMoney << "$\n";
    std::cout << "Dealer : " << dealerMoney << "$\n\n";
}

void playerTurn(int &points)
{
    char input{};

    // these 5 lines of code is to make the player
    // prepared to draw cards. Instead of just instantly
    // giving the player points at the previous "Press any key to continue..."
    system("CLS");
    std::cout << "YOU CAN NOW DRAW\n\n";
    std::cout << "Player points : " << points << "\n\n";
    std::cout << "--------------------------------\n";
    system("PAUSE");

    system("CLS");
    playerDraws(points);
    do{
    std::cout << "Player points : " << points << "\n\n";
    std::cout << "-------------------------\n";
    std::cout << "(d) to draw, (s) to stop\nMove : ";
    std::cin >> input;

        switch (input)
        {
        case 'd' : case 'D':
            system("CLS");
            playerDraws(points);
            break;

        case 's' : case 'S':
            system("CLS");
            std::cout << "YOU DECIDED TO STOP\n\n";
            break; // will now exit the loop because input = 's' (check the condition for the loop)

        default :
            system("CLS");
            std::cout << "INVALID\n\n";
            break;
        }

    // if points are equal to 21, the player is automatically forced
    // to choose "STOP" (as it doesn't make sense to draw).
    } while (input != 's' && input != 'S' && points < 21);
}

void playerDraws(int &playerPoints)
{
    int pointsDrawn{};

    pointsDrawn = drawFromDeck();
    if (pointsDrawn == 1)
    {
        do{
        std::cout << "YOU DREW AN ACE!\n\n"
                     "How many points would you like it to be?\n"
                     "You currently have " << playerPoints << " points.\n\n"
                     "--------------\n(1 or 11) : ";
        std::cin >> pointsDrawn;

        system("CLS");
        } while (pointsDrawn != 1 && pointsDrawn != 11); // this makes sure the player inputs a valid number
    }

    std::cout << "YOU DREW " << pointsDrawn << " POINTS!\n\n";
    playerPoints += pointsDrawn;
}

int drawFromDeck()
{
    int drawnCard{};

    drawnCard = deckOfCards[positionInDeck];
    positionInDeck --;

    // if positionInDeck < 0, the game should end or deck should be reshuffled.
    // This is not necessary to implement as it is impossible to reach this number
    // (because I've decided to reshuffle the deck between each round).
    // I could add it as a "just in case" if I decide to update the game in the future
    // (where more players could join). I don't see that being a possibility for a game
    // like Black Jack though.

    return drawnCard;
}

void dealerTurn(int &dealerPoints, int playerPoints)
{
    int pointsDrawn{};

    // same here as when the player draws cards. This little code
    // prepares the player for what's about to happen.
    system("CLS");
    std::cout << "THE DEALER IS READY TO DRAW\n\n";
    std::cout << "Player points : " << playerPoints << "\n";
    std::cout << "Dealer points : " << dealerPoints << "\n\n";
    std::cout << "--------------------------------\n";
    system("PAUSE");

    oneLastTime:
    do{
        system("CLS");

        pointsDrawn = drawFromDeck();
        pointsDrawn = (pointsDrawn == 1 && dealerPoints < 11) ? 11 : pointsDrawn; // this is my ace logic
        std::cout << "THE DEALER DREW " << pointsDrawn << " POINTS!\n\n";

        dealerPoints += pointsDrawn;
        std::cout << "Player points : " << playerPoints << "\n";
        std::cout << "Dealer points : " << dealerPoints << "\n\n";
        std::cout << "--------------------------------\n";
        system("PAUSE");

    // The daeler can draw points as long as it is not equal to playerPoints
    // or above playerPoints.
    } while (dealerPoints != playerPoints && dealerPoints < playerPoints);

    // this makes sure that the dealer won't stop for a draw at a low score
    if(dealerPoints == playerPoints && playerPoints < 15){
        goto oneLastTime;
    }

    system("CLS");
}

void askForKeepPlaying(char &keepPlaying)
{
    do{
        std::cout << "---------------------------\n";
        std::cout << "Would you like to continue?" << "\n(y/n) : ";
        std::cin >> keepPlaying;
    } while (keepPlaying != 'y' && keepPlaying != 'n'); // safety check for invalid inputs
}

//
//  main.cpp
//  DiceGame
//
//  Created by Ben Adkins on 10/7/19.
//  Copyright © 2019 Ben Adkins. All rights reserved.
//
#include <string>
#include <iostream>
#include <new>
#include <cstdlib>
#include <ctime>

using namespace std;

int dieRoll(int *weighting);
int placeBet(int &bankroll);
void win(int bet, int &bankroll);
string setPoint(int sum);
string setRollValue(int sum2);

int main() {
    int weight[6];
    int num = 0;
    int bankroll = 1000;
    int bet;
    int firstRoll = 0;
    int secondRoll = 0;
    int sum = 0;
    int sum2 = 0;
    string point;
    string rollValue;
    string weightAns;
    bool crapOut = false;
    
    srand(time(NULL));
    
    cout << "Would you like to weigh the dice? (Yes or No)" << endl;
    getline(cin, weightAns);
    if (weightAns.compare("Yes") == 0 || weightAns.compare("yes") == 0)
    {
        cout << "Which number would you like to weigh? (1 - 6)" << endl;
        cin >> num;
        cout << "\n" << endl;
    }
    else
        cout << "Thanks for playing fair\n" << endl;
    
    switch (num)
    {
        case 0:
            for (int i = 0; i < 6; i++)
            {
                weight[i] = 357921099; // .16667 of 2 147 483 647
            }
            break;
        case 1:
            weight[0] = 536870912; // .25 of 2147483647
            for (int i = 1; i < 6; i++)
            {
                weight[i] = 322122547; // .15 of 2147483647
            }
            break;
        case 2:
            weight[0] = 322122547; // .15 of 2147483647
            weight[1] = 536870912; // .25 of 2147483647
            for (int i = 2; i < 6; i++)
            {
                weight[i] = 322122547; // .15 of 2147483647
            }
            break;
        case 3:
            weight[0] = 322122547; // .15 of 2147483647
            weight[1] = 322122547; // .15 of 2147483647
            weight[2] = 536870912; // .25 of 2147483647
            for (int i = 3; i < 6; i++)
            {
                weight[i] = 322122547; // .15 of 2147483647
            }
            break;
        case 4:
            for (int i = 0; i < 3; i++)
            {
                weight[i] = 322122547; // .15 of 2147483647
            }
            weight[3] = 536870912; // .25 of 2147483647
            for (int i = 4; i < 6; i++)
            {
                weight[i] = 322122547; // .15 of 2147483647
            }
            break;
        case 5:
            for (int i = 0; i < 4; i++)
            {
                weight[i] = 322122547; // .15 of 2147483647
            }
            weight[4] = 536870912; // .25 of 2147483647
            weight[5] = 322122547; // .15 of 2147483647
            break;
        case 6:
            for (int i = 0; i < 5; i++)
            {
                weight[i] = 322122547; // .15 of 2147483647
            }
            weight[5] = 536870912; // .25 of 2147483647
            break;
        default: cout << "num not valid" << endl;
    }
    
    do
    {
        
        cout << "Player's Bank: " << bankroll << endl;
        bet = placeBet(bankroll);
        cout << "Player's Bet: " << bet << endl;
        
        firstRoll = dieRoll(weight);
        secondRoll = dieRoll(weight);
        
        
        cout << "\tCome-out roll: " << firstRoll << " " << secondRoll << endl;
        
        sum = firstRoll + secondRoll;
        if (sum == 2)
        {
            cout << "\tSnake eyes! Player craps out." << endl;
            crapOut = true;
        }
        else if (sum == 3)
        {
            cout << "\tAcey-Duecy! Player craps out." << endl;
            crapOut = true;
        }
        else if (sum == 12)
        {
            cout << "\tBox Cars! Player craps out." << endl;
            crapOut = true;
        }
        else if (sum == 7)
        {
            cout << "\tBig Red! Player gets a natural." << endl;
            win(bet, bankroll);
            crapOut = false;
        }
        else if (sum == 11)
        {
            cout << "\tYo-leven! Player gets a natural." << endl;
            win(bet, bankroll);
            crapOut = false;
        }
        else
        {
            point = setPoint(sum);
            cout << "\tPoint set! Point is " << point << endl;
            crapOut = false;
        }
        
        sum2 = 0;
        
        if (crapOut == false)
        {
            while(sum2 != sum && sum2 != 7 && sum != 7 && sum != 11)
            {
                firstRoll = dieRoll(weight);
                secondRoll = dieRoll(weight);
                sum2 = firstRoll + secondRoll;
                rollValue = setRollValue(sum2);
                
                cout << "\tRoll-Out Roll: " << firstRoll << " " << secondRoll << endl;
                
                if (sum2 == 7)
                {
                    cout << "\tSeven Out! Player loses." << endl;
                }
                else if (sum2 == sum)
                {
                    cout << "\t" << rollValue << "! Point made, player wins!" << endl;
                    win(bet, bankroll);
                }
                else
                {
                    cout << "\t" << rollValue << "! Point not made, player re-rolls." << endl;
                }
                
            }
        }
        
        
    } while(bankroll > 0 && bankroll <= 2000);
    
    return 0;
}

int dieRoll(int *weighting)
{
    double temp[6];
    
    for (int i = 0; i < 6; i++)
    {
        temp[i] = *(weighting+i);
    }
    
    /*
     I took the liberty of adjusting the random number generator you gave us. I wasn't satisfied with the puesdo-random number generated by your code so I adjusted it. My thought process behind it is that if I generate a puesdo-random number seeded by computer time (using the "srand()" function), I'll get a different set of puesdo-random numbers given my application is ran at differnt computer times. With this different set of numbers, I can adjust the likely hood of a certain result from a roll by adjusting the shares of 2147483647 possibilities (the range of numbers generated when the "rand()" function is called) owned by each result on the die. With an unweighted die this is easy, just return whatever remainder, using the modulus operation, of six plus 1 "rand()" generates. This should give each result a 16.667% chance of occuring. With a weighted die it gets a little trickier. You must take 214748365 (10.000% of 2147483647) shares of 2147483647 from each possible result not intended to be weighted and adjust the value of "r" to a value that works for your weighted result. After this your weighted value should have a 25% chance of occuring while every remaining result will only have a 15% chance of occuring. I know this because regardless of the weight of the dice your desire result should occur 16.667% of the time naturally; however, if you take 10.000% of the remaining 83.333% chance of any result besides your desired result occuring, your desired result will now occur 25.000% of the time. Because each remaining result has now essentially lost 10.000% of its natural 16.667% occurances, every result outside your desired result will only occur 15.000% [16.667% - 10.000%(16.667%)] of the time.
     
     P(desired result) = P(naturally occuring) + .10[P(every other naturally occuring result)]
     P(desired result) = 16.667% + 10.000%(83.333%)
     P(desired result) = 25.000%
     
     P(undesired result) = P(naturally occuring) - .10[P(naturally occuring)]
     P(undesired result) = 16.667% - 10.000%(16.667%)
     P(undesired result) = 15.000%
     
     I hope this adjustment is okay with you. I was forced to move away from your use of ratios inside your "weight" array because the switch function will not accept floating-point variables. I tried my best to make my code as easy on the eye as possible despite the many cases of arithmetic between realitivly large integers. I'd appreciate any feedback regarding an easier way to implement such concept or advice regarding any poor coding techniques (if any) implemented in this concept.
     */
    
    int die = -1;
    int r = (rand()%6)+1;
    
    if (temp[0] == 357921099) // .16667 of 2147483647
    {
        die = r;
        return die;
    }
    
    // if one is weighted take 10.000% of 2147483647 from each possibility of r != 1
    if (temp[0] == 536870912) // .25 of 2147483647
    {
        switch(r)
        {
            case 1:
                r = 178949812; // .08333 of 2147483647
                break;
            case 2:
            {
                int oneW = rand();
                if (oneW <= 214748365) //.10000 of 2147483647
                    r = 178949812; // .08333 of 2147483647
                else
                    r = 536892387; // .25001 of 2147483647
                break;
            }
            case 3:
            {
                int oneW = rand();
                if (oneW <= 214748365) //.10000 of 2147483647
                    r = 178949812; // .08333 of 2147483647
                else
                    r = 894792011; // .41667 of 2147483647
                break;
            }
            case 4:
            {
                int oneW = rand();
                if (oneW <= 214748365) //.10000 of 2147483647
                    r = 178949812; // .08333 of 2147483647
                else
                    r = 1252713111; // .58334 of 2147483647
                break;
            }
            case 5:
            {
                int oneW = rand();
                if (oneW <= 214748365) //.10000 of 2147483647
                    r = 178949812; // .08333 of 2147483647
                else
                    r = 1610634210; // .75001 of 2147483647
                break;
            }
            case 6:
            {
                int oneW = rand();
                if (oneW <= 214748365) //.10000 of 2147483647
                    r = 178949812; // .08333 of 2147483647
                else
                    r = 1968555310; // .91668 of 2147483647
                break;
            }
            default:
                cout << "variable r of type double is outside of expected range (1-6)" << endl;
        }
        
    }
    
    // if two is weighted take 10.000% of 2147483647 from each possibility of r != 2
    if (temp[1] == 536870912) // .25 of 2147483647
    {
        switch(r)
        {
            case 1:
            {
                int twoW = rand();
                if (twoW <= 214748365) //.10000 of 2147483647
                    r = 472446402; // .22000 of 2147483647
                else
                    r = 178949812; // .08333 of 2147483647
                break;
            }
            case 2:
                r = 472446402; // .22000 of 2147483647
                break;
            case 3:
            {
                int twoW = rand();
                if (twoW <= 214748365) //.10000 of 2147483647
                    r = 472446402; // .22000 of 2147483647
                else
                    r = 894792011; // .41667 of 2147483647
                break;
            }
            case 4:
            {
                int twoW = rand();
                if (twoW <= 214748365) //.10000 of 2147483647
                    r = 472446402; // .22000 of 2147483647
                else
                    r = 1252713111; // .58334 of 2147483647
                break;
            }
            case 5:
            {
                int twoW = rand();
                if (twoW <= 214748365) //.10000 of 2147483647
                    r = 472446402; // .22000 of 2147483647
                else
                    r = 1610634210; // .75001 of 2147483647
                break;
            }
            case 6:
            {
                int twoW = rand();
                if (twoW <= 214748365) //.10000 of 2147483647
                    r = 472446402; // .22000 of 2147483647
                else
                    r = 1968555310; // .91668 of 2147483647
                break;
            }
            default:
                cout << "variable r of type double is outside of expected range (1-6)" << endl;
        }
        
    }
    
    // if three is weighted take 10.000% of 2147483647 from each possibility of r != 3
    if (temp[2] == 536870912) // .25 of 2147483647
    {
        switch(r)
        {
            case 1:
            {
                int threeW = rand();
                if (threeW <= 214748365) //.10000 of 2147483647
                    r = 894792011; // .41667 of 2147483647
                else
                    r = 178949812; // .08333 of 2147483647
                break;
            }
            case 2:
            {
                int threeW = rand();
                if (threeW <= 214748365) //.10000 of 2147483647
                    r = 894792011; // .41667 of 2147483647
                else
                    r = 536892387; // .25001 of 2147483647
                break;
            }
            case 3:
                r = 894792011; // .41667 of 2147483647
                break;
            case 4:
            {
                int threeW = rand();
                if (threeW <= 214748365) //.10000 of 2147483647
                    r = 894792011; // .41667 of 2147483647
                else
                    r = 1252713111; // .58334 of 2147483647
                break;
            }
            case 5:
            {
                int threeW = rand();
                if (threeW <= 214748365) //.10000 of 2147483647
                    r = 894792011; // .41667 of 2147483647
                else
                    r = 1610634210; // .75001 of 2147483647
                break;
            }
            case 6:
            {
                int threeW = rand();
                if (threeW <= 214748365) //.10000 of 2147483647
                    r = 894792011; // .41667 of 2147483647
                else
                    r = 1968555310; // .91668 of 2147483647
                break;
            }
            default:
                cout << "variable r of type double is outside of expected range (1-6)" << endl;
        }
        
    }
    
    // if four is weighted take 10.000% of 2147483647 from each possibility of r != 4
    if (temp[3] == 536870912) // .25 of 2147483647
    {
        switch(r)
        {
            case 1:
            {
                int fourW = rand();
                if (fourW <= 214748365) //.10000 of 2147483647
                    r = 1252713111; // .58334 of 2147483647
                else
                    r = 178949812; // .08333 of 2147483647
                break;
            }
            case 2:
            {
                int fourW = rand();
                if (fourW <= 214748365) //.10000 of 2147483647
                    r = 1252713111; // .58334 of 2147483647
                else
                    r = 536892387; // .25001 of 2147483647
                break;
            }
            case 3:
            {
                int fourW = rand();
                if (fourW <= 214748365) //.10000 of 2147483647
                    r = 1252713111; // .58334 of 2147483647
                else
                    r = 894792011; // .41667 of 2147483647
                break;
            }
            case 4:
                r = 1252713111; // .58334 of 2147483647
                break;
            case 5:
            {
                int fourW = rand();
                if (fourW <= 214748365) //.10000 of 2147483647
                    r = 1252713111; // .58334 of 2147483647
                else
                    r = 1610634210; // .75001 of 2147483647
                break;
            }
            case 6:
            {
                int fourW = rand();
                if (fourW <= 214748365) //.10000 of 2147483647
                    r = 1252713111; // .58334 of 2147483647
                else
                    r = 1968555310; // .91668 of 2147483647
                break;
            }
            default:
                cout << "variable r of type double is outside of expected range (1-6)" << endl;
        }
        
    }
    
    // if five is weighted take 10.000% of 2147483647 from each possibility of r != 5
    if (temp[4] == 536870912) // .25 of 2147483647
    {
        switch(r)
        {
            case 1:
            {
                int fiveW = rand();
                if (fiveW <= 214748365) //.10000 of 2147483647
                    r = 1610634210; // .75001 of 2147483647
                else
                    r = 178949812; // .08333 of 2147483647
                break;
            }
            case 2:
            {
                int fiveW = rand();
                if (fiveW <= 214748365) //.10000 of 2147483647
                    r = 1610634210; // .75001 of 2147483647
                else
                    r = 536892387; // .25001 of 2147483647
                break;
            }
            case 3:
            {
                int fiveW = rand();
                if (fiveW <= 214748365) //.10000 of 2147483647
                    r = 1610634210; // .75001 of 2147483647
                else
                    r = 894792011; // .41667 of 2147483647
                break;
            }
            case 4:
            {
                int fiveW = rand();
                if (fiveW <= 214748365) //.10000 of 2147483647
                    r = 1610634210; // .75001 of 2147483647
                else
                    r = 1252713111; // .58334 of 2147483647
                break;
            }
            case 5:
                r = 1610634210; // .75001 of 2147483647
                break;
            case 6:
            {
                int fiveW = rand();
                if (fiveW <= 214748365) //.10000 of 2147483647
                    r = 1610634210; // .75001 of 2147483647
                else
                    r = 1968555310; // .91668 of 2147483647
                break;
            }
            default:
                cout << "variable r of type double is outside of expected range (1-6)" << endl;
        }
        
    }
    
    // if six is weighted take 10.000% of 2147483647 from each possibility of r != 6
    if (temp[5] == 536870912) // .25 of 2147483647
    {
        switch(r)
        {
            case 1:
            {
                int sixW = rand();
                if (sixW <= 214748365) //.10000 of 2147483647
                    r = 1968555310; // .91668 of 2147483647
                else
                    r = 178949812; // .08333 of 2147483647
                break;
            }
            case 2:
            {
                int sixW = rand();
                if (sixW <= 214748365) //.10000 of 2147483647
                    r = 1968555310; // .91668 of 2147483647
                else
                    r = 536892387; // .25001 of 2147483647
                break;
            }
            case 3:
            {
                int sixW = rand();
                if (sixW <= 214748365) //.10000 of 2147483647
                    r = 1968555310; // .91668 of 2147483647
                else
                    r = 894792011; // .41667 of 2147483647
                break;
            }
            case 4:
            {
                int sixW = rand();
                if (sixW <= 214748365) //.10000 of 2147483647
                    r = 1968555310; // .91668 of 2147483647
                else
                    r = 1252713111; // .58334 of 2147483647
                break;
            }
            case 5:
            {
                int sixW = rand();
                if (sixW <= 214748365) //.10000 of 2147483647
                    r = 1968555310; // .91668 of 2147483647
                else
                    r = 1610634210; // .75001 of 2147483647
                break;
            }
            case 6:
                r = 1968555310; // .91668 of 2147483647
                break;
            default:
                cout << "variable r of type double is outside of expected range (1-6)" << endl;
        }
        
    }
    
    while(r > 0)
    {
        die++;
        r = r - temp[die];
    }
    
    return die+1;
    
}

int placeBet(int &bankroll)
{
    if (bankroll >= 1000)
    {
        bankroll = bankroll - 100;
        return 100;
    }
    else if (bankroll < 1000 && bankroll >= 500)
    {
        bankroll = bankroll - 50;
        return 50;
    }
    else
    {
        bankroll = bankroll - 25;
        return 25;
    }
}

void win(int bet, int &bankroll)
{
    bankroll = bankroll + (bet*2);
}

string setPoint(int sum)
{
    if (sum == 4)
        return "Four";
    else if (sum == 5)
        return "Five";
    else if (sum == 6)
        return "Six";
    else if (sum == 8)
        return "Eight";
    else if (sum == 9)
        return "Nine";
    else
        return "Ten";
}

string setRollValue(int sum2)
{
    switch (sum2)
    {
        case 1:
        case 2:
            return "Two";
            break;
        case 3:
            return "Three";
            break;
        case 4:
            return "Four";
            break;
        case 5:
            return "Five";
            break;
        case 6:
            return "Six";
            break;
        case 7:
            return "Seven";
            break;
        case 8:
            return "Eight";
            break;
        case 9:
            return "Nine";
            break;
        case 10:
            return "Ten";
            break;
        case 11:
            return "Eleven";
            break;
        case 12:
            return "Twelve";
            break;
        default:
            return "Not A Valid Roll";
    }
}



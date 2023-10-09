// multithreading2.0.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <thread>
#include <stdio.h> 
#include <stdlib.h>
#include <time.h>    
#include <iostream>
#include <deque> 
#include <mutex>
#include <windows.h>

class Player
{

public:
    bool inGame;
    int delay;
    double chance;
    Player();
    Player(int, double);
    ~Player();
    void got_chair();
};


Player::Player()
{
}
Player::Player(int delayvar, double chancevar)
{
    this->delay = delayvar;  //determine how fast the contestant notice that the music stopped playing
    this->chance = chancevar;//determine chance that the contestant has been in an advantageous placewhen music stoped
    inGame = true;
}

std::deque<Player> AllPlayer;
int chairsLeft;
std::mutex mutexChair;

void got_chair(int contestantNR)
{
    Sleep(AllPlayer[contestantNR].delay);

    while (rand()%10 >= AllPlayer[contestantNR].chance) //how difficult founding of empty chair is at this very moment
    {
        Sleep(10);
    }
    mutexChair.lock();
    if (chairsLeft > 0)
    {
        chairsLeft--;
    }
    else
    {
        AllPlayer[contestantNR].inGame = false; //
    }
    mutexChair.unlock();

}

Player::~Player()
{
}

int main()
{

    srand(time(NULL));
    std::deque <std::thread> threadQueue;
    threadQueue.clear();
    AllPlayer.clear();

    int howManyPlayers = rand() % 10 + 5;
    while (AllPlayer.size() < howManyPlayers)
    {
        Player TmpPlayer(rand() % 50, rand() % 10 + 1);
        AllPlayer.push_back(TmpPlayer);
    }

    while (AllPlayer.size() > 1)
    {
        chairsLeft = AllPlayer.size() - 1;
        for (int i = 0; i < AllPlayer.size(); i++)
        {
            threadQueue.push_back(std::thread(got_chair, i));
        }

        for (auto& thr : threadQueue)
        {
            thr.join();
        }
        threadQueue.clear();

        for (int i = 0; i < AllPlayer.size(); i++)
        {
            Player TmpPlayer = AllPlayer.front();
            AllPlayer.pop_front();
            if (!TmpPlayer.inGame)
            {
                std::cout << " In this round Contestant with properties: chance- " << TmpPlayer.chance << "\tdelay- " << TmpPlayer.delay << " has died\n\t\t" << AllPlayer.size() << "\tleft \n";

            }
            else
            {
                AllPlayer.push_back(TmpPlayer);
            }
        }
    }
    std::cout<< "The first place goes to contestant with properties: chance- " << AllPlayer[0].chance << "\tdelay- " << AllPlayer[0].delay<<"\n\n\n\n";
    AllPlayer.clear();

    return 1;
}


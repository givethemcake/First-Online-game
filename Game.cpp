#include "Game.h"
#include <math.h>
#include<ctime>
#include <termios.h>
#include <unistd.h>
#include <netdb.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<cstring>
#include<stdio.h>
#include<arpa/inet.h>

void Game::sendPlayerBooleans(int Player1,int Player2)
{
     bool amPlayer1 = true;
     int sent= send(Player1, &amPlayer1, sizeof(amPlayer1), 0);
        
        if(sent==-1)
        {
            perror(nullptr);
            exit(-1);
        }
        else if(sent==0)
        {
            perror("Client Failed");
        }

        amPlayer1=false;
        sent= send(Player2, &amPlayer1, sizeof(amPlayer1), 0);

        if(sent==-1)
        {
            perror(nullptr);
            exit(-1);
        }
        else if(sent==0)
        {
            perror("Client Failed");
        }
}


Game* Game::currentActieInstance=nullptr;

void Game::randomizePlayerSpawn()
{
    int x, y;

    // Player 1
    x = rand() % (GameWidth - 2) + 1;
    y = rand() % (GameHeight - 2) + 1;

    Player1Pos.x = x;
    Player1Pos.y = y;

    // Player 2
    do
    {
        x = rand() % (GameWidth - 2) + 1;
        y = rand() % (GameHeight - 2) + 1;
    }
    while (x == Player1Pos.x && y == Player1Pos.y);

    Player2Pos.x = x;
    Player2Pos.y = y;
}

Game::Game():boundry('#'),playerOneTurn(true)
{
    
    //setup game arr
    srand(time(0));


    for(int i=0;i<GameHeight;i++)
    {
        for (int j=0;j<GameWidth;j++)
            {
                if( j==0 || j==GameWidth-1 ) gameArr[i][j] = boundry;
                else if(i==0 || i==GameHeight-1) gameArr[i][j]=boundry;
                else gameArr[i][j]=' ';
            }
    }

    while(true)
    {
        randomizePlayerSpawn();
        bool exit = !(Player1Pos.x ==- 1 || Player1Pos.y==-1 || Player2Pos.x ==-1 || Player2Pos.y==-1);
        if(exit) break;
    }

    currentActieInstance=this;

    gameArr[Player1Pos.y][Player1Pos.x] = '@';
    gameArr[Player2Pos.y][Player2Pos.x] = '%';

}

Game* Game::getGameInstance() 
{
    if(currentActieInstance==nullptr) return new Game;
    else return nullptr; 
    
    // dont give 2 people acess to same game instance 
    // and always maintain only one single instance
}


// bool Game::processTurn(char input)
// {
//     pos* currentPlayer;

//     if (playerOneTurn)
//         currentPlayer = &Player1Pos;
//     else
//         currentPlayer = &Player2Pos;

   

//     int newX = currentPlayer->x;
//     int newY = currentPlayer->y;

//     switch (input)
//     {
//         case 'w':
//         case 'W':
//             newY--;
//             break;

//         case 's':
//         case 'S':
//             newY++;
//             break;

//         case 'a':
//         case 'A':
//             newX--;
//             break;

//         case 'd':
//         case 'D':
//             newX++;
//             break;

//         default:
//             return false; // invalid input
//     }
//      gameArr[currentPlayer->y][currentPlayer->x] = ' ';

//     // The outer cells are boundaries, so valid coordinates are:
//     // x: 1 to GameWidth - 2
//     // y: 1 to GameHeight - 2
 
//     if(newX<=0) newX=1;
//     if(newY<=0) newY=1;
//     if(newY>=GameHeight-1) newY=GameHeight-3;
//     if(newX>=GameWidth-1) newX=GameWidth-3;

//     currentPlayer->x = newX;
//     currentPlayer->y = newY;

//     playerOneTurn = !playerOneTurn;

//     gameArr[Player1Pos.y][Player1Pos.x] = '@';
//     gameArr[Player2Pos.y][Player2Pos.x] = '%';

//     return true;
// }





bool Game::processTurn(char input, bool iAmPlayer1)
{
    pos* currentPlayer = iAmPlayer1 ? &Player1Pos : &Player2Pos;
    int newX = currentPlayer->x;
    int newY = currentPlayer->y;

    switch (input)
    {
        case 'w':
        case 'W':
            newY--;
            break;
        case 's':
        case 'S':
            newY++;
            break;
        case 'a':
        case 'A':
            newX--;
            break;
        case 'd':
        case 'D':
            newX++;
            break;
        default:
        {
            std::cout<<"[PROCESS INPUT] INVALID INPUT: "<<input<<std::endl;
            return false; // invalid input, turn not consumed
        }
    }

    gameArr[currentPlayer->y][currentPlayer->x] = ' ';

    // The outer cells are boundaries, so valid coordinates are:
    // x: 1 to GameWidth - 2
    // y: 1 to GameHeight - 2
    if(newX<=0) newX=1;
    if(newY<=0) newY=1;
    if(newY>=GameHeight-1) newY=GameHeight-3;
    if(newX>=GameWidth-1) newX=GameWidth-3;

    currentPlayer->x = newX;
    currentPlayer->y = newY;

    playerOneTurn = !playerOneTurn;

    gameArr[Player1Pos.y][Player1Pos.x] = '@';
    gameArr[Player2Pos.y][Player2Pos.x] = '%';

    std::cout<<"\n CHANGE STATE \n"<<std::endl;

    return true;
}

std::ostream& operator << (std::ostream& out, const Game& display)
{
        for(int i=0;i<display.GameHeight;i++)
        {
            for(int j=0;j<display.GameWidth;j++)
            {
                out<<display.gameArr[i][j];
            }
            out<<"\n";
        }     
        return out;
}

void Game::drawGame() const
{
    std::cout<<(*this);
}

Game:: ~Game()
{
    currentActieInstance=nullptr;
}

bool Game::sendGameArr(int Player1,int Player2)
{
    int Gsent=0;
    int needToSend=sizeof(gameArr);
    //std::cout<<"fml"<<std::endl;
    while(Gsent<needToSend)
    {
        int sent=send(Player1,(char*)gameArr+Gsent,sizeof(gameArr)-Gsent,0);
        Gsent+=sent;
        if (sent == -1) { perror("recv"); return false; }
        if(sent==0){perror("Server Closed the Connection");return false;}
    }
    send(Player1,&playerOneTurn,1,0);
    //std::cout<<"fml x2"<<std::endl;
    Gsent=0;
    while(Gsent<needToSend)
    {
        int sent=send(Player2,(char*)gameArr+Gsent,sizeof(gameArr)-Gsent,0);
        Gsent+=sent;
        if (sent == -1) { perror("recv"); return false; }
        if(sent==0){perror("Server Closed the Connection");return false;}
    }
    
    send(Player2,&playerOneTurn,1,0);
    //std::cout<<"fml x3"<<std::endl;
    return true;
}
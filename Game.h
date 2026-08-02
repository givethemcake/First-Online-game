#include<iostream>
struct pos
{
    int x;
    int y;
    pos():x(-1),y(-1){}
    
};

class Game
{
private:
    
    enum 
    {
        GameWidth=25,
        GameHeight=25
    };
 
    char gameArr[GameHeight][GameWidth];

    const char boundry;
    
    static Game*       currentActieInstance;

    pos Player1Pos;
    pos Player2Pos;


    Game();
    void randomizePlayerSpawn();
    
public:

    bool playerOneTurn;

    ~Game();
    static Game* getGameInstance();//returns nullptr if game is already running
    bool processTurn(char input,bool iAmPlayer1);//process user input and return either true of false
    void drawGame() const;
    void sendPlayerBooleans(int Player1,int Player2);
    bool sendGameArr(int Player1,int Player2);

    friend std::ostream& operator << (std::ostream& out, const Game& display);
    
    //no custom copy or assignment constructor needed
};
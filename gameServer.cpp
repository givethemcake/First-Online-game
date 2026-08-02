#include "Game.h"
#include <termios.h>
#include <unistd.h>
#include <netdb.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<cstring>
#include<stdio.h>
#include<arpa/inet.h>
#include<thread>
#include<atomic>

//what should state include that clients get access to?
//player turn to know if you can move yet
//current game arr for display
//gameWidth
//gameHeight
//do i make a struct containing all this and send it to my the client
//both machines are just gonna use machine btye order so no conversion needed
//the server handles all logic
//the clients just send the input to the server and draw whatever the current state is 



//THIS RETARDED FUCKING API


void RecordInput_1(std::atomic<char>& input1, int Player1, std::atomic<bool>& ready)
{
    while (true) {
        char temp;
        int rec = recv(Player1, &temp, 1, 0);
        if (rec <= 0) break;        
        input1=temp;  
        ready = true;
        std::cout << "\n[PLAYER 1] READY\n" << std::endl;
    }
}

void RecordInput_2(std::atomic<char>& input2, int Player2, std::atomic<bool>& ready)
{
    while (true) {
        char temp;
        int rec = recv(Player2, &temp, 1, 0);
        if (rec <= 0) break;
        input2 = temp;
        ready = true;
        std::cout << "\n[PLAYER 2] READY\n" << std::endl;  
    }
}
int main()
{


    char ServerAddress[INET6_ADDRSTRLEN];

    struct addrinfo hints;
    struct addrinfo* Results;
    
    memset(&hints,0,sizeof hints);

    hints.ai_family=AF_INET;
    hints.ai_flags=AI_PASSIVE;
    hints.ai_socktype=SOCK_STREAM; // should use dgram maybe but this isnt an fps game the server handles all logic 

    int status;

    status=getaddrinfo(NULL,"8080",&hints,&Results);
    if(status){std::cerr<<gai_strerror(status);exit(-1);}

    int ServerSocket=socket(Results->ai_family,Results->ai_socktype,Results->ai_protocol);
    if(ServerSocket==-1){std::perror("Socker call failed: ");exit(-1);}

    int yes=1;
    if(setsockopt(ServerSocket,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(yes)) == -1 ) {std::perror("setSockpot: ");exit(-1);}


    status=bind(ServerSocket,Results->ai_addr,Results->ai_addrlen);

    if(status==-1){std::perror("Bind call failed: ");exit(-1);}
    struct sockaddr_in* faaaah=(sockaddr_in*)(Results->ai_addr);
    inet_ntop(AF_INET,&faaaah->sin_addr,ServerAddress,sizeof(ServerAddress));
    std::cout<<"listening on "<<ServerAddress<<std::endl;

    listen(ServerSocket,2);

    struct sockaddr_storage Player1_addr;
    socklen_t Player1_addr_size=sizeof(Player1_addr);
    struct sockaddr_storage Player2_addr;
    socklen_t Player2_addr_size=sizeof(Player2_addr);



    int Player1=accept(ServerSocket,(struct sockaddr *)&Player1_addr,&Player1_addr_size);
    int Player2=accept(ServerSocket,(struct sockaddr *)&Player2_addr,&Player2_addr_size);
    std::cout<<"Clients connected"<<std::endl;

    //send booleans to player1 and 2
    
    Game* game = Game::getGameInstance();
    
     std::atomic<char> input_1{'\0'};
     std::atomic<char> input_2{'\0'};
     std::atomic<bool> ready1{false};
     std::atomic<bool> ready2{false};




    // char input_1{'\0'};
    // char input_2{'\0'};
    // bool ready1{false};
    // bool ready2=false;

    game->sendPlayerBooleans(Player1,Player2);
    
    std::thread worker1(RecordInput_1,std::ref(input_1),Player1,std::ref(ready1));
    std::thread worker2(RecordInput_2,std::ref(input_2),Player2,std::ref(ready2));
    
    while(true)
    {
        
        //char input;
        if(ready1){ std::cout << "\n[PLAYER 1] READY IN MAIN\n" << std::endl;}
        if(ready2){ std::cout << "\n[PLAYER 2] READY IN MAIN\n" << std::endl;}


        game->sendGameArr(Player1,Player2);
        std::cout<<"Waiting for the clients"<<std::endl;
        
        if (ready1) {
            game->processTurn(input_1, true);
            ready1 = false;
        }
        if (ready2) {
            game->processTurn(input_2, false);
            ready2 = false;
        }

    }

    worker1.join();
    worker2.join();
}
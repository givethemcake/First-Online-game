#include<netdb.h>
#include<sys/socket.h>
#include<iostream>
#include<sys/types.h>
#include<stdio.h>
#include<arpa/inet.h>
#include <termios.h>
#include <unistd.h>
#include <cstdio>
#include<thread>
#include<string.h>
using namespace std;


//only needs game arr and player boolean



char getch()
{
    termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    char c = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return c;
}


struct state
{
    bool AmPlayer1=false;
    char gameArr[25][25];//fixing it for now
    bool PlayerOneTurn=false;
};


bool recieve(int sock,struct state* fill,bool PlayerToFill=false)//fill the passed state struct
{

    int offset=1;//skip boolean
    if(PlayerToFill==true) offset=0;
    int bytes=sizeof(*fill)-offset;
    int bytesRecieved=0;
    

    while(bytesRecieved<bytes)
    {
        int sent=recv(sock,(char*)fill+bytesRecieved+offset,sizeof(*fill)-bytesRecieved-offset,0);
        bytesRecieved+=sent;
        if (sent == -1) { perror("recv"); return false; }
        if(sent==0){perror("Server Closed the Connection");return false;}

    }
    //cout<<"I returned"<<endl;
    return true;
}


void printThisBullshit(const struct state& re)
{
    
    std::cout << "\033[2J\033[1;1H";
        
        for(int i=0;i<25;i++)
        {
            for(int j=0;j<25;j++)
            {
                cout<<re.gameArr[i][j];
            }
            cout<<"\n";
        }     //this always needs to print regardless of if the server is sending data or not
}



void mkcISchutiyapaKaProjectKi(int sock,struct state* fill,bool PlayerToFill=false)
{
    printThisBullshit(*fill);
    struct state newstate =*fill;
    while(recieve(sock,&newstate))
    if(memcmp(&newstate,fill,sizeof(*fill)) ==0 ) continue;
    else {printThisBullshit(newstate);
          *fill=newstate;
         }
}


int main(int argc,char* argv[])
{

    if(argc==1){perror("Enter a god dammed ipv4 address you fucktard");exit(-1);}

    int Sock=socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in fuckthis;
    fuckthis.sin_port=htons(8080);
    fuckthis.sin_family=AF_INET;
    inet_pton(AF_INET,argv[1],&fuckthis.sin_addr);

    if(connect(Sock,(sockaddr*)&fuckthis,sizeof(fuckthis))==-1)
    {
        perror("Connect ki mkc");
        exit(-1);
    }else cout<<"Connected To server"<<endl;

    struct state currState;
    recieve(Sock,&currState,true);

  

    thread worker1(mkcISchutiyapaKaProjectKi,Sock,&currState,false);
   int input;
    while(1){
      
        input=getch();
        send(Sock,&input,sizeof(char),0);     
    }
}
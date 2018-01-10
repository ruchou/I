#include <winsock2.h>
#include <stdio.h>
#include <cstdio>
#include <iostream>
#include <string>
#include <sstream>

#include "Account.h"
#include "BankSystem.h"


using namespace std;

#define MAX_SIZE 2048
#define MY_ERROR(s) printf(s); system("PAUSE"); exit(1);
#define SERVER_PORT 9999

int main(int argc, char **argv)
{
    SOCKET serverSocket, clientSocket; // create a socket
    struct sockaddr_in serverAddress, clientAddress; // sockaddr_in：IP4 格式使用 ,  sockaddr_in6：IP6 格式使用 , sockaddr：通用格式
    int clientAddressLen;
    int bytesRead;
    char buf[MAX_SIZE];

    stringstream ss;





    if (argc != 2)
    {
        printf("wrong arguments! Please run ./server.out <port> \n");
        return -1;
    }
    else
    {
        cout<<"port"<<atoi(argv[1])<<endl;
    }


    // call WSAStartup first for Winsock
    WSADATA wsadata;
    if( WSAStartup(MAKEWORD(2,2),(LPWSADATA)&wsadata) != 0)   // ( version of winsock )
    {
        MY_ERROR("Winsock Error\n");
    }

    serverSocket = socket(PF_INET, SOCK_STREAM, 0); // (address , type , protocal(0表示不強制) )

    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port =  htons((uint16_t) atoi(argv[1]));//converts a u_short from host to TCP/IP network byte order



    if( bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
        MY_ERROR("Bind Error\n");
    }

    if( listen(serverSocket, 3) < 0)
    {
        MY_ERROR("Listen Error\n");
    }



    while(1)
    {
        printf("Waiting...\n");
        clientAddressLen = sizeof(clientAddress);
        clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLen);
        printf("Client IP is : %s \n", inet_ntoa(clientAddress.sin_addr));
        string command;
        command="\nwelcome to Bank";
        command+="\nlogin(l)";
        command+="\ncreate(c)\n";
        send(clientSocket,command.c_str(), command.size(), 0);

        command="\n------------------------------\n";
        command+="\nlogin(l)";
        command+="\ncreate(c)\n";
        command+="exit(exit)\n";


        string user_name;



        again:

        BankSystem nthu;


        while( (bytesRead = recv(clientSocket, buf, MAX_SIZE, 0))>0  )
        {
            buf[bytesRead] = '\0';
            if(!strcmp(buf,"exit"))
            {
                nthu.update();
                cout<<"Client Want to leave"<<endl;
                send(clientSocket,"Bye Bye",sizeof("Bye Bye"),0);
                command="\nwelcome to Bank";
                command+="\nlogin(l)";
                command+="\ncreate(c)\n";
                break;
            }
            else if(!strcmp(buf,"c"))
            {
                string str="enter your name";
                send(clientSocket,str.c_str(),str.size(), 0);
                printf("send %d byte(s) to user to ask for the account name: %s\n",str.size(),str.c_str());

                if((bytesRead = recv(clientSocket, buf, MAX_SIZE, 0))>0){
                    cout<<"recieve user account name "<<buf<<endl;
                    buf[bytesRead] = '\0';
                    string name=buf;

                    if(nthu.findAccount(name).getName()==name){
                        str="The account name already exist\n";
                        str+="Press m to continue\n";
                        send(clientSocket,str.c_str(),str.size(), 0);
                        continue;
                    }

                    str="enter the password";
                    send(clientSocket,str.c_str(),str.size(), 0);
                    if((bytesRead = recv(clientSocket, buf, MAX_SIZE, 0))>0){
                        buf[bytesRead] = '\0';
                        string password=buf;
                        nthu.addCount(name,0,password);
                        str="The account is created\n";
                        str+="Press m to continue\n";
                        send(clientSocket,str.c_str(),str.size(), 0);
                        continue;

                    }





                }

            }
            else if(!strcmp(buf,"l"))
            {

                string str="enter your name to login";
                send(clientSocket,str.c_str(),str.size(), 0);

                printf("send %d byte(s) to user to ask for the account name to login : %s\n",str.size(),str.c_str());

                if((bytesRead = recv(clientSocket, buf, MAX_SIZE, 0))>0){
                    buf[bytesRead] = '\0';
                    cout<<"receive user account name "<<buf<<endl;
                    string name=buf;

                    if(nthu.findAccount(name).getName()==name){


                        str="enter the password";
                        send(clientSocket,str.c_str(),str.size(), 0);

                        if((bytesRead = recv(clientSocket, buf, MAX_SIZE, 0))>0){
                            buf[bytesRead] = '\0';
                            string password=buf;


                            if(nthu.authenticate(name,password)==true){
                                str="The authentication success\n";
                                str+="Your current money: ";
                                user_name=name;
                                int money=nthu.findAccount(user_name).getMoney();
                                char s[12];
                                sprintf(s, "%d",money);
                                str+=s;
                                str+="\n";
                                str+="Press m to continue\n";



                                send(clientSocket,str.c_str(),str.size(), 0);


                                command="\nDeposite(d)\n";
                                command+="Withdraw(w)\n";
                                command+="Check Info(i)\n";
                                command+="Exit(exit)\n";
                                continue;

                            }else{

                                str="The authentication fail\n";
                                str+="Press m to continue\n";
                                send(clientSocket,str.c_str(),str.size(), 0);
                                continue;



                            }



                        }






                    }else{
                        string str="No such account\n";
                        str+="Press m to continue to create a new account\n";
                        send(clientSocket,str.c_str(),str.size(), 0);
                        continue;

                    }



                }


            }else if(!strcmp(buf,"d")){
                string str="enter the amount of money to deposit";
                send(clientSocket,str.c_str(),str.size(), 0);
                printf("send %d byte(s) to user to ask for the amount of money to deposit: %s\n",str.size(),str.c_str());

                if((bytesRead = recv(clientSocket, buf, MAX_SIZE, 0))>0){
                    buf[bytesRead] = '\0';
                    int money;
                    money=atoi(buf);
                    cout<<"recieve user money to deposit "<<money<<endl;


                    int remaining_money;
                    remaining_money=nthu.deposit(user_name,money);
                    string str="The money you have:";
                    string m;
                    cout<<"the remaining money"<<remaining_money<<endl;

                    char s[12];
                    sprintf(s, "%d", remaining_money);
                    str+=s;
                    str+="\npress m to next\n";

                    send(clientSocket,str.c_str(),str.size(), 0);
                    printf("send %d byte(s) to user to show money after deposit: %s\n",str.size(),str.c_str());

                    continue;

                }

            }else if(!strcmp(buf,"w")){

                string str="enter the amount of money to withdraw";
                send(clientSocket,str.c_str(),str.size(), 0);
                printf("send %d byte(s) to user to ask for the amount of money to withdraw: %s\n",str.size(),str.c_str());

                if((bytesRead = recv(clientSocket, buf, MAX_SIZE, 0))>0){
                    buf[bytesRead] = '\0';
                    int money=atoi(buf);
                    cout<<"recieve user money to withdraw "<<money<<endl;

                    int remaining_money;
                    remaining_money=nthu.withdraw(user_name,money);

                    if(remaining_money>=0){
                        string str="The money you have:";
                        string m;

                        char s[12];
                        sprintf(s, "%d", remaining_money);
                        str+=s;
                        str+="\npress m to next\n";

                        send(clientSocket,str.c_str(),str.size(), 0);
                        printf("send %d byte(s) to user to show money after withdraw: %s\n",str.size(),str.c_str());

                    }else{
                        string str="you do not have enough money\n";
                         str+="press m to next\n";

                        send(clientSocket,str.c_str(),str.size(), 0);
                        printf("send %d byte(s) to user to show money after withdraw: %s\n",str.size(),str.c_str());



                    }

                    continue;



                }


            }else if(!strcmp(buf,"i")){
                string name="Account Info: ";
                name+=nthu.findAccount(user_name).getName();
                int money=nthu.findAccount(user_name).getMoney();
                char s[12];
                sprintf(s, "%d",money);
                name+=" ";
                name+=s;
                cout<<"Check account info"<<name<<endl;

                name+="\nPress m to continue\n";
                send(clientSocket,name.c_str(),name.size(), 0);

                continue;

            }else if(!strcmp(buf,"m")){
                //send(clientSocket," ", command.size(), 0);
               // continue;
            }else{

                string str="No such command\n";
                str+="Press m to continue\n";
                send(clientSocket,str.c_str(),str.size(),0);
                continue;


            }






            send(clientSocket, command.c_str(), command.size(), 0);
            printf("send request %d byte(s): %s\n", command.size(), command.c_str());

            nthu.print();

        }

        if(bytesRead<0)
        {
            goto again; // ignore EINTR
        }

        closesocket(clientSocket);
    }

    return 0;
}

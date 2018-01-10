//
//  BankSystem.h
//  networklab1
//
//  Created by 周延儒 on 28/11/2017.
//  Copyright © 2017 周延儒. All rights reserved.
//
#include <iostream>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <set>
#include <string>

#include "Account.h"

using namespace std;


#ifndef BankSystem_h
#define BankSystem_h

class BankSystem {
private:
    Account write_data,read_data;
    vector<Account>list;

public:
    BankSystem(){


        ifstream infile;
        infile.open ("BankData.txt", ios::in|ios::binary);
        string name;
        int money;
        string password;
        while (infile>>name>>money>>password ) {
                Account account(name,money,password);
                list.push_back(account);
        }
        infile.close();


   /*
        FILE *file = fopen("BankData.txt", "rb");
        if (file != NULL) {

            while (fread(&account, sizeof(account), 1, file) != 0) {
                list.push_back(account);
            }
            fclose(file);
        } else {
        }
        */


    }

    void addCount(string name, int money,string code){
        Account tmp=findAccount(name);
        if(tmp.getName()!=name){
            tmp.get_data();
            list.push_back(Account(name,money,code));
        }
    }

    Account findAccount(string name){
        for(auto c:list ){
            if(c.getName()==name){
                cout<<"Accout exist"<<endl;
                return c;
            }
        }
        return Account("NULL");
    }

    bool authenticate(string name , string code){
        Account tmp=findAccount(name);
        if(tmp.getName()==name){

            if(tmp.checkPassword(code)==true)
                return true;
            else
                return false;
        }

        return false;

    }

    int setMoney(string name, int money){

        for (auto i = list.begin(); i != list.end(); i++)
        {
            if(i->getName()==name){
                i->setMoney(money);
                return money;
            }


        }

        return -1;

    }

    int deposit(string name,int money){
        for (auto i = list.begin(); i != list.end(); i++)
        {
            if(i->getName()==name){
                int cur=i->getMoney();
                i->setMoney(cur+money);
                return cur+money;
            }


        }

        return -1;

    }

    int withdraw(string name,int money){
        for (auto i = list.begin(); i != list.end(); i++)
        {
            if(i->getName()==name){
                int cur=i->getMoney();
                if(cur<money)return -1;
                i->setMoney(cur-money);
                return cur-money;
            }


        }

        return -1;

    }

    void print(){
        cout<<"Client Data"<<endl;
        cout<<"---------------------------"<<endl;
        for (int i=0; i<list.size(); i++) {
            cout<<list[i].getName()<<"--"<<list[i].getMoney()<<endl;

        }



    }


    void update(){

        fstream outfile;
        outfile.open("BankData.txt",ios::binary|ios::trunc|ios::out);


        for(auto c:list){
            outfile<<c.getName()<<' '<<c.getMoney()<<' '<<c.getPassword()<<' ';
        }
        outfile.close();



    }






};



#endif /* BankSystem_h */

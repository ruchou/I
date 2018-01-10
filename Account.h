//
//  Account.h
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

using namespace std;





#ifndef Account_h
#define Account_h

class Account {
private:
    int money;
    string name;
    string password;
public:
    Account(){
        money=0;
        name="Null";
    }
    Account(string name){
        name=name;

    }
    Account(string name, int money){
        this->name=name;
        this->money=money;

    }
    Account(string name, int money,string code){
        this->name=name;
        this->money=money;
        this->password=code;
    }


    void get_data()
    {
        cout << money << endl;
        cout << name << endl;
    }
    void set_data(int arg_money, string arg_name)
    {
        money = arg_money;
        name=arg_name;
    }
    string getName(){
        return name;
    }
    void setName(string str){
        this->name=str;

    }
    int getMoney(){
        return this->money;
    }
    void setMoney(int money){
        this->money=money;
    }

    bool checkPassword(string str){
        return (password==str?true:false);
    }
    void changePassword(string str){
        this->password=str;
    }
    string getPassword(){
        return password;
    }


};


#endif /* Account_h */

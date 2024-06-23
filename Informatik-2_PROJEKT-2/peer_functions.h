#pragma once


//hello
int start_(bool debug); 
std::string floatToString(float num);
float stringToFloat(std::string str);
bool check_INFO2CONNECT(std::string text, float version);
bool check_INFO2OK(std::string text);
std::string checkBACKCONNECT(std::string text);
int createMessageID();
int get_ID(std::string input);
std::string get_message(std::string input);
std::string cutEmptySpaces(std::string input);
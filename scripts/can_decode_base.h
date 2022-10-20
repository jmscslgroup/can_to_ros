#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <bitset>
#include <math.h>
#include <ctime>


typedef struct values_struct
{
   float var1,var2,var3,var4,var5,var6,var7,var8,var9,var10,var11,var12,var13,var14,var15,var16,var17,var18,var19,var20,var21,var22,var23,var24;
   string choice_var3;
} values;

class decode_msgs{
public:
values decode_message( unsigned int msg_id, std::string msg); // decoding CAN messages
std::string findTwosComplement(std::string str); // functon to return two's complement

};

std::string decode_msgs::findTwosComplement(std::string str) {
    int n = str.length();

    // Traverse the string to get first '1' from
    // the last of string
    int i;
    for (i = n-1 ; i >= 0 ; i--)
        if (str[i] == '1')
            break;

    // If there exists no '1' concatenate 1 at the
    // starting of string
    if (i == -1)
        return '1' + str;

    // Continue traversal after the position of
    // first '1'
    for (int k = i-1 ; k >= 0; k--)
    {
        //Just flip the values
        if (str[k] == '1')
            str[k] = '0';
        else
            str[k] = '1';
    }
    // return the modified string
    return str;
}

values decode_msgs::decode_message( unsigned int msg_id, std::string msg){
  values returnedVal;
  unsigned long long int n;
  //std::string binary;
  std::string raw;
  std::string raw2;
  std::string raw3;
  std::string raw4;
  std::string raw5;
  std::string raw6;
  std::string raw7;
  std::string raw8;
  std::string raw9;
  std::string raw10;
  std::string raw11;
  std::string raw12;
  std::string raw13;
  std::string raw14;
  std::string raw15;
  std::string raw16;
  std::string raw17;
  std::string raw18;
  std::string raw19;
  std::string raw20;
  std::string raw21;
  std::string raw22;
  std::string raw23;
  std::string raw24;
  std::string rawVal;
  float scaled;
  float conversion;
  unsigned long long raw_dec;
  returnedVal.var1=0.0;
  returnedVal.var2=0.0;
  returnedVal.var3=0.0;
  returnedVal.var4=0.0;
  returnedVal.var5=0.0;
  returnedVal.var6=0.0;
  returnedVal.var7=0.0;
  returnedVal.var8=0.0;
  returnedVal.var9=0.0;
  returnedVal.var10=0.0;
  returnedVal.var11=0.0;
  returnedVal.var12=0.0;
  returnedVal.var13=0.0;
  returnedVal.var14=0.0;
  returnedVal.var15=0.0;
  returnedVal.var16=0.0;
  returnedVal.var17=0.0;
  returnedVal.var18=0.0;
  returnedVal.var19=0.0;
  returnedVal.var20=0.0;
  returnedVal.var21=0.0;
  returnedVal.var22=0.0;
  returnedVal.var23=0.0;
  returnedVal.var24=0.0;
  returnedVal.choice_var3="";

  int len = msg.length();
  int i = 0;
  std::string word;
  std::string binary;
  while (i < len){
      while (i < len-16){
          // std::cout << msg.substr(i,16) << '\n';
          word = msg.substr(i,16);
          std::istringstream(word) >> std::hex >> n;
          // std::cout << std::dec << "Parsing word gives " << n << '\n';
          // std::cout << std::bitset<64>{n} << '\n';
          binary.append(std::bitset<64>{n}.to_string());
          i = i + 16;
      }
      // std::cout << msg.substr(i,1) << '\n';
      word = msg.substr(i,1);
      std::istringstream(word) >> std::hex >> n;
      // std::cout << std::dec << "Parsing word gives " << n << '\n';
      // std::cout << std::bitset<4>{n} << '\n';
      binary.append(std::bitset<4>{n}.to_string());

      i = i+1;
  }

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
   float var1,var2,var3,var4,var5,var6,var7,var8,var9,var10,var11,var12,var13,var14,var15,var16,var17,var18,var19,var20;
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
  std::string rawVal;
  float scaled;
  float conversion;
  unsigned long long raw_dec;
  returnedVal.var1=0.0;
  returnedVal.var2=0.0;
  returnedVal.var3=0.0;
  returnedVal.var4=0.0;

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
if (msg_id == 139){
	std::string raw1 = binary.substr(48,16); //STEER_ANGLE
	raw_dec = std::stoull(raw1, 0, 2);
	scaled = (float)raw_dec * 0.050000 + 0.000000;
	returnedVal.var1 = scaled;

	return returnedVal;
}
if (msg_id == 303){
	std::string raw1 = binary.substr(74,12); //SPEED
	raw_dec = std::stoull(raw1, 0, 2);
	scaled = (float)raw_dec * 0.050000 + 0.000000;
	conversion = ceilf((scaled* (10.0/36.0)) * 100) / 100;
	returnedVal.var1 = conversion;

	return returnedVal;
}
if (msg_id == 308){
	std::string raw1 = binary.substr(309,5); //CRUISE_STATE
	raw_dec = std::stoull(raw1, 0, 2);
	scaled = (float)raw_dec * 1.000000 + 0.000000;
	returnedVal.var1 = scaled;

	return returnedVal;
}
if (msg_id == 1119){
	std::string raw1 = binary.substr(58,3); //ACC_BTNS
	raw_dec = std::stoull(raw1, 0, 2);
	scaled = (float)raw_dec * 1.000000 + 0.000000;
	returnedVal.var1 = scaled;

	std::string raw2 = binary.substr(48,1); //LKAS_LDW_ON
	raw_dec = std::stoull(raw2, 0, 2);
	scaled = (float)raw_dec * 1.000000 + 0.000000;
	returnedVal.var2 = scaled;

	return returnedVal;
}
if (msg_id == 1487){
	std::string raw1 = binary.substr(153,7); //SET_SPEED
	raw_dec = std::stoull(raw1, 0, 2);
	scaled = (float)raw_dec * 1.000000 + 0.000000;
	returnedVal.var1 = scaled;

	std::string raw2 = binary.substr(137,1); //LANE_LINES
	raw_dec = std::stoull(raw2, 0, 2);
	scaled = (float)raw_dec * 1.000000 + 0.000000;
	returnedVal.var2 = scaled;

	std::string raw3 = binary.substr(280,4); //STATE_SIGNAL
	raw_dec = std::stoull(raw3, 0, 2);
	scaled = (float)raw_dec * 1.000000 + 0.000000;
	returnedVal.var3 = scaled;

	std::string raw4 = binary.substr(161,2); //SET_DISTANCE
	raw_dec = std::stoull(raw4, 0, 2);
	scaled = (float)raw_dec * 1.000000 + 0.000000;
	returnedVal.var4 = scaled;

	std::string raw5 = binary.substr(182,5); //HUD_STATES
	raw_dec = std::stoull(raw5, 0, 2);
	scaled = (float)raw_dec * 1.000000 + 0.000000;
	returnedVal.var5 = scaled;

	std::string raw6 = binary.substr(89,1); //SYSTEM_ON
	raw_dec = std::stoull(raw6, 0, 2);
	scaled = (float)raw_dec * 1.000000 + 0.000000;
	returnedVal.var6 = scaled;

	return returnedVal;
}
	return returnedVal;
}

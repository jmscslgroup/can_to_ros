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
if (msg_id == 381){
	std::string raw1 = binary.substr(56,7); //REL_ACCEL1
	raw_dec = std::stoull(raw1, 0, 2);
	scaled = (float)raw_dec * 1.000000 + 0.000000;
	returnedVal.var1 = scaled;

	std::string raw2 = binary.substr(128,6); //REL_ACCEL2
	raw_dec = std::stoull(raw2, 0, 2);
	scaled = (float)raw_dec * 1.000000 + 0.000000;
	returnedVal.var2 = scaled;

	std::string raw3 = binary.substr(200,6); //REL_ACCEL3
	raw_dec = std::stoull(raw3, 0, 2);
	scaled = (float)raw_dec * 1.000000 + 0.000000;
	returnedVal.var3 = scaled;

	std::string raw4 = binary.substr(64,12); //FOURTH1
	raw_dec = std::stoull(raw4, 0, 2);
	scaled = (float)raw_dec * 1.000000 + 0.000000;
	returnedVal.var4 = scaled;

	std::string raw5 = binary.substr(135,13); //FOURTH2
	raw_dec = std::stoull(raw5, 0, 2);
	scaled = (float)raw_dec * 1.000000 + 0.000000;
	returnedVal.var5 = scaled;

	std::string raw6 = binary.substr(207,13); //FOURTH3
	raw_dec = std::stoull(raw6, 0, 2);
	scaled = (float)raw_dec * 1.000000 + 0.000000;
	returnedVal.var6 = scaled;

	std::string raw7 = binary.substr(85,12); //LEAD_DIST1
	raw_dec = std::stoull(raw7, 0, 2);
	scaled = (float)raw_dec * 1.000000 + -4096.000000;
	returnedVal.var7 = scaled;

	std::string raw8 = binary.substr(157,12); //LEAD_DIST2
	raw_dec = std::stoull(raw8, 0, 2);
	scaled = (float)raw_dec * 1.000000 + -4096.000000;
	returnedVal.var8 = scaled;

	std::string raw9 = binary.substr(229,12); //LEAD_DIST3
	raw_dec = std::stoull(raw9, 0, 2);
	scaled = (float)raw_dec * 1.000000 + -4096.000000;
	returnedVal.var9 = scaled;

	std::string raw10 = binary.substr(100,11); //REL_VEL1
float REL_VEL1_fl;
        	if (raw10[0] == '0'){
        	REL_VEL1_fl= std::stoul( raw10, 0, 2 );
	}
	else {
        	REL_VEL1_fl = std::stoul(findTwosComplement(raw10), 0, 2);
        	REL_VEL1_fl = REL_VEL1_fl * -1.0;
        	}
	scaled = REL_VEL1_fl * 0.200000 + 2.000000;
	returnedVal.var10 = scaled;

	std::string raw11 = binary.substr(170,13); //REL_VEL2
	raw_dec = std::stoull(raw11, 0, 2);
	scaled = (float)raw_dec * 1.000000 + 0.000000;
	returnedVal.var11 = scaled;

	std::string raw12 = binary.substr(242,13); //REL_VEL3
	raw_dec = std::stoull(raw12, 0, 2);
	scaled = (float)raw_dec * 1.000000 + 0.000000;
	returnedVal.var12 = scaled;

	std::string raw13 = binary.substr(123,1); //BUTTON_PRESS0
	raw_dec = std::stoull(raw13, 0, 2);
	scaled = (float)raw_dec * 1.000000 + 0.000000;
	returnedVal.var13 = scaled;

	std::string raw14 = binary.substr(195,1); //BUTTON_PRESS1
	raw_dec = std::stoull(raw14, 0, 2);
	scaled = (float)raw_dec * 1.000000 + 0.000000;
	returnedVal.var14 = scaled;

	std::string raw15 = binary.substr(267,1); //BUTTON_PRESS2
	raw_dec = std::stoull(raw15, 0, 2);
	scaled = (float)raw_dec * 1.000000 + 0.000000;
	returnedVal.var15 = scaled;

	return returnedVal;
}
if (msg_id == 771){
	std::string raw1 = binary.substr(128,6); //SONAR1
	raw_dec = std::stoull(raw1, 0, 2);
	scaled = (float)raw_dec * 0.083300 + 0.000000;
	returnedVal.var1 = scaled;

	std::string raw2 = binary.substr(136,6); //SONAR2
	raw_dec = std::stoull(raw2, 0, 2);
	scaled = (float)raw_dec * 0.083300 + 0.000000;
	returnedVal.var2 = scaled;

	std::string raw3 = binary.substr(144,6); //SONAR3
	raw_dec = std::stoull(raw3, 0, 2);
	scaled = (float)raw_dec * 0.083300 + 0.000000;
	returnedVal.var3 = scaled;

	std::string raw4 = binary.substr(152,6); //SONAR4
	raw_dec = std::stoull(raw4, 0, 2);
	scaled = (float)raw_dec * 0.083300 + 0.000000;
	returnedVal.var4 = scaled;

	std::string raw5 = binary.substr(160,6); //SONAR5
	raw_dec = std::stoull(raw5, 0, 2);
	scaled = (float)raw_dec * 0.083300 + 0.000000;
	returnedVal.var5 = scaled;

	std::string raw6 = binary.substr(168,6); //SONAR6
	raw_dec = std::stoull(raw6, 0, 2);
	scaled = (float)raw_dec * 0.083300 + 0.000000;
	returnedVal.var6 = scaled;

	std::string raw7 = binary.substr(216,6); //SONAR7
	raw_dec = std::stoull(raw7, 0, 2);
	scaled = (float)raw_dec * 0.083300 + 0.000000;
	returnedVal.var7 = scaled;

	std::string raw8 = binary.substr(224,6); //SONAR8
	raw_dec = std::stoull(raw8, 0, 2);
	scaled = (float)raw_dec * 0.083300 + 0.000000;
	returnedVal.var8 = scaled;

	std::string raw9 = binary.substr(232,6); //SONAR9
	raw_dec = std::stoull(raw9, 0, 2);
	scaled = (float)raw_dec * 0.083300 + 0.000000;
	returnedVal.var9 = scaled;

	std::string raw10 = binary.substr(240,6); //SONAR10
	raw_dec = std::stoull(raw10, 0, 2);
	scaled = (float)raw_dec * 0.083300 + 0.000000;
	returnedVal.var10 = scaled;

	std::string raw11 = binary.substr(248,6); //SONAR11
	raw_dec = std::stoull(raw11, 0, 2);
	scaled = (float)raw_dec * 0.083300 + 0.000000;
	returnedVal.var11 = scaled;

	std::string raw12 = binary.substr(256,6); //SONAR12
	raw_dec = std::stoull(raw12, 0, 2);
	scaled = (float)raw_dec * 0.083300 + 0.000000;
	returnedVal.var12 = scaled;

	std::string raw13 = binary.substr(304,6); //SONAR13
	raw_dec = std::stoull(raw13, 0, 2);
	scaled = (float)raw_dec * 0.083300 + 0.000000;
	returnedVal.var13 = scaled;

	std::string raw14 = binary.substr(312,6); //SONAR14
	raw_dec = std::stoull(raw14, 0, 2);
	scaled = (float)raw_dec * 0.083300 + 0.000000;
	returnedVal.var14 = scaled;

	std::string raw15 = binary.substr(320,6); //SONAR15
	raw_dec = std::stoull(raw15, 0, 2);
	scaled = (float)raw_dec * 0.083300 + 0.000000;
	returnedVal.var15 = scaled;

	std::string raw16 = binary.substr(328,6); //SONAR16
	raw_dec = std::stoull(raw16, 0, 2);
	scaled = (float)raw_dec * 0.083300 + 0.000000;
	returnedVal.var16 = scaled;

	std::string raw17 = binary.substr(336,6); //SONAR17
	raw_dec = std::stoull(raw17, 0, 2);
	scaled = (float)raw_dec * 0.083300 + 0.000000;
	returnedVal.var17 = scaled;

	std::string raw18 = binary.substr(344,6); //SONAR18
	raw_dec = std::stoull(raw18, 0, 2);
	scaled = (float)raw_dec * 0.083300 + 0.000000;
	returnedVal.var18 = scaled;

	std::string raw19 = binary.substr(352,6); //SONAR19
	raw_dec = std::stoull(raw19, 0, 2);
	scaled = (float)raw_dec * 0.083300 + 0.000000;
	returnedVal.var19 = scaled;

	std::string raw20 = binary.substr(360,6); //SONAR20
	raw_dec = std::stoull(raw20, 0, 2);
	scaled = (float)raw_dec * 0.083300 + 0.000000;
	returnedVal.var20 = scaled;

	return returnedVal;
}
	return returnedVal;
}

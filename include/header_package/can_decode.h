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
   std::string choice_var1,choice_var2,choice_var3,choice_var4,choice_var5,choice_var6,choice_var7,choice_var8,choice_var9,choice_var10,choice_var11,choice_var12,choice_var13,choice_var14,choice_var15,choice_var16,choice_var17,choice_var18,choice_var19,choice_var20,choice_var21,choice_var22,choice_var23,choice_var24;
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
  returnedVal.choice_var1="";
  returnedVal.choice_var2="";
  returnedVal.choice_var3="";
  returnedVal.choice_var4="";
  returnedVal.choice_var5="";
  returnedVal.choice_var6="";
  returnedVal.choice_var7="";
  returnedVal.choice_var8="";
  returnedVal.choice_var9="";
  returnedVal.choice_var10="";
  returnedVal.choice_var11="";
  returnedVal.choice_var12="";
  returnedVal.choice_var13="";
  returnedVal.choice_var14="";
  returnedVal.choice_var15="";
  returnedVal.choice_var16="";
  returnedVal.choice_var17="";
  returnedVal.choice_var18="";
  returnedVal.choice_var19="";
  returnedVal.choice_var20="";
  returnedVal.choice_var21="";
  returnedVal.choice_var22="";
  returnedVal.choice_var23="";
  returnedVal.choice_var24="";

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
if (msg_id == 140){
	std::string raw1 = binary.substr(44,10); //ACCEL_PEDAL_POSITION
	raw_dec = std::stoull(raw1, 0, 2);
	scaled = (float)raw_dec * 1.000000 + 0.000000;
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

	std::string raw2 = binary.substr(67,12); //WHEEL_TORQUE_CMD
	raw_dec = std::stoull(raw2, 0, 2);
	scaled = (float)raw_dec * 1.000000 + 0.000000;
	returnedVal.var2 = scaled;

	std::string raw3 = binary.substr(173,10); //BRAKE_TORQUE_CMD
	raw_dec = std::stoull(raw3, 0, 2);
	scaled = (float)raw_dec * 1.000000 + 0.000000;
	returnedVal.var3 = scaled;

	return returnedVal;
}
if (msg_id == 1119){
	std::string raw1 = binary.substr(58,3); //ACC_BTNS
	raw_dec = std::stoull(raw1, 0, 2);
	scaled = (float)raw_dec * 1.000000 + 0.000000;
	returnedVal.var1 = scaled;

    if (int(scaled) == 0){
    	returnedVal.choice_var1 = "none";
    	}

    if (int(scaled) == 1){
    	returnedVal.choice_var1 = "resume";
    	}

    if (int(scaled) == 2){
    	returnedVal.choice_var1 = "set";
    	}

    if (int(scaled) == 3){
    	returnedVal.choice_var1 = "distance";
    	}

    if (int(scaled) == 4){
    	returnedVal.choice_var1 = "cancel";
    	}

    if (int(scaled) == 5){
    	returnedVal.choice_var1 = "system_toggle";
    	}

	return returnedVal;
}
if (msg_id == 1487){
	std::string raw1 = binary.substr(153,7); //SET_SPEED
	raw_dec = std::stoull(raw1, 0, 2);
	scaled = (float)raw_dec * 1.000000 + 0.000000;
	returnedVal.var1 = scaled;

	std::string raw2 = binary.substr(280,4); //STATE_SIGNAL
	raw_dec = std::stoull(raw2, 0, 2);
	scaled = (float)raw_dec * 1.000000 + 0.000000;
	returnedVal.var2 = scaled;

	std::string raw3 = binary.substr(161,2); //SET_DISTANCE
	raw_dec = std::stoull(raw3, 0, 2);
	scaled = (float)raw_dec * 1.000000 + 0.000000;
	returnedVal.var3 = scaled;

	std::string raw4 = binary.substr(182,5); //HUD_STATES
	raw_dec = std::stoull(raw4, 0, 2);
	scaled = (float)raw_dec * 1.000000 + 0.000000;
	returnedVal.var4 = scaled;

	std::string raw5 = binary.substr(137,1); //MINICAR
	raw_dec = std::stoull(raw5, 0, 2);
	scaled = (float)raw_dec * 1.000000 + 0.000000;
	returnedVal.var5 = scaled;

	return returnedVal;
}
	return returnedVal;
}

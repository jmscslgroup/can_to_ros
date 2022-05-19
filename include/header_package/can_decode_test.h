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
   float var1, var2, var3, var4;
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
  std::string binary;
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
if (msg_id == 139){
    	std::stringstream hex_ss(msg);
    	hex_ss >> std::hex >> n;// making the message hex
    	binary = std::bitset<384>(n).to_string(); // convert hex to binary

	std::string raw1 = binary.substr(48,16); //STEER_ANGLE
	//std::cout <<"THERE IS A MESSAGE";

	raw_dec = std::stoull(raw1, 0, 2);

	scaled = (float)raw_dec * 0.050000 + 0.000000;
	returnedVal.var1 = scaled;


	return returnedVal;
}
if (msg_id == 303){
    	std::stringstream hex_ss(msg);
    	hex_ss >> std::hex >> n;// making the message hex
    	binary = std::bitset<192>(n).to_string(); // convert hex to binary

	std::string raw1 = binary.substr(75,11); //SPEED
	std::cout <<raw1;

	raw_dec = std::stoull(raw1, 0, 2);
	scaled = (float)raw_dec * 0.050000 + 0.000000;
	conversion = ceilf((scaled* (10.0/36.0)) * 100) / 100;
	returnedVal.var1 = conversion;

	return returnedVal;
}
	return returnedVal;
}

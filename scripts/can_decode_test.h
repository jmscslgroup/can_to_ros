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
std::string findTwoscomplement(std::string str); // functon to return two's complement

};

std::string decode_msgs::findTwoscomplement(std::string str) {
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

  std::string rawVal;
  returnedVal.var1=0.0;
  returnedVal.var2=0.0;
  returnedVal.var3=0.0;
  returnedVal.var4=0.0;
if (msg_id == 37){
    	std::stringstream hex_ss(msg);
    	hex_ss >> std::hex >> n;// making the message hex
    	binary = std::bitset<{64}>(n).to_string(); // convert hex to binary

	std::string raw = binary.substr(4,16); //STEER_ANGLE
	if ((raw)[0] == '0'){
        	raw_dec= std::stoul( raw, 0, 2 );
	}
	else {
        	raw_dec = std::stoul(findTwosComplement(raw), 0, 2);
        	raw_dec = 	raw_dec * -1.0
	scaled = (float)raw_dec * 1.500000 + 0.000000;
	returnedVal.var1 = scaled;

	return returnedVal;
}
if (msg_id == 180){
    	std::stringstream hex_ss(msg);
    	hex_ss >> std::hex >> n;// making the message hex
    	binary = std::bitset<{64}>(n).to_string(); // convert hex to binary

	std::string raw = binary.substr(40,56); //SPEED
	raw_dec = std::stoull(raw, 0, 2);
	scaled = (float)raw_dec * 0.010000 + 0.000000;
	conversion = ceilf((scaled* (10.0/36.0)) * 100) / 100;
	returnedVal.var1 = conversion;

	return returnedVal;
}
if (msg_id == 869){
    	std::stringstream hex_ss(msg);
    	hex_ss >> std::hex >> n;// making the message hex
    	binary = std::bitset<{56}>(n).to_string(); // convert hex to binary

	std::string raw = binary.substr(32,40); //LEAD_DISTANCE
	raw_dec = std::stoull(raw, 0, 2);
	scaled = (float)raw_dec * 1.000000 + 0.000000;
	returnedVal.var1 = scaled;

	std::string raw = binary.substr(40,52); //REL_SPEED
	if ((raw)[0] == '0'){
        	raw_dec= std::stoul( raw, 0, 2 );
	}
	else {
        	raw_dec = std::stoul(findTwosComplement(raw), 0, 2);
        	raw_dec = 	raw_dec * -1.0
	scaled = (float)raw_dec * 0.001785 + 0.000000;
	returnedVal.var2 = scaled;

	return returnedVal;
}
if (msg_id == 384){
    	std::stringstream hex_ss(msg);
    	hex_ss >> std::hex >> n;// making the message hex
    	binary = std::bitset<{64}>(n).to_string(); // convert hex to binary

	std::string raw = binary.substr(8,21); //LONG_DIST
	raw_dec = std::stoull(raw, 0, 2);
	scaled = (float)raw_dec * 0.040000 + 0.000000;
	returnedVal.var1 = scaled;

	std::string raw = binary.substr(24,35); //LAT_DIST
	if ((raw)[0] == '0'){
        	raw_dec= std::stoul( raw, 0, 2 );
	}
	else {
        	raw_dec = std::stoul(findTwosComplement(raw), 0, 2);
        	raw_dec = 	raw_dec * -1.0
	scaled = (float)raw_dec * 0.040000 + 0.000000;
	returnedVal.var2 = scaled;

	std::string raw = binary.substr(40,52); //REL_SPEED
	if ((raw)[0] == '0'){
        	raw_dec= std::stoul( raw, 0, 2 );
	}
	else {
        	raw_dec = std::stoul(findTwosComplement(raw), 0, 2);
        	raw_dec = 	raw_dec * -1.0
	scaled = (float)raw_dec * 0.025000 + 0.000000;
	returnedVal.var3 = scaled;

	return returnedVal;
}
if (msg_id == 385){
    	std::stringstream hex_ss(msg);
    	hex_ss >> std::hex >> n;// making the message hex
    	binary = std::bitset<{64}>(n).to_string(); // convert hex to binary

	std::string raw = binary.substr(8,21); //LONG_DIST
	raw_dec = std::stoull(raw, 0, 2);
	scaled = (float)raw_dec * 0.040000 + 0.000000;
	returnedVal.var1 = scaled;

	std::string raw = binary.substr(24,35); //LAT_DIST
	if ((raw)[0] == '0'){
        	raw_dec= std::stoul( raw, 0, 2 );
	}
	else {
        	raw_dec = std::stoul(findTwosComplement(raw), 0, 2);
        	raw_dec = 	raw_dec * -1.0
	scaled = (float)raw_dec * 0.040000 + 0.000000;
	returnedVal.var2 = scaled;

	std::string raw = binary.substr(40,52); //REL_SPEED
	if ((raw)[0] == '0'){
        	raw_dec= std::stoul( raw, 0, 2 );
	}
	else {
        	raw_dec = std::stoul(findTwosComplement(raw), 0, 2);
        	raw_dec = 	raw_dec * -1.0
	scaled = (float)raw_dec * 0.025000 + 0.000000;
	returnedVal.var3 = scaled;

	return returnedVal;
}
if (msg_id == 386){
    	std::stringstream hex_ss(msg);
    	hex_ss >> std::hex >> n;// making the message hex
    	binary = std::bitset<{64}>(n).to_string(); // convert hex to binary

	std::string raw = binary.substr(8,21); //LONG_DIST
	raw_dec = std::stoull(raw, 0, 2);
	scaled = (float)raw_dec * 0.040000 + 0.000000;
	returnedVal.var1 = scaled;

	std::string raw = binary.substr(24,35); //LAT_DIST
	if ((raw)[0] == '0'){
        	raw_dec= std::stoul( raw, 0, 2 );
	}
	else {
        	raw_dec = std::stoul(findTwosComplement(raw), 0, 2);
        	raw_dec = 	raw_dec * -1.0
	scaled = (float)raw_dec * 0.040000 + 0.000000;
	returnedVal.var2 = scaled;

	std::string raw = binary.substr(40,52); //REL_SPEED
	if ((raw)[0] == '0'){
        	raw_dec= std::stoul( raw, 0, 2 );
	}
	else {
        	raw_dec = std::stoul(findTwosComplement(raw), 0, 2);
        	raw_dec = 	raw_dec * -1.0
	scaled = (float)raw_dec * 0.025000 + 0.000000;
	returnedVal.var3 = scaled;

	return returnedVal;
}
if (msg_id == 387){
    	std::stringstream hex_ss(msg);
    	hex_ss >> std::hex >> n;// making the message hex
    	binary = std::bitset<{64}>(n).to_string(); // convert hex to binary

	std::string raw = binary.substr(8,21); //LONG_DIST
	raw_dec = std::stoull(raw, 0, 2);
	scaled = (float)raw_dec * 0.040000 + 0.000000;
	returnedVal.var1 = scaled;

	std::string raw = binary.substr(24,35); //LAT_DIST
	if ((raw)[0] == '0'){
        	raw_dec= std::stoul( raw, 0, 2 );
	}
	else {
        	raw_dec = std::stoul(findTwosComplement(raw), 0, 2);
        	raw_dec = 	raw_dec * -1.0
	scaled = (float)raw_dec * 0.040000 + 0.000000;
	returnedVal.var2 = scaled;

	std::string raw = binary.substr(40,52); //REL_SPEED
	if ((raw)[0] == '0'){
        	raw_dec= std::stoul( raw, 0, 2 );
	}
	else {
        	raw_dec = std::stoul(findTwosComplement(raw), 0, 2);
        	raw_dec = 	raw_dec * -1.0
	scaled = (float)raw_dec * 0.025000 + 0.000000;
	returnedVal.var3 = scaled;

	return returnedVal;
}
if (msg_id == 388){
    	std::stringstream hex_ss(msg);
    	hex_ss >> std::hex >> n;// making the message hex
    	binary = std::bitset<{64}>(n).to_string(); // convert hex to binary

	std::string raw = binary.substr(8,21); //LONG_DIST
	raw_dec = std::stoull(raw, 0, 2);
	scaled = (float)raw_dec * 0.040000 + 0.000000;
	returnedVal.var1 = scaled;

	std::string raw = binary.substr(24,35); //LAT_DIST
	if ((raw)[0] == '0'){
        	raw_dec= std::stoul( raw, 0, 2 );
	}
	else {
        	raw_dec = std::stoul(findTwosComplement(raw), 0, 2);
        	raw_dec = 	raw_dec * -1.0
	scaled = (float)raw_dec * 0.040000 + 0.000000;
	returnedVal.var2 = scaled;

	std::string raw = binary.substr(40,52); //REL_SPEED
	if ((raw)[0] == '0'){
        	raw_dec= std::stoul( raw, 0, 2 );
	}
	else {
        	raw_dec = std::stoul(findTwosComplement(raw), 0, 2);
        	raw_dec = 	raw_dec * -1.0
	scaled = (float)raw_dec * 0.025000 + 0.000000;
	returnedVal.var3 = scaled;

	return returnedVal;
}
if (msg_id == 389){
    	std::stringstream hex_ss(msg);
    	hex_ss >> std::hex >> n;// making the message hex
    	binary = std::bitset<{64}>(n).to_string(); // convert hex to binary

	std::string raw = binary.substr(8,21); //LONG_DIST
	raw_dec = std::stoull(raw, 0, 2);
	scaled = (float)raw_dec * 0.040000 + 0.000000;
	returnedVal.var1 = scaled;

	std::string raw = binary.substr(24,35); //LAT_DIST
	if ((raw)[0] == '0'){
        	raw_dec= std::stoul( raw, 0, 2 );
	}
	else {
        	raw_dec = std::stoul(findTwosComplement(raw), 0, 2);
        	raw_dec = 	raw_dec * -1.0
	scaled = (float)raw_dec * 0.040000 + 0.000000;
	returnedVal.var2 = scaled;

	std::string raw = binary.substr(40,52); //REL_SPEED
	if ((raw)[0] == '0'){
        	raw_dec= std::stoul( raw, 0, 2 );
	}
	else {
        	raw_dec = std::stoul(findTwosComplement(raw), 0, 2);
        	raw_dec = 	raw_dec * -1.0
	scaled = (float)raw_dec * 0.025000 + 0.000000;
	returnedVal.var3 = scaled;

	return returnedVal;
}
if (msg_id == 390){
    	std::stringstream hex_ss(msg);
    	hex_ss >> std::hex >> n;// making the message hex
    	binary = std::bitset<{64}>(n).to_string(); // convert hex to binary

	std::string raw = binary.substr(8,21); //LONG_DIST
	raw_dec = std::stoull(raw, 0, 2);
	scaled = (float)raw_dec * 0.040000 + 0.000000;
	returnedVal.var1 = scaled;

	std::string raw = binary.substr(24,35); //LAT_DIST
	if ((raw)[0] == '0'){
        	raw_dec= std::stoul( raw, 0, 2 );
	}
	else {
        	raw_dec = std::stoul(findTwosComplement(raw), 0, 2);
        	raw_dec = 	raw_dec * -1.0
	scaled = (float)raw_dec * 0.040000 + 0.000000;
	returnedVal.var2 = scaled;

	std::string raw = binary.substr(40,52); //REL_SPEED
	if ((raw)[0] == '0'){
        	raw_dec= std::stoul( raw, 0, 2 );
	}
	else {
        	raw_dec = std::stoul(findTwosComplement(raw), 0, 2);
        	raw_dec = 	raw_dec * -1.0
	scaled = (float)raw_dec * 0.025000 + 0.000000;
	returnedVal.var3 = scaled;

	return returnedVal;
}
if (msg_id == 391){
    	std::stringstream hex_ss(msg);
    	hex_ss >> std::hex >> n;// making the message hex
    	binary = std::bitset<{64}>(n).to_string(); // convert hex to binary

	std::string raw = binary.substr(8,21); //LONG_DIST
	raw_dec = std::stoull(raw, 0, 2);
	scaled = (float)raw_dec * 0.040000 + 0.000000;
	returnedVal.var1 = scaled;

	std::string raw = binary.substr(24,35); //LAT_DIST
	if ((raw)[0] == '0'){
        	raw_dec= std::stoul( raw, 0, 2 );
	}
	else {
        	raw_dec = std::stoul(findTwosComplement(raw), 0, 2);
        	raw_dec = 	raw_dec * -1.0
	scaled = (float)raw_dec * 0.040000 + 0.000000;
	returnedVal.var2 = scaled;

	std::string raw = binary.substr(40,52); //REL_SPEED
	if ((raw)[0] == '0'){
        	raw_dec= std::stoul( raw, 0, 2 );
	}
	else {
        	raw_dec = std::stoul(findTwosComplement(raw), 0, 2);
        	raw_dec = 	raw_dec * -1.0
	scaled = (float)raw_dec * 0.025000 + 0.000000;
	returnedVal.var3 = scaled;

	return returnedVal;
}
if (msg_id == 392){
    	std::stringstream hex_ss(msg);
    	hex_ss >> std::hex >> n;// making the message hex
    	binary = std::bitset<{64}>(n).to_string(); // convert hex to binary

	std::string raw = binary.substr(8,21); //LONG_DIST
	raw_dec = std::stoull(raw, 0, 2);
	scaled = (float)raw_dec * 0.040000 + 0.000000;
	returnedVal.var1 = scaled;

	std::string raw = binary.substr(24,35); //LAT_DIST
	if ((raw)[0] == '0'){
        	raw_dec= std::stoul( raw, 0, 2 );
	}
	else {
        	raw_dec = std::stoul(findTwosComplement(raw), 0, 2);
        	raw_dec = 	raw_dec * -1.0
	scaled = (float)raw_dec * 0.040000 + 0.000000;
	returnedVal.var2 = scaled;

	std::string raw = binary.substr(40,52); //REL_SPEED
	if ((raw)[0] == '0'){
        	raw_dec= std::stoul( raw, 0, 2 );
	}
	else {
        	raw_dec = std::stoul(findTwosComplement(raw), 0, 2);
        	raw_dec = 	raw_dec * -1.0
	scaled = (float)raw_dec * 0.025000 + 0.000000;
	returnedVal.var3 = scaled;

	return returnedVal;
}
if (msg_id == 393){
    	std::stringstream hex_ss(msg);
    	hex_ss >> std::hex >> n;// making the message hex
    	binary = std::bitset<{64}>(n).to_string(); // convert hex to binary

	std::string raw = binary.substr(8,21); //LONG_DIST
	raw_dec = std::stoull(raw, 0, 2);
	scaled = (float)raw_dec * 0.040000 + 0.000000;
	returnedVal.var1 = scaled;

	std::string raw = binary.substr(24,35); //LAT_DIST
	if ((raw)[0] == '0'){
        	raw_dec= std::stoul( raw, 0, 2 );
	}
	else {
        	raw_dec = std::stoul(findTwosComplement(raw), 0, 2);
        	raw_dec = 	raw_dec * -1.0
	scaled = (float)raw_dec * 0.040000 + 0.000000;
	returnedVal.var2 = scaled;

	std::string raw = binary.substr(40,52); //REL_SPEED
	if ((raw)[0] == '0'){
        	raw_dec= std::stoul( raw, 0, 2 );
	}
	else {
        	raw_dec = std::stoul(findTwosComplement(raw), 0, 2);
        	raw_dec = 	raw_dec * -1.0
	scaled = (float)raw_dec * 0.025000 + 0.000000;
	returnedVal.var3 = scaled;

	return returnedVal;
}
if (msg_id == 394){
    	std::stringstream hex_ss(msg);
    	hex_ss >> std::hex >> n;// making the message hex
    	binary = std::bitset<{64}>(n).to_string(); // convert hex to binary

	std::string raw = binary.substr(8,21); //LONG_DIST
	raw_dec = std::stoull(raw, 0, 2);
	scaled = (float)raw_dec * 0.040000 + 0.000000;
	returnedVal.var1 = scaled;

	std::string raw = binary.substr(24,35); //LAT_DIST
	if ((raw)[0] == '0'){
        	raw_dec= std::stoul( raw, 0, 2 );
	}
	else {
        	raw_dec = std::stoul(findTwosComplement(raw), 0, 2);
        	raw_dec = 	raw_dec * -1.0
	scaled = (float)raw_dec * 0.040000 + 0.000000;
	returnedVal.var2 = scaled;

	std::string raw = binary.substr(40,52); //REL_SPEED
	if ((raw)[0] == '0'){
        	raw_dec= std::stoul( raw, 0, 2 );
	}
	else {
        	raw_dec = std::stoul(findTwosComplement(raw), 0, 2);
        	raw_dec = 	raw_dec * -1.0
	scaled = (float)raw_dec * 0.025000 + 0.000000;
	returnedVal.var3 = scaled;

	return returnedVal;
}
if (msg_id == 395){
    	std::stringstream hex_ss(msg);
    	hex_ss >> std::hex >> n;// making the message hex
    	binary = std::bitset<{64}>(n).to_string(); // convert hex to binary

	std::string raw = binary.substr(8,21); //LONG_DIST
	raw_dec = std::stoull(raw, 0, 2);
	scaled = (float)raw_dec * 0.040000 + 0.000000;
	returnedVal.var1 = scaled;

	std::string raw = binary.substr(24,35); //LAT_DIST
	if ((raw)[0] == '0'){
        	raw_dec= std::stoul( raw, 0, 2 );
	}
	else {
        	raw_dec = std::stoul(findTwosComplement(raw), 0, 2);
        	raw_dec = 	raw_dec * -1.0
	scaled = (float)raw_dec * 0.040000 + 0.000000;
	returnedVal.var2 = scaled;

	std::string raw = binary.substr(40,52); //REL_SPEED
	if ((raw)[0] == '0'){
        	raw_dec= std::stoul( raw, 0, 2 );
	}
	else {
        	raw_dec = std::stoul(findTwosComplement(raw), 0, 2);
        	raw_dec = 	raw_dec * -1.0
	scaled = (float)raw_dec * 0.025000 + 0.000000;
	returnedVal.var3 = scaled;

	return returnedVal;
}
if (msg_id == 396){
    	std::stringstream hex_ss(msg);
    	hex_ss >> std::hex >> n;// making the message hex
    	binary = std::bitset<{64}>(n).to_string(); // convert hex to binary

	std::string raw = binary.substr(8,21); //LONG_DIST
	raw_dec = std::stoull(raw, 0, 2);
	scaled = (float)raw_dec * 0.040000 + 0.000000;
	returnedVal.var1 = scaled;

	std::string raw = binary.substr(24,35); //LAT_DIST
	if ((raw)[0] == '0'){
        	raw_dec= std::stoul( raw, 0, 2 );
	}
	else {
        	raw_dec = std::stoul(findTwosComplement(raw), 0, 2);
        	raw_dec = 	raw_dec * -1.0
	scaled = (float)raw_dec * 0.040000 + 0.000000;
	returnedVal.var2 = scaled;

	std::string raw = binary.substr(40,52); //REL_SPEED
	if ((raw)[0] == '0'){
        	raw_dec= std::stoul( raw, 0, 2 );
	}
	else {
        	raw_dec = std::stoul(findTwosComplement(raw), 0, 2);
        	raw_dec = 	raw_dec * -1.0
	scaled = (float)raw_dec * 0.025000 + 0.000000;
	returnedVal.var3 = scaled;

	return returnedVal;
}
if (msg_id == 397){
    	std::stringstream hex_ss(msg);
    	hex_ss >> std::hex >> n;// making the message hex
    	binary = std::bitset<{64}>(n).to_string(); // convert hex to binary

	std::string raw = binary.substr(8,21); //LONG_DIST
	raw_dec = std::stoull(raw, 0, 2);
	scaled = (float)raw_dec * 0.040000 + 0.000000;
	returnedVal.var1 = scaled;

	std::string raw = binary.substr(24,35); //LAT_DIST
	if ((raw)[0] == '0'){
        	raw_dec= std::stoul( raw, 0, 2 );
	}
	else {
        	raw_dec = std::stoul(findTwosComplement(raw), 0, 2);
        	raw_dec = 	raw_dec * -1.0
	scaled = (float)raw_dec * 0.040000 + 0.000000;
	returnedVal.var2 = scaled;

	std::string raw = binary.substr(40,52); //REL_SPEED
	if ((raw)[0] == '0'){
        	raw_dec= std::stoul( raw, 0, 2 );
	}
	else {
        	raw_dec = std::stoul(findTwosComplement(raw), 0, 2);
        	raw_dec = 	raw_dec * -1.0
	scaled = (float)raw_dec * 0.025000 + 0.000000;
	returnedVal.var3 = scaled;

	return returnedVal;
}
if (msg_id == 398){
    	std::stringstream hex_ss(msg);
    	hex_ss >> std::hex >> n;// making the message hex
    	binary = std::bitset<{64}>(n).to_string(); // convert hex to binary

	std::string raw = binary.substr(8,21); //LONG_DIST
	raw_dec = std::stoull(raw, 0, 2);
	scaled = (float)raw_dec * 0.040000 + 0.000000;
	returnedVal.var1 = scaled;

	std::string raw = binary.substr(24,35); //LAT_DIST
	if ((raw)[0] == '0'){
        	raw_dec= std::stoul( raw, 0, 2 );
	}
	else {
        	raw_dec = std::stoul(findTwosComplement(raw), 0, 2);
        	raw_dec = 	raw_dec * -1.0
	scaled = (float)raw_dec * 0.040000 + 0.000000;
	returnedVal.var2 = scaled;

	std::string raw = binary.substr(40,52); //REL_SPEED
	if ((raw)[0] == '0'){
        	raw_dec= std::stoul( raw, 0, 2 );
	}
	else {
        	raw_dec = std::stoul(findTwosComplement(raw), 0, 2);
        	raw_dec = 	raw_dec * -1.0
	scaled = (float)raw_dec * 0.025000 + 0.000000;
	returnedVal.var3 = scaled;

	return returnedVal;
}
if (msg_id == 399){
    	std::stringstream hex_ss(msg);
    	hex_ss >> std::hex >> n;// making the message hex
    	binary = std::bitset<{64}>(n).to_string(); // convert hex to binary

	std::string raw = binary.substr(8,21); //LONG_DIST
	raw_dec = std::stoull(raw, 0, 2);
	scaled = (float)raw_dec * 0.040000 + 0.000000;
	returnedVal.var1 = scaled;

	std::string raw = binary.substr(24,35); //LAT_DIST
	if ((raw)[0] == '0'){
        	raw_dec= std::stoul( raw, 0, 2 );
	}
	else {
        	raw_dec = std::stoul(findTwosComplement(raw), 0, 2);
        	raw_dec = 	raw_dec * -1.0
	scaled = (float)raw_dec * 0.040000 + 0.000000;
	returnedVal.var2 = scaled;

	std::string raw = binary.substr(40,52); //REL_SPEED
	if ((raw)[0] == '0'){
        	raw_dec= std::stoul( raw, 0, 2 );
	}
	else {
        	raw_dec = std::stoul(findTwosComplement(raw), 0, 2);
        	raw_dec = 	raw_dec * -1.0
	scaled = (float)raw_dec * 0.025000 + 0.000000;
	returnedVal.var3 = scaled;

	return returnedVal;
}
if (msg_id == 1570){
    	std::stringstream hex_ss(msg);
    	hex_ss >> std::hex >> n;// making the message hex
    	binary = std::bitset<{64}>(n).to_string(); // convert hex to binary

	std::string raw = binary.substr(25,26); //HIGH_BEAMS_ON
	raw_dec = std::stoull(raw, 0, 2);
	scaled = (float)raw_dec * 1.000000 + 0.000000;
	returnedVal.var1 = scaled;

	return returnedVal;
}
if (msg_id == 467){
    	std::stringstream hex_ss(msg);
    	hex_ss >> std::hex >> n;// making the message hex
    	binary = std::bitset<{64}>(n).to_string(); // convert hex to binary

	std::string raw = binary.substr(8,9); //MAIN_ON
	raw_dec = std::stoull(raw, 0, 2);
	scaled = (float)raw_dec * 1.000000 + 0.000000;
	returnedVal.var1 = scaled;

	std::string raw = binary.substr(16,24); //SET_SPEED
	raw_dec = std::stoull(raw, 0, 2);
	scaled = (float)raw_dec * 1.000000 + 0.000000;
	conversion = ceilf((scaled* (10.0/36.0)) * 100) / 100;
	returnedVal.var2 = conversion;

	return returnedVal;
}
if (msg_id == 921){
    	std::stringstream hex_ss(msg);
    	hex_ss >> std::hex >> n;// making the message hex
    	binary = std::bitset<{64}>(n).to_string(); // convert hex to binary

	std::string raw = binary.substr(3,4); //MAIN_ON
	raw_dec = std::stoull(raw, 0, 2);
	scaled = (float)raw_dec * 1.000000 + 0.000000;
	returnedVal.var1 = scaled;

	std::string raw = binary.substr(24,32); //UI_SET_SPEED
	raw_dec = std::stoull(raw, 0, 2);
	scaled = (float)raw_dec * 1.000000 + 0.000000;
	returnedVal.var2 = scaled;

	std::string raw = binary.substr(12,16); //CRUISE_CONTROL_STATE
	raw_dec = std::stoull(raw, 0, 2);
	scaled = (float)raw_dec * 1.000000 + 0.000000;
	returnedVal.var3 = scaled;

	return returnedVal;
}
if (msg_id == 552){
    	std::stringstream hex_ss(msg);
    	hex_ss >> std::hex >> n;// making the message hex
    	binary = std::bitset<{64}>(n).to_string(); // convert hex to binary

	std::string raw = binary.substr(1,16); //ACCEL_X
	if ((raw)[0] == '0'){
        	raw_dec= std::stoul( raw, 0, 2 );
	}
	else {
        	raw_dec = std::stoul(findTwosComplement(raw), 0, 2);
        	raw_dec = 	raw_dec * -1.0
	scaled = (float)raw_dec * 0.001000 + 0.000000;
	returnedVal.var1 = scaled;

	return returnedVal;
}

}

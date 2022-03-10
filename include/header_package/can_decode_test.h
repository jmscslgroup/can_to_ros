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
if (msg_id == 37){
    	std::stringstream hex_ss(msg);
    	hex_ss >> std::hex >> n;// making the message hex
    	binary = std::bitset<64>(n).to_string(); // convert hex to binary

	std::string raw1 = binary.substr(4,12); //STEER_ANGLE
float STEER_ANGLE_fl;
        	if (raw1[0] == '0'){
        	STEER_ANGLE_fl= std::stoul( raw1, 0, 2 );
	}
	else {
        	STEER_ANGLE_fl = std::stoul(findTwosComplement(raw1), 0, 2);
        	STEER_ANGLE_fl = STEER_ANGLE_fl * -1.0;
        	}
	scaled = STEER_ANGLE_fl * 1.500000 + 0.000000;
	returnedVal.var1 = scaled;

	return returnedVal;
}
if (msg_id == 180){
    	std::stringstream hex_ss(msg);
    	hex_ss >> std::hex >> n;// making the message hex
    	binary = std::bitset<64>(n).to_string(); // convert hex to binary

	std::string raw1 = binary.substr(40,16); //SPEED
	raw_dec = std::stoull(raw1, 0, 2);
	scaled = (float)raw_dec * 0.010000 + 0.000000;
	conversion = ceilf((scaled* (10.0/36.0)) * 100) / 100;
	returnedVal.var1 = conversion;

	return returnedVal;
}
if (msg_id == 869){
    	std::stringstream hex_ss(msg);
    	hex_ss >> std::hex >> n;// making the message hex
    	binary = std::bitset<56>(n).to_string(); // convert hex to binary

	std::string raw1 = binary.substr(32,8); //LEAD_DISTANCE
	raw_dec = std::stoull(raw1, 0, 2);
	scaled = (float)raw_dec * 1.000000 + 0.000000;
	returnedVal.var1 = scaled;

	std::string raw2 = binary.substr(40,12); //REL_SPEED
float REL_SPEED_fl;
        	if (raw2[0] == '0'){
        	REL_SPEED_fl= std::stoul( raw2, 0, 2 );
	}
	else {
        	REL_SPEED_fl = std::stoul(findTwosComplement(raw2), 0, 2);
        	REL_SPEED_fl = REL_SPEED_fl * -1.0;
        	}
	scaled = REL_SPEED_fl * 0.017850 + 0.000000;
	returnedVal.var2 = scaled;

	return returnedVal;
}
if (msg_id == 384){
    	std::stringstream hex_ss(msg);
    	hex_ss >> std::hex >> n;// making the message hex
    	binary = std::bitset<64>(n).to_string(); // convert hex to binary

	std::string raw1 = binary.substr(8,13); //LONG_DIST
	raw_dec = std::stoull(raw1, 0, 2);
	scaled = (float)raw_dec * 0.040000 + 0.000000;
	returnedVal.var1 = scaled;

	std::string raw2 = binary.substr(24,11); //LAT_DIST
float LAT_DIST_fl;
        	if (raw2[0] == '0'){
        	LAT_DIST_fl= std::stoul( raw2, 0, 2 );
	}
	else {
        	LAT_DIST_fl = std::stoul(findTwosComplement(raw2), 0, 2);
        	LAT_DIST_fl = LAT_DIST_fl * -1.0;
        	}
	scaled = LAT_DIST_fl * 0.040000 + 0.000000;
	returnedVal.var2 = scaled;

	std::string raw3 = binary.substr(40,12); //REL_SPEED
float REL_SPEED_fl;
        	if (raw3[0] == '0'){
        	REL_SPEED_fl= std::stoul( raw3, 0, 2 );
	}
	else {
        	REL_SPEED_fl = std::stoul(findTwosComplement(raw3), 0, 2);
        	REL_SPEED_fl = REL_SPEED_fl * -1.0;
        	}
	scaled = REL_SPEED_fl * 0.017850 + 0.000000;
	returnedVal.var3 = scaled;

	return returnedVal;
}
if (msg_id == 385){
    	std::stringstream hex_ss(msg);
    	hex_ss >> std::hex >> n;// making the message hex
    	binary = std::bitset<64>(n).to_string(); // convert hex to binary

	std::string raw1 = binary.substr(8,13); //LONG_DIST
	raw_dec = std::stoull(raw1, 0, 2);
	scaled = (float)raw_dec * 0.040000 + 0.000000;
	returnedVal.var1 = scaled;

	std::string raw2 = binary.substr(24,11); //LAT_DIST
float LAT_DIST_fl;
        	if (raw2[0] == '0'){
        	LAT_DIST_fl= std::stoul( raw2, 0, 2 );
	}
	else {
        	LAT_DIST_fl = std::stoul(findTwosComplement(raw2), 0, 2);
        	LAT_DIST_fl = LAT_DIST_fl * -1.0;
        	}
	scaled = LAT_DIST_fl * 0.040000 + 0.000000;
	returnedVal.var2 = scaled;

	std::string raw3 = binary.substr(40,12); //REL_SPEED
float REL_SPEED_fl;
        	if (raw3[0] == '0'){
        	REL_SPEED_fl= std::stoul( raw3, 0, 2 );
	}
	else {
        	REL_SPEED_fl = std::stoul(findTwosComplement(raw3), 0, 2);
        	REL_SPEED_fl = REL_SPEED_fl * -1.0;
        	}
	scaled = REL_SPEED_fl * 0.017850 + 0.000000;
	returnedVal.var3 = scaled;

	return returnedVal;
}
if (msg_id == 386){
    	std::stringstream hex_ss(msg);
    	hex_ss >> std::hex >> n;// making the message hex
    	binary = std::bitset<64>(n).to_string(); // convert hex to binary

	std::string raw1 = binary.substr(8,13); //LONG_DIST
	raw_dec = std::stoull(raw1, 0, 2);
	scaled = (float)raw_dec * 0.040000 + 0.000000;
	returnedVal.var1 = scaled;

	std::string raw2 = binary.substr(24,11); //LAT_DIST
float LAT_DIST_fl;
        	if (raw2[0] == '0'){
        	LAT_DIST_fl= std::stoul( raw2, 0, 2 );
	}
	else {
        	LAT_DIST_fl = std::stoul(findTwosComplement(raw2), 0, 2);
        	LAT_DIST_fl = LAT_DIST_fl * -1.0;
        	}
	scaled = LAT_DIST_fl * 0.040000 + 0.000000;
	returnedVal.var2 = scaled;

	std::string raw3 = binary.substr(40,12); //REL_SPEED
float REL_SPEED_fl;
        	if (raw3[0] == '0'){
        	REL_SPEED_fl= std::stoul( raw3, 0, 2 );
	}
	else {
        	REL_SPEED_fl = std::stoul(findTwosComplement(raw3), 0, 2);
        	REL_SPEED_fl = REL_SPEED_fl * -1.0;
        	}
	scaled = REL_SPEED_fl * 0.017850 + 0.000000;
	returnedVal.var3 = scaled;

	return returnedVal;
}
if (msg_id == 387){
    	std::stringstream hex_ss(msg);
    	hex_ss >> std::hex >> n;// making the message hex
    	binary = std::bitset<64>(n).to_string(); // convert hex to binary

	std::string raw1 = binary.substr(8,13); //LONG_DIST
	raw_dec = std::stoull(raw1, 0, 2);
	scaled = (float)raw_dec * 0.040000 + 0.000000;
	returnedVal.var1 = scaled;

	std::string raw2 = binary.substr(24,11); //LAT_DIST
float LAT_DIST_fl;
        	if (raw2[0] == '0'){
        	LAT_DIST_fl= std::stoul( raw2, 0, 2 );
	}
	else {
        	LAT_DIST_fl = std::stoul(findTwosComplement(raw2), 0, 2);
        	LAT_DIST_fl = LAT_DIST_fl * -1.0;
        	}
	scaled = LAT_DIST_fl * 0.040000 + 0.000000;
	returnedVal.var2 = scaled;

	std::string raw3 = binary.substr(40,12); //REL_SPEED
float REL_SPEED_fl;
        	if (raw3[0] == '0'){
        	REL_SPEED_fl= std::stoul( raw3, 0, 2 );
	}
	else {
        	REL_SPEED_fl = std::stoul(findTwosComplement(raw3), 0, 2);
        	REL_SPEED_fl = REL_SPEED_fl * -1.0;
        	}
	scaled = REL_SPEED_fl * 0.017850 + 0.000000;
	returnedVal.var3 = scaled;

	return returnedVal;
}
if (msg_id == 388){
    	std::stringstream hex_ss(msg);
    	hex_ss >> std::hex >> n;// making the message hex
    	binary = std::bitset<64>(n).to_string(); // convert hex to binary

	std::string raw1 = binary.substr(8,13); //LONG_DIST
	raw_dec = std::stoull(raw1, 0, 2);
	scaled = (float)raw_dec * 0.040000 + 0.000000;
	returnedVal.var1 = scaled;

	std::string raw2 = binary.substr(24,11); //LAT_DIST
float LAT_DIST_fl;
        	if (raw2[0] == '0'){
        	LAT_DIST_fl= std::stoul( raw2, 0, 2 );
	}
	else {
        	LAT_DIST_fl = std::stoul(findTwosComplement(raw2), 0, 2);
        	LAT_DIST_fl = LAT_DIST_fl * -1.0;
        	}
	scaled = LAT_DIST_fl * 0.040000 + 0.000000;
	returnedVal.var2 = scaled;

	std::string raw3 = binary.substr(40,12); //REL_SPEED
float REL_SPEED_fl;
        	if (raw3[0] == '0'){
        	REL_SPEED_fl= std::stoul( raw3, 0, 2 );
	}
	else {
        	REL_SPEED_fl = std::stoul(findTwosComplement(raw3), 0, 2);
        	REL_SPEED_fl = REL_SPEED_fl * -1.0;
        	}
	scaled = REL_SPEED_fl * 0.017850 + 0.000000;
	returnedVal.var3 = scaled;

	return returnedVal;
}
if (msg_id == 389){
    	std::stringstream hex_ss(msg);
    	hex_ss >> std::hex >> n;// making the message hex
    	binary = std::bitset<64>(n).to_string(); // convert hex to binary

	std::string raw1 = binary.substr(8,13); //LONG_DIST
	raw_dec = std::stoull(raw1, 0, 2);
	scaled = (float)raw_dec * 0.040000 + 0.000000;
	returnedVal.var1 = scaled;

	std::string raw2 = binary.substr(24,11); //LAT_DIST
float LAT_DIST_fl;
        	if (raw2[0] == '0'){
        	LAT_DIST_fl= std::stoul( raw2, 0, 2 );
	}
	else {
        	LAT_DIST_fl = std::stoul(findTwosComplement(raw2), 0, 2);
        	LAT_DIST_fl = LAT_DIST_fl * -1.0;
        	}
	scaled = LAT_DIST_fl * 0.040000 + 0.000000;
	returnedVal.var2 = scaled;

	std::string raw3 = binary.substr(40,12); //REL_SPEED
float REL_SPEED_fl;
        	if (raw3[0] == '0'){
        	REL_SPEED_fl= std::stoul( raw3, 0, 2 );
	}
	else {
        	REL_SPEED_fl = std::stoul(findTwosComplement(raw3), 0, 2);
        	REL_SPEED_fl = REL_SPEED_fl * -1.0;
        	}
	scaled = REL_SPEED_fl * 0.017850 + 0.000000;
	returnedVal.var3 = scaled;

	return returnedVal;
}
if (msg_id == 390){
    	std::stringstream hex_ss(msg);
    	hex_ss >> std::hex >> n;// making the message hex
    	binary = std::bitset<64>(n).to_string(); // convert hex to binary

	std::string raw1 = binary.substr(8,13); //LONG_DIST
	raw_dec = std::stoull(raw1, 0, 2);
	scaled = (float)raw_dec * 0.040000 + 0.000000;
	returnedVal.var1 = scaled;

	std::string raw2 = binary.substr(24,11); //LAT_DIST
float LAT_DIST_fl;
        	if (raw2[0] == '0'){
        	LAT_DIST_fl= std::stoul( raw2, 0, 2 );
	}
	else {
        	LAT_DIST_fl = std::stoul(findTwosComplement(raw2), 0, 2);
        	LAT_DIST_fl = LAT_DIST_fl * -1.0;
        	}
	scaled = LAT_DIST_fl * 0.040000 + 0.000000;
	returnedVal.var2 = scaled;

	std::string raw3 = binary.substr(40,12); //REL_SPEED
float REL_SPEED_fl;
        	if (raw3[0] == '0'){
        	REL_SPEED_fl= std::stoul( raw3, 0, 2 );
	}
	else {
        	REL_SPEED_fl = std::stoul(findTwosComplement(raw3), 0, 2);
        	REL_SPEED_fl = REL_SPEED_fl * -1.0;
        	}
	scaled = REL_SPEED_fl * 0.017850 + 0.000000;
	returnedVal.var3 = scaled;

	return returnedVal;
}
if (msg_id == 391){
    	std::stringstream hex_ss(msg);
    	hex_ss >> std::hex >> n;// making the message hex
    	binary = std::bitset<64>(n).to_string(); // convert hex to binary

	std::string raw1 = binary.substr(8,13); //LONG_DIST
	raw_dec = std::stoull(raw1, 0, 2);
	scaled = (float)raw_dec * 0.040000 + 0.000000;
	returnedVal.var1 = scaled;

	std::string raw2 = binary.substr(24,11); //LAT_DIST
float LAT_DIST_fl;
        	if (raw2[0] == '0'){
        	LAT_DIST_fl= std::stoul( raw2, 0, 2 );
	}
	else {
        	LAT_DIST_fl = std::stoul(findTwosComplement(raw2), 0, 2);
        	LAT_DIST_fl = LAT_DIST_fl * -1.0;
        	}
	scaled = LAT_DIST_fl * 0.040000 + 0.000000;
	returnedVal.var2 = scaled;

	std::string raw3 = binary.substr(40,12); //REL_SPEED
float REL_SPEED_fl;
        	if (raw3[0] == '0'){
        	REL_SPEED_fl= std::stoul( raw3, 0, 2 );
	}
	else {
        	REL_SPEED_fl = std::stoul(findTwosComplement(raw3), 0, 2);
        	REL_SPEED_fl = REL_SPEED_fl * -1.0;
        	}
	scaled = REL_SPEED_fl * 0.017850 + 0.000000;
	returnedVal.var3 = scaled;

	return returnedVal;
}
if (msg_id == 392){
    	std::stringstream hex_ss(msg);
    	hex_ss >> std::hex >> n;// making the message hex
    	binary = std::bitset<64>(n).to_string(); // convert hex to binary

	std::string raw1 = binary.substr(8,13); //LONG_DIST
	raw_dec = std::stoull(raw1, 0, 2);
	scaled = (float)raw_dec * 0.040000 + 0.000000;
	returnedVal.var1 = scaled;

	std::string raw2 = binary.substr(24,11); //LAT_DIST
float LAT_DIST_fl;
        	if (raw2[0] == '0'){
        	LAT_DIST_fl= std::stoul( raw2, 0, 2 );
	}
	else {
        	LAT_DIST_fl = std::stoul(findTwosComplement(raw2), 0, 2);
        	LAT_DIST_fl = LAT_DIST_fl * -1.0;
        	}
	scaled = LAT_DIST_fl * 0.040000 + 0.000000;
	returnedVal.var2 = scaled;

	std::string raw3 = binary.substr(40,12); //REL_SPEED
float REL_SPEED_fl;
        	if (raw3[0] == '0'){
        	REL_SPEED_fl= std::stoul( raw3, 0, 2 );
	}
	else {
        	REL_SPEED_fl = std::stoul(findTwosComplement(raw3), 0, 2);
        	REL_SPEED_fl = REL_SPEED_fl * -1.0;
        	}
	scaled = REL_SPEED_fl * 0.017850 + 0.000000;
	returnedVal.var3 = scaled;

	return returnedVal;
}
if (msg_id == 393){
    	std::stringstream hex_ss(msg);
    	hex_ss >> std::hex >> n;// making the message hex
    	binary = std::bitset<64>(n).to_string(); // convert hex to binary

	std::string raw1 = binary.substr(8,13); //LONG_DIST
	raw_dec = std::stoull(raw1, 0, 2);
	scaled = (float)raw_dec * 0.040000 + 0.000000;
	returnedVal.var1 = scaled;

	std::string raw2 = binary.substr(24,11); //LAT_DIST
float LAT_DIST_fl;
        	if (raw2[0] == '0'){
        	LAT_DIST_fl= std::stoul( raw2, 0, 2 );
	}
	else {
        	LAT_DIST_fl = std::stoul(findTwosComplement(raw2), 0, 2);
        	LAT_DIST_fl = LAT_DIST_fl * -1.0;
        	}
	scaled = LAT_DIST_fl * 0.040000 + 0.000000;
	returnedVal.var2 = scaled;

	std::string raw3 = binary.substr(40,12); //REL_SPEED
float REL_SPEED_fl;
        	if (raw3[0] == '0'){
        	REL_SPEED_fl= std::stoul( raw3, 0, 2 );
	}
	else {
        	REL_SPEED_fl = std::stoul(findTwosComplement(raw3), 0, 2);
        	REL_SPEED_fl = REL_SPEED_fl * -1.0;
        	}
	scaled = REL_SPEED_fl * 0.017850 + 0.000000;
	returnedVal.var3 = scaled;

	return returnedVal;
}
if (msg_id == 394){
    	std::stringstream hex_ss(msg);
    	hex_ss >> std::hex >> n;// making the message hex
    	binary = std::bitset<64>(n).to_string(); // convert hex to binary

	std::string raw1 = binary.substr(8,13); //LONG_DIST
	raw_dec = std::stoull(raw1, 0, 2);
	scaled = (float)raw_dec * 0.040000 + 0.000000;
	returnedVal.var1 = scaled;

	std::string raw2 = binary.substr(24,11); //LAT_DIST
float LAT_DIST_fl;
        	if (raw2[0] == '0'){
        	LAT_DIST_fl= std::stoul( raw2, 0, 2 );
	}
	else {
        	LAT_DIST_fl = std::stoul(findTwosComplement(raw2), 0, 2);
        	LAT_DIST_fl = LAT_DIST_fl * -1.0;
        	}
	scaled = LAT_DIST_fl * 0.040000 + 0.000000;
	returnedVal.var2 = scaled;

	std::string raw3 = binary.substr(40,12); //REL_SPEED
float REL_SPEED_fl;
        	if (raw3[0] == '0'){
        	REL_SPEED_fl= std::stoul( raw3, 0, 2 );
	}
	else {
        	REL_SPEED_fl = std::stoul(findTwosComplement(raw3), 0, 2);
        	REL_SPEED_fl = REL_SPEED_fl * -1.0;
        	}
	scaled = REL_SPEED_fl * 0.017850 + 0.000000;
	returnedVal.var3 = scaled;

	return returnedVal;
}
if (msg_id == 395){
    	std::stringstream hex_ss(msg);
    	hex_ss >> std::hex >> n;// making the message hex
    	binary = std::bitset<64>(n).to_string(); // convert hex to binary

	std::string raw1 = binary.substr(8,13); //LONG_DIST
	raw_dec = std::stoull(raw1, 0, 2);
	scaled = (float)raw_dec * 0.040000 + 0.000000;
	returnedVal.var1 = scaled;

	std::string raw2 = binary.substr(24,11); //LAT_DIST
float LAT_DIST_fl;
        	if (raw2[0] == '0'){
        	LAT_DIST_fl= std::stoul( raw2, 0, 2 );
	}
	else {
        	LAT_DIST_fl = std::stoul(findTwosComplement(raw2), 0, 2);
        	LAT_DIST_fl = LAT_DIST_fl * -1.0;
        	}
	scaled = LAT_DIST_fl * 0.040000 + 0.000000;
	returnedVal.var2 = scaled;

	std::string raw3 = binary.substr(40,12); //REL_SPEED
float REL_SPEED_fl;
        	if (raw3[0] == '0'){
        	REL_SPEED_fl= std::stoul( raw3, 0, 2 );
	}
	else {
        	REL_SPEED_fl = std::stoul(findTwosComplement(raw3), 0, 2);
        	REL_SPEED_fl = REL_SPEED_fl * -1.0;
        	}
	scaled = REL_SPEED_fl * 0.017850 + 0.000000;
	returnedVal.var3 = scaled;

	return returnedVal;
}
if (msg_id == 396){
    	std::stringstream hex_ss(msg);
    	hex_ss >> std::hex >> n;// making the message hex
    	binary = std::bitset<64>(n).to_string(); // convert hex to binary

	std::string raw1 = binary.substr(8,13); //LONG_DIST
	raw_dec = std::stoull(raw1, 0, 2);
	scaled = (float)raw_dec * 0.040000 + 0.000000;
	returnedVal.var1 = scaled;

	std::string raw2 = binary.substr(24,11); //LAT_DIST
float LAT_DIST_fl;
        	if (raw2[0] == '0'){
        	LAT_DIST_fl= std::stoul( raw2, 0, 2 );
	}
	else {
        	LAT_DIST_fl = std::stoul(findTwosComplement(raw2), 0, 2);
        	LAT_DIST_fl = LAT_DIST_fl * -1.0;
        	}
	scaled = LAT_DIST_fl * 0.040000 + 0.000000;
	returnedVal.var2 = scaled;

	std::string raw3 = binary.substr(40,12); //REL_SPEED
float REL_SPEED_fl;
        	if (raw3[0] == '0'){
        	REL_SPEED_fl= std::stoul( raw3, 0, 2 );
	}
	else {
        	REL_SPEED_fl = std::stoul(findTwosComplement(raw3), 0, 2);
        	REL_SPEED_fl = REL_SPEED_fl * -1.0;
        	}
	scaled = REL_SPEED_fl * 0.017850 + 0.000000;
	returnedVal.var3 = scaled;

	return returnedVal;
}
if (msg_id == 397){
    	std::stringstream hex_ss(msg);
    	hex_ss >> std::hex >> n;// making the message hex
    	binary = std::bitset<64>(n).to_string(); // convert hex to binary

	std::string raw1 = binary.substr(8,13); //LONG_DIST
	raw_dec = std::stoull(raw1, 0, 2);
	scaled = (float)raw_dec * 0.040000 + 0.000000;
	returnedVal.var1 = scaled;

	std::string raw2 = binary.substr(24,11); //LAT_DIST
float LAT_DIST_fl;
        	if (raw2[0] == '0'){
        	LAT_DIST_fl= std::stoul( raw2, 0, 2 );
	}
	else {
        	LAT_DIST_fl = std::stoul(findTwosComplement(raw2), 0, 2);
        	LAT_DIST_fl = LAT_DIST_fl * -1.0;
        	}
	scaled = LAT_DIST_fl * 0.040000 + 0.000000;
	returnedVal.var2 = scaled;

	std::string raw3 = binary.substr(40,12); //REL_SPEED
float REL_SPEED_fl;
        	if (raw3[0] == '0'){
        	REL_SPEED_fl= std::stoul( raw3, 0, 2 );
	}
	else {
        	REL_SPEED_fl = std::stoul(findTwosComplement(raw3), 0, 2);
        	REL_SPEED_fl = REL_SPEED_fl * -1.0;
        	}
	scaled = REL_SPEED_fl * 0.017850 + 0.000000;
	returnedVal.var3 = scaled;

	return returnedVal;
}
if (msg_id == 398){
    	std::stringstream hex_ss(msg);
    	hex_ss >> std::hex >> n;// making the message hex
    	binary = std::bitset<64>(n).to_string(); // convert hex to binary

	std::string raw1 = binary.substr(8,13); //LONG_DIST
	raw_dec = std::stoull(raw1, 0, 2);
	scaled = (float)raw_dec * 0.040000 + 0.000000;
	returnedVal.var1 = scaled;

	std::string raw2 = binary.substr(24,11); //LAT_DIST
float LAT_DIST_fl;
        	if (raw2[0] == '0'){
        	LAT_DIST_fl= std::stoul( raw2, 0, 2 );
	}
	else {
        	LAT_DIST_fl = std::stoul(findTwosComplement(raw2), 0, 2);
        	LAT_DIST_fl = LAT_DIST_fl * -1.0;
        	}
	scaled = LAT_DIST_fl * 0.040000 + 0.000000;
	returnedVal.var2 = scaled;

	std::string raw3 = binary.substr(40,12); //REL_SPEED
float REL_SPEED_fl;
        	if (raw3[0] == '0'){
        	REL_SPEED_fl= std::stoul( raw3, 0, 2 );
	}
	else {
        	REL_SPEED_fl = std::stoul(findTwosComplement(raw3), 0, 2);
        	REL_SPEED_fl = REL_SPEED_fl * -1.0;
        	}
	scaled = REL_SPEED_fl * 0.017850 + 0.000000;
	returnedVal.var3 = scaled;

	return returnedVal;
}
if (msg_id == 399){
    	std::stringstream hex_ss(msg);
    	hex_ss >> std::hex >> n;// making the message hex
    	binary = std::bitset<64>(n).to_string(); // convert hex to binary

	std::string raw1 = binary.substr(8,13); //LONG_DIST
	raw_dec = std::stoull(raw1, 0, 2);
	scaled = (float)raw_dec * 0.040000 + 0.000000;
	returnedVal.var1 = scaled;

	std::string raw2 = binary.substr(24,11); //LAT_DIST
float LAT_DIST_fl;
        	if (raw2[0] == '0'){
        	LAT_DIST_fl= std::stoul( raw2, 0, 2 );
	}
	else {
        	LAT_DIST_fl = std::stoul(findTwosComplement(raw2), 0, 2);
        	LAT_DIST_fl = LAT_DIST_fl * -1.0;
        	}
	scaled = LAT_DIST_fl * 0.040000 + 0.000000;
	returnedVal.var2 = scaled;

	std::string raw3 = binary.substr(40,12); //REL_SPEED
float REL_SPEED_fl;
        	if (raw3[0] == '0'){
        	REL_SPEED_fl= std::stoul( raw3, 0, 2 );
	}
	else {
        	REL_SPEED_fl = std::stoul(findTwosComplement(raw3), 0, 2);
        	REL_SPEED_fl = REL_SPEED_fl * -1.0;
        	}
	scaled = REL_SPEED_fl * 0.017850 + 0.000000;
	returnedVal.var3 = scaled;

	return returnedVal;
}
if (msg_id == 1570){
    	std::stringstream hex_ss(msg);
    	hex_ss >> std::hex >> n;// making the message hex
    	binary = std::bitset<64>(n).to_string(); // convert hex to binary

	std::string raw1 = binary.substr(25,1); //HIGH_BEAMS_ON
	raw_dec = std::stoull(raw1, 0, 2);
	scaled = (float)raw_dec * 1.000000 + 0.000000;
	returnedVal.var1 = scaled;

	return returnedVal;
}
if (msg_id == 467){
    	std::stringstream hex_ss(msg);
    	hex_ss >> std::hex >> n;// making the message hex
    	binary = std::bitset<64>(n).to_string(); // convert hex to binary

	std::string raw1 = binary.substr(8,1); //MAIN_ON
	raw_dec = std::stoull(raw1, 0, 2);
	scaled = (float)raw_dec * 1.000000 + 0.000000;
	returnedVal.var1 = scaled;

	std::string raw2 = binary.substr(16,8); //SET_SPEED
	raw_dec = std::stoull(raw2, 0, 2);
	scaled = (float)raw_dec * 1.000000 + 0.000000;
	conversion = ceilf((scaled* (10.0/36.0)) * 100) / 100;
	returnedVal.var2 = conversion;

	return returnedVal;
}
if (msg_id == 921){
    	std::stringstream hex_ss(msg);
    	hex_ss >> std::hex >> n;// making the message hex
    	binary = std::bitset<64>(n).to_string(); // convert hex to binary

	std::string raw1 = binary.substr(3,1); //MAIN_ON
	raw_dec = std::stoull(raw1, 0, 2);
	scaled = (float)raw_dec * 1.000000 + 0.000000;
	returnedVal.var1 = scaled;

	std::string raw2 = binary.substr(24,8); //UI_SET_SPEED
	raw_dec = std::stoull(raw2, 0, 2);
	scaled = (float)raw_dec * 1.000000 + 0.000000;
	returnedVal.var2 = scaled;

	std::string raw3 = binary.substr(12,4); //CRUISE_CONTROL_STATE
	raw_dec = std::stoull(raw3, 0, 2);
	scaled = (float)raw_dec * 1.000000 + 0.000000;
	returnedVal.var3 = scaled;

	return returnedVal;
}
if (msg_id == 552){
    	std::stringstream hex_ss(msg);
    	hex_ss >> std::hex >> n;// making the message hex
    	binary = std::bitset<32>(n).to_string(); // convert hex to binary

	std::string raw1 = binary.substr(1,15); //ACCEL_X
float ACCEL_X_fl;
        	if (raw1[0] == '0'){
        	ACCEL_X_fl= std::stoul( raw1, 0, 2 );
	}
	else {
        	ACCEL_X_fl = std::stoul(findTwosComplement(raw1), 0, 2);
        	ACCEL_X_fl = ACCEL_X_fl * -1.0;
        	}
	scaled = ACCEL_X_fl * 0.001000 + 0.000000;
	returnedVal.var1 = scaled;

	return returnedVal;
}
	return returnedVal;
}

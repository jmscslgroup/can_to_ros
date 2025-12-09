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
if (msg_id == 36){
	std::string raw1 = binary.substr(22,10); //ACCEL_X
	raw_dec = std::stoull(raw1, 0, 2);
	scaled = (float)raw_dec * 0.035890 + -18.375000;
	returnedVal.var1 = scaled;

	std::string raw2 = binary.substr(38,10); //ACCEL_Y
	raw_dec = std::stoull(raw2, 0, 2);
	scaled = (float)raw_dec * 0.035890 + -18.375000;
	returnedVal.var2 = scaled;

	std::string raw3 = binary.substr(6,10); //YAW_RATE
	raw_dec = std::stoull(raw3, 0, 2);
	scaled = (float)raw_dec * 0.244000 + -125.000000;
	returnedVal.var3 = scaled;

	return returnedVal;
}
if (msg_id == 37){
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

	std::string raw2 = binary.substr(32,4); //STEER_FRACTION
	float STEER_FRACTION_fl;
        	if (raw2[0] == '0'){
        	STEER_FRACTION_fl= std::stoul( raw2, 0, 2 );
	}
	else {
        	STEER_FRACTION_fl = std::stoul(findTwosComplement(raw2), 0, 2);
        	STEER_FRACTION_fl = STEER_FRACTION_fl * -1.0;
        	}
	scaled = STEER_FRACTION_fl * 0.100000 + 0.000000;
	returnedVal.var2 = scaled;

	std::string raw3 = binary.substr(36,12); //STEER_RATE
	float STEER_RATE_fl;
        	if (raw3[0] == '0'){
        	STEER_RATE_fl= std::stoul( raw3, 0, 2 );
	}
	else {
        	STEER_RATE_fl = std::stoul(findTwosComplement(raw3), 0, 2);
        	STEER_RATE_fl = STEER_RATE_fl * -1.0;
        	}
	scaled = STEER_RATE_fl * 1.000000 + 0.000000;
	returnedVal.var3 = scaled;

	return returnedVal;
}
if (msg_id == 170){
	std::string raw1 = binary.substr(0,16); //WHEEL_SPEED_FR
	raw_dec = std::stoull(raw1, 0, 2);
	scaled = (float)raw_dec * 0.010000 + -67.670000;
	conversion = ceilf((scaled* (10.0/36.0)) * 100) / 100;
	returnedVal.var1 = conversion;

	std::string raw2 = binary.substr(16,16); //WHEEL_SPEED_FL
	raw_dec = std::stoull(raw2, 0, 2);
	scaled = (float)raw_dec * 0.010000 + -67.670000;
	conversion = ceilf((scaled* (10.0/36.0)) * 100) / 100;
	returnedVal.var2 = conversion;

	std::string raw3 = binary.substr(32,16); //WHEEL_SPEED_RR
	raw_dec = std::stoull(raw3, 0, 2);
	scaled = (float)raw_dec * 0.010000 + -67.670000;
	conversion = ceilf((scaled* (10.0/36.0)) * 100) / 100;
	returnedVal.var3 = conversion;

	std::string raw4 = binary.substr(48,16); //WHEEL_SPEED_RL
	raw_dec = std::stoull(raw4, 0, 2);
	scaled = (float)raw_dec * 0.010000 + -67.670000;
	conversion = ceilf((scaled* (10.0/36.0)) * 100) / 100;
	returnedVal.var4 = conversion;

	return returnedVal;
}
if (msg_id == 180){
	std::string raw1 = binary.substr(40,16); //SPEED
	raw_dec = std::stoull(raw1, 0, 2);
	scaled = (float)raw_dec * 0.010000 + 0.000000;
	conversion = ceilf((scaled* (10.0/36.0)) * 100) / 100;
	returnedVal.var1 = conversion;

	return returnedVal;
}
if (msg_id == 384){
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
if (msg_id == 467){
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
if (msg_id == 552){
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
if (msg_id == 608){
	std::string raw1 = binary.substr(8,16); //STEER_TORQUE_DRIVER
	float STEER_TORQUE_DRIVER_fl;
        	if (raw1[0] == '0'){
        	STEER_TORQUE_DRIVER_fl= std::stoul( raw1, 0, 2 );
	}
	else {
        	STEER_TORQUE_DRIVER_fl = std::stoul(findTwosComplement(raw1), 0, 2);
        	STEER_TORQUE_DRIVER_fl = STEER_TORQUE_DRIVER_fl * -1.0;
        	}
	scaled = STEER_TORQUE_DRIVER_fl * 1.000000 + 0.000000;
	returnedVal.var1 = scaled;

	std::string raw2 = binary.substr(40,16); //STEER_TORQUE_EPS
	float STEER_TORQUE_EPS_fl;
        	if (raw2[0] == '0'){
        	STEER_TORQUE_EPS_fl= std::stoul( raw2, 0, 2 );
	}
	else {
        	STEER_TORQUE_EPS_fl = std::stoul(findTwosComplement(raw2), 0, 2);
        	STEER_TORQUE_EPS_fl = STEER_TORQUE_EPS_fl * -1.0;
        	}
	scaled = STEER_TORQUE_EPS_fl * 0.660000 + 0.000000;
	returnedVal.var2 = scaled;

	std::string raw3 = binary.substr(7,1); //STEER_OVERRIDE
	raw_dec = std::stoull(raw3, 0, 2);
	scaled = (float)raw_dec * 1.000000 + 0.000000;
	returnedVal.var3 = scaled;

	return returnedVal;
}
if (msg_id == 740){
	std::string raw1 = binary.substr(7,1); //STEER_REQUEST
	raw_dec = std::stoull(raw1, 0, 2);
	scaled = (float)raw_dec * 1.000000 + 0.000000;
	returnedVal.var1 = scaled;

	std::string raw2 = binary.substr(8,16); //STEER_TORQUE_CMD
	float STEER_TORQUE_CMD_fl;
        	if (raw2[0] == '0'){
        	STEER_TORQUE_CMD_fl= std::stoul( raw2, 0, 2 );
	}
	else {
        	STEER_TORQUE_CMD_fl = std::stoul(findTwosComplement(raw2), 0, 2);
        	STEER_TORQUE_CMD_fl = STEER_TORQUE_CMD_fl * -1.0;
        	}
	scaled = STEER_TORQUE_CMD_fl * 1.000000 + 0.000000;
	returnedVal.var2 = scaled;

	return returnedVal;
}
if (msg_id == 835){
	std::string raw1 = binary.substr(0,16); //ACCEL_CMD
	float ACCEL_CMD_fl;
        	if (raw1[0] == '0'){
        	ACCEL_CMD_fl= std::stoul( raw1, 0, 2 );
	}
	else {
        	ACCEL_CMD_fl = std::stoul(findTwosComplement(raw1), 0, 2);
        	ACCEL_CMD_fl = ACCEL_CMD_fl * -1.0;
        	}
	scaled = ACCEL_CMD_fl * 0.001000 + 0.000000;
	returnedVal.var1 = scaled;

	std::string raw2 = binary.substr(18,1); //MINI_CAR
	raw_dec = std::stoull(raw2, 0, 2);
	scaled = (float)raw_dec * 1.000000 + 0.000000;
	returnedVal.var2 = scaled;

	std::string raw3 = binary.substr(25,1); //CAR_AHEAD
	raw_dec = std::stoull(raw3, 0, 2);
	scaled = (float)raw_dec * 1.000000 + 0.000000;
	returnedVal.var3 = scaled;

	std::string raw4 = binary.substr(31,1); //CANCEL_REQ
	raw_dec = std::stoull(raw4, 0, 2);
	scaled = (float)raw_dec * 1.000000 + 0.000000;
	returnedVal.var4 = scaled;

	std::string raw5 = binary.substr(30,1); //ACC_CUT_IN
	raw_dec = std::stoull(raw5, 0, 2);
	scaled = (float)raw_dec * 1.000000 + 0.000000;
	returnedVal.var5 = scaled;

	std::string raw6 = binary.substr(21,1); //ACC_MALFUNCTION
	raw_dec = std::stoull(raw6, 0, 2);
	scaled = (float)raw_dec * 1.000000 + 0.000000;
	returnedVal.var6 = scaled;

	return returnedVal;
}
if (msg_id == 869){
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

	std::string raw3 = binary.substr(5,1); //SET_BTN
	raw_dec = std::stoull(raw3, 0, 2);
	scaled = (float)raw_dec * 1.000000 + 0.000000;
	returnedVal.var3 = scaled;

	std::string raw4 = binary.substr(4,1); //RES_BTN
	raw_dec = std::stoull(raw4, 0, 2);
	scaled = (float)raw_dec * 1.000000 + 0.000000;
	returnedVal.var4 = scaled;

	std::string raw5 = binary.substr(6,1); //CANCEL_BTN
	raw_dec = std::stoull(raw5, 0, 2);
	scaled = (float)raw_dec * 1.000000 + 0.000000;
	returnedVal.var5 = scaled;

	return returnedVal;
}
if (msg_id == 921){
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

    if (int(scaled) == 2){
    	returnedVal.choice_var3 = "disabled";
    	}

    if (int(scaled) == 11){
    	returnedVal.choice_var3 = "hold";
    	}

    if (int(scaled) == 10){
    	returnedVal.choice_var3 = "hold_waiting_user_cmd";
    	}

    if (int(scaled) == 6){
    	returnedVal.choice_var3 = "enabled";
    	}

    if (int(scaled) == 5){
    	returnedVal.choice_var3 = "faulted";
    	}

    if (int(scaled) == 0){
    	returnedVal.choice_var3 = "off";
    	}

	std::string raw4 = binary.substr(9,2); //DISTANCE_LINES
	raw_dec = std::stoull(raw4, 0, 2);
	scaled = (float)raw_dec * 1.000000 + 0.000000;
	returnedVal.var4 = scaled;

	return returnedVal;
}
if (msg_id == 956){
	std::string raw1 = binary.substr(5,1); //SPORT_ON
	raw_dec = std::stoull(raw1, 0, 2);
	scaled = (float)raw_dec * 1.000000 + 0.000000;
	returnedVal.var1 = scaled;

	std::string raw2 = binary.substr(47,1); //ECON_ON
	raw_dec = std::stoull(raw2, 0, 2);
	scaled = (float)raw_dec * 1.000000 + 0.000000;
	returnedVal.var2 = scaled;

	return returnedVal;
}
if (msg_id == 1570){
	std::string raw1 = binary.substr(25,1); //HIGH_BEAMS_ON
	raw_dec = std::stoull(raw1, 0, 2);
	scaled = (float)raw_dec * 1.000000 + 0.000000;
	returnedVal.var1 = scaled;

	return returnedVal;
}
	return returnedVal;
}

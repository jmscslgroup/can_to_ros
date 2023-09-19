#Author: Matthew Nice

import cantools
import json
import os

def findDBC(vin_details):
    userFile = open('/etc/libpanda.d/libpanda_usr','r')
    user = userFile.read().rstrip('\n\r')
    userFile.close()
    jsonfile = None
    dbcfile = None
    
    if vin_details['Make'] == 'TOYOTA':
        if vin_details['Model'] == 'RAV4':
            jsonfile = 'toyota_rav4.json'
            if int(vin_details['ModelYear']) >= 2021:
                #TODO: figure out where the dbc files will be
                dbcfile = '/home/' + user + '/strym/strym/dbc/toyota_rav4_2021.dbc'
            elif int(vin_details['ModelYear']) == 2020:
                dbcfile = '/home/' + user + '/strym/strym/dbc/toyota_rav4_2020.dbc'
            elif int(vin_details['ModelYear']) == 2019:
                dbcfile = '/home/' + user + '/strym/strym/dbc/toyota_rav4_2019.dbc'
            if 'HV' in vin_details['Trim']:
                # dbcfile = '/Users/mnice/Documents/GitHub/strym/strym/dbc/toyota_rav4_hybrid.dbc'
                dbcfile = '/home/' + user + '/strym/strym/dbc/toyota_rav4_hybrid.dbc'
    #space here to add in info for honda and nissan vehicles
    if vin_details['Make'] == 'NISSAN':
        if vin_details['Model'] == 'Rogue':
            jsonfile = 'nissan_rogue.json'
            if int(vin_details['ModelYear']) >= 2021:
                #TODO: figure out where the dbc files will be
                # dbcfile = '/Users/mnice/Documents/GitHub/strym/strym/dbc/nissan_rogue_2021.dbc'
                dbcfile = '/home/' + user + '/strym/strym/dbc/nissan_rogue_2021.dbc'
                # dbcfile = '/home/' + user + '/strym/strym/dbc/nissan_rogue_experimental.dbc'

    if vin_details['Make'] == 'HONDA':
        if vin_details['Model'] == 'Pilot':
            jsonfile = 'honda_pilot.json'
            dbcfile = '/home/' + user + '/strym/strym/dbc/honda_pilot_2017.dbc'
    try:
        print('The DBC is set as %s. The Json is set as %s'%(dbcfile,jsonfile))
    except UnboundLocalError:
        print('DBC File not found, check /etc/libpanda.d/vin_details.json is correct.')

    return jsonfile, dbcfile

def initializeDBC_Cantools(fileName):
    db = cantools.database.Database()
    with open(fileName,'r') as fin:
        db.add_dbc_string(fin.read())
    return db

def findMessageInfo(messageNameorNum,db):
    """This function does text parsing on a dbc file to select the relevant pieces
    of information.

    It looks first for the kind of message that is desired, which
    is specified by messageNameorNum. It then looks through db, which is a Database object from cantools
    that corresponds to the lines of the dbc file. It returns the message, which
    includes the message attributes, its signals and their attributes as well."""

#Breakdown of a signal: name, start, length, byte_order, is_signed, is_float, scale, offset, minimum, maximum, unit, is_Multiplexer, idk=None ,idk=None , comment
    if type(messageNameorNum) is str:
        try:
            messageInfo = db.get_message_by_name(messageNameorNum) #if string used, get message by name, e.g. 'KINEMATICS'
        except:
            print("warning: str addr not in DBC")
            messageInfo = "not in DBC"
    elif type(messageNameorNum) is int:
        try:
            messageInfo = db.get_message_by_frame_id(messageNameorNum) #if number use, get message by frame id, e.g. 36
        except:
            print("warning: int addr not in DBC")
            messageInfo = "not in DBC"

    return messageInfo

def signalCheck(signal):
    print(signal.start,signal.length,signal.scale, signal.offset, signal.unit, signal.is_signed)

def getSignals(key, dictionary, dbc):
    """Inputting the msgID as key, the dictionary of signals to go into ROS, and the dbc where the
    signal decoding is defined. You get the cantools msg and signals back."""
    msg = findMessageInfo(key,dbc)
    signalNames=dictionary[key]

    signals = []
    for i in signalNames:
        signals.append(msg.get_signal_by_name(i))

    return msg, signals

def decodeBuilder(msg, signals,toROS):
    """Cantools message and signals are input."""

    decode = "if (msg_id == %d){\n"%(msg.frame_id)
    # \tstd::stringstream hex_ss(msg);\n\
    # \thex_ss >> std::hex >> n;// making the message hex\n\
    # \tbinary = std::bitset<%d>(n).to_string(); // convert hex to binary\n\n"%(msg.frame_id,msg.length*8)
    # for canid in toROS:
    #     for rostopic in toROS[canid]:
    #         rosmsg= toROS[canid][rostopic]
    #             if signals[0].name in rosmsg[1]:
    #                 print(rosmsg)
    #                 if 'String' in rosmsg[0][0]:
    #                     print('string in here!')

    ##find rosmsg(s) for a given signal in toROS

    #for each signal in the set of signals:
    signalString = ""
    rosmsg_info_list = []
    for i in signals: #pass the rosmsg(s) for a signal to the findSubstring function
        # print('signal is: ', i)
        for canid in toROS:
            # print('canid is: ', canid)
            for rostopic in toROS[canid]:
                # print('rostopic is: ', rostopic)
                rosmsg= toROS[canid][rostopic]
                if i.name in rosmsg[1]:
                    rosmsg_info_list+=rosmsg
                    # print('rosmsg info is: ',rosmsg_info)
        signalString += findSubstring(i,signals.index(i),rosmsg_info_list) #finds substring and does conversions
        rosmsg_info = []
#         print(signals.index(i))
    returns = '\treturn returnedVal;\n}\n'

    decode=decode+ signalString + returns

    return decode

def findSubstring(signal, varNum, rosmsg_info_list):
    #find the bit substring
    print('running substring, my signal is: ', signal)
    print('my rosmsg info is: ', rosmsg_info_list,'\n')
    varNum+=1
    pos = bitConversion(signal.start)
    rawVal = "\tstd::string raw%d = binary.substr(%d,%d); //%s\n"%(varNum,pos,signal.length,signal.name)

    if not signal.is_signed:
        rawVal_dec = "\traw_dec = std::stoull(raw%d, 0, 2);\n"%(varNum)
    else:
        rawVal_dec = "\tfloat %s_fl;\n\
        \tif (raw%d[0] == '0'){\n\
        \t%s_fl= std::stoul( raw%d, 0, 2 );\n\t}\n\telse {\n\
        \t%s_fl = std::stoul(findTwosComplement(raw%d), 0, 2);\n\
        \t%s_fl = %s_fl * -1.0;\n\
        \t}\n"%(signal.name,varNum,signal.name,varNum,signal.name,varNum,signal.name,signal.name)

    if not signal.is_signed:
        scale = "\tscaled = (float)raw_dec * %0.6f + %0.6f;\n"%(signal.scale,signal.offset)
    else:
        scale = "\tscaled = %s_fl * %0.6f + %0.6f;\n"%(signal.name,signal.scale,signal.offset)


    if signal.unit == 'kph':
        conversion = "\tconversion = ceilf((scaled* (10.0/36.0)) * 100) / 100;\n"
        returner = '\treturnedVal.var%d = conversion;\n'%(varNum)
    else:
        conversion = ''
        returner = '\treturnedVal.var%d = scaled;\n'%(varNum)

#####changes to make to create a string in the header file #########
    string_choice = ''
    flag = False
    for l in rosmsg_info_list:
        for k in l:
            if 'String' in k:
                flag = True
    if (signal.choices != None and flag==True):##rosmsg type is string):
        choiceDict=signal.choices
        print('found a string in header')
        for key in choiceDict.keys():
            string_choice += '\n\
    if (int(scaled) == %d){\n\
    \treturnedVal.choice_var%d = "%s";\n\
    \t}\n'%(key,varNum,choiceDict[key])


    ##  if decimal value is in choiceDict.keys()
    ##      change value to be a string in the values_struct
    ##  else: return the decimal value as usual

    mySignalCode = rawVal + rawVal_dec + scale + conversion + returner + string_choice +'\n'
#     print(decode)

    return mySignalCode

def bitConversion(x):
    y = (int(x/8)+1)*8 -(x%8)-1
    return y

def keystoint(x):
    try:
        return {int(k): v for k, v in x.items()}
    except:
        return x

def generatePrivatePubs(toROS):

    text = "private:\n\
\tros::NodeHandle n_;\n"

    for canid in toROS:
            for rostopic in toROS[canid]:
                rostopicVar = rostopic.replace('/','_')
                text += "\tros::Publisher %s_pub;\n"%(rostopicVar)

    text += "\n\tros::Subscriber sub_;\n\
\tdecode_msgs obj;\n\
\tstd::string Time,Buffer,Message,MessageLength;\n\
\tdouble MessageID, Bus;\n\
\tvalues data;\n\
};\n"

    return text

def buildCallbacks(toROS):##add varNum?
    """Use this function to generate the callbacks from the JSON"""
    # print('this is to ROS')
    # print(toROS)
    text = ""
    for canid in toROS:
        if len(text) > 0:
                call = "\telse if (MessageID == %s)\n\
\t{\n\
\t\tdata = obj.decode_message (MessageID, Message);\n"%(canid)

                text += call
        else:
            call = "\tif (MessageID == %s){\n\
\t\tdata = obj.decode_message (MessageID, Message);\n"%(canid)

            text += call
        count = 0
        for rostopic in toROS[canid]:
            print(rostopic)
            found=False

            rosmsg = toROS[canid][rostopic][0][0]
            signals = toROS[canid][rostopic][1]
            count += 1
            text+= '\t\t%s msg%d;\n'%(rosmsg,count)
            # print(rosmsg)

            if 'PointStamped' in rosmsg:
                # print(signals,len(signals))
                ###treat as a pointstamped
                text+= '\t\tmsg%d.header.frame_id = "front_laser_link";\n\
    msg%d.header.stamp = ros::Time(std::stod(Time));\n'%(count,count)

                if len(signals) >= 3: #this is a bad fix to a msg specified with more than 3 values, when 3 is max
                    text+= '\t\tmsg%d.point.x = data.var%d; //%s\n\
    msg%d.point.y = data.var%d; //%s\n\
    msg%d.point.z = data.var%d; //%s\n' %(count,toDecode.get(canid).index(signals[0])+1,signals[0]\
                ,count,toDecode.get(canid).index(signals[1])+1,signals[1],\
                count,toDecode.get(canid).index(signals[2])+1,signals[2])#check to see if this works for radar signals, may need to change the decode_message
                elif len(signals) == 2:
                    text+= '\t\tmsg%d.point.x = data.var%d; //%s\n\
            msg%d.point.y = data.var%d; //%s\n' %(count,toDecode.get(canid).index(signals[0])+1,signals[0]\
            ,count,toDecode.get(canid).index(signals[1])+1,signals[1])#check to see if this works for radar signals, may need to change the decode_message
                elif len(signals) == 1:
                    #text+= '\t\tmsg%d.point.x = data.var%d; //%s\n' %(count,(count-1)*3+1,signals[0],count)#check to see if this works for radar signals, may need to change the decode_message
                    text+= '\t\tmsg%d.point.x = data.var%d; //%s\n' %(count,toDecode.get(canid).index(signals[0])+1,signals[0])#check to see if this works for radar signals, may need to change the decode_message
                found = True

            elif 'Point' in rosmsg:
                ##treat as point
                var = ['.x','.y','.z']
                for i in range(0,min(len(signals),3)):
                    # print(signals)
                    text += '\t\tmsg%d%s = data.var%s; //%s\n'%(count,var[i],toDecode.get(canid).index(signals[i])+1,signals[i])
                    found = True
            #### changes to make in the cpp node ####
            ## if the rosmsg is String
            elif 'String' in rosmsg:
                print('found a string in callbacks')
                text += '\t\tmsg%d.data = data.choice_var%d; //%s\n'%(count,toDecode.get(canid).index(signals[0])+1,signals[0])
                found = True
            ## take the string value from the values_struct of the signal index
            elif 'Float64MultiArray' in rosmsg: # courtesy of Bunting
                arrayLabel = signals[0]
                for i in range(len(signals)-1):
                    arrayLabel += '/' + signals[i+1]
                text += '\t\tmsg%d.layout.dim.push_back(std_msgs::MultiArrayDimension());\n\
                msg%d.layout.dim[0].size = %d;\n\
                msg%d.layout.dim[0].stride = 1;\n\
                msg%d.layout.dim[0].label = "%s";\n\
                \n\
                msg%d.data.clear();\n' % (count, count, len(signals), count, count, arrayLabel, count)
                for i in range(len(signals)):
                    text += '\t\tmsg%d.data.push_back(data.var%s);\t//%s\n' % (count,toDecode.get(canid).index(signals[i])+1,signals[i])
                found = True
            else:
                singleValTypes = ['Float64','Float32','Int16','Int8']

                for type in singleValTypes:
                    if type in rosmsg:
                        ##treat as a float64
                        # print(canid,signals)
                        # print(toDecode.get(canid))
                        text += '\t\tmsg%d.data = data.var%d; //%s\n'%(count,toDecode.get(canid).index(signals[0])+1,signals[0])
                        found = True

            if found == False:
                print('this rosmg not accounted for: ' + rosmsg)


            rostopicVar = rostopic.replace('/','_')
            text += '\t\t%s_pub.publish(msg%d);\n\n'%(rostopicVar,count)
        text += '\t}\n'
    text += '}\n'
    return text

def generateToDecode(toROS):
    """Using ROS node JSON/dictionary to generate decoding header json"""
    temp = toROS
    decode = {}

    for canid in temp:
#         print(toROS[canid])
        counter = 1
        for topic in temp[canid]:
#             print(toROS[canid][topic][1])
            # print('inside the for loop')
            # print(toROS)
            try:
                skip=False
                for each in temp[canid][topic][1]:
                    if each in decode[canid]:
                        skip=True
                    # print('this: ',temp[canid][topic][1],' is not in this: ', decode[canid])
                if skip == False:
                    decode[canid] += temp[canid][topic][1]
            except:
                # print('I am in except')
                # print(temp[canid][topic][1])
                decode[canid] = temp[canid][topic][1]
            counter += 1
#             print(decode)
    return decode

def buildAds(toROS):
    """Function that builds the ros node pubs from a JSON read in. Model-specific JSON specifying the node."""

    text = ""
    for canid in toROS:
        for rostopic in toROS[canid]:
#             print(rostopic)
            rosmsg = toROS[canid][rostopic][0][0]
            signals = toROS[canid][rostopic][1]

            # print(rosmsg,rostopic)
            rostopicVar = rostopic.replace('/','_')

            text += '\t%s_pub = n_.advertise<%s>("%s",1000);\n'%(rostopicVar,rosmsg,rostopic)
    return text

def generateMain(toROS):
    text = ''

    text +='int main(int argc, char **argv){\n\
\tros::init(argc, argv, "subs_fs");\n\
\tros::NodeHandle nh1;\n\
\tSubscribeAndPublish SAPObject;\n\
\tros::spin();\n\
\treturn 0;\n\
}'
    return text

def buildNode(toROS):

    text = ""
    text+= buildAds(toROS)
    #ad subs and start stringstream
    text+='\n\tsub_ = n_.subscribe("/realtime_raw_data", 1000, &SubscribeAndPublish::callback, this);\n\
}\n\
\tvoid callback(const std_msgs::String::ConstPtr& raw_data)\n\
{\n\n\
\tstd::stringstream ss(raw_data->data);\n\
\tss >> Time>> Bus>> MessageID>> Message>> MessageLength;\n\n'

    text+= buildCallbacks(toROS)
    text+= generatePrivatePubs(toROS)
    text+= generateMain(toROS)

    return text


#/etc/libpanda.d has a JSON with the make,model,trim,year
f = open('/etc/libpanda.d/vin_details.json')
vin_details = json.load(f)
f.close()
#find the correct DBC and ROS msg dict based on the vin details
try:
    jsonfile, dbcfile = findDBC(vin_details)
except TypeError:
    print('findDBC Function did not operate properly, check /etc/libpanda/vin_details.json')
#load the correct DBC file to decode CAN to ROS
if dbcfile is not None:
    try:
        dbc = initializeDBC_Cantools(dbcfile)
    except FileNotFoundError:
        userFile = open('/etc/libpanda.d/libpanda_usr','r')
        user = userFile.read().rstrip('\n\r')
        userFile.close()
        cmd = "mkdir -p /home/" + user + "/strym/strym/dbc && wget -P /home/" + user + "/strym/strym/dbc/ https://raw.githubusercontent.com/jmscslgroup/strym/master/strym/dbc/" + (dbcfile.split('/')[-1])
        print(cmd)
        print("File Not Found -- did not run download.")
        # os.system(cmd)
        # dbcfile = '/Users/mnice/Documents/GitHub/strym/strym/dbc/toyota_rav4_hybrid.dbc'

        #dbc = initializeDBC_Cantools(dbcfile)
        dbc = cantools.database.Database()
else:
    dbc = cantools.database.Database()
    
if jsonfile is not None:
    # Opening ROS JSON file
    f = open('./'+jsonfile)

    # returns JSON object as a dictionary
    print('The generated JSON file is: ',jsonfile)
    toROS = json.load(f, object_hook=keystoint)
    # print('this is toROS before generateToDecode')
    # print(toROS)
    f.close()
else:
    toROS = {}
    
toDecode = generateToDecode(toROS)

#
# print('this is toROS after generateToDecode')
# print(toROS)
#generating the C++ header file
#TODO: for each key in the decoding dictionary, add text to the can_decode.h file
text = ''
print('The JSON file contains these signals: ',toDecode)
for i in toDecode.keys():
    msg, signals = getSignals(i,toDecode,dbc)
    # rosmsg = ###name of the rosmsg rosmsg = toROS[canid][rostopic][0][0]
    text += decodeBuilder(msg, signals,toROS)
text += '\treturn returnedVal;\n}\n'

###put in changes for realtime_raw_data publishing in can to ros #####

##open base vehicle_interface_nissan
##find the line of the comment where the new lines go in
##insert with sed the lines? or python similar tool
##each line states the message ID and length to published

#output into C++ file(s)
#TODO make them the real files (after testing) <--DONE
file1 = open('../include/header_package/can_decode.h', 'w')
file2 = open('can_decode_base.h','r')
file1.writelines(file2.read())
file2.close()

file1.writelines(text)
file1.close()
########finished creating can_decode.h

###########Generating the subs_fs.cpp node ########
# print('this is to ROS before buildNode')
# print(toROS)
text = ''
text += buildNode(toROS)
# print('buildNode complete')
#TODO make into the real file
file1 = open('../src/subs.cpp', 'w')
file2 = open('../src/subs_fs_base.cpp','r')

file1.writelines(file2.read())
file2.close()

file1.writelines(text)
file1.close()
########Finished generating subs_fs.cpp node #######


#FINAL: confirmed working with comparison of rosbags

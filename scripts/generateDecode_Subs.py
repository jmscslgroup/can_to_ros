import cantools
import json


def findDBC(vin_details):
    if vin_details['Make'] == 'TOYOTA':
        if vin_details['Model'] == 'RAV4':
            jsonfile = 'toyota_rav4.json'
            if int(vin_details['ModelYear']) >= 2021:
                #TODO: figure out where the dbc files will be
                dbcfile = '/home/circles/strym/strym/dbc/toyota_rav4_2021.dbc'
            elif int(vin_details['ModelYear']) == 2020:
                dbcfile = '/home/circles/strym/strym/dbc/toyota_rav4_2020.dbc'
            elif int(vin_details['ModelYear']) == 2019:
                dbcfile = '/home/circles/strym/strym/dbc/toyota_rav4_2019.dbc'
            if 'HV' in vin_details['Trim']:
                dbcfile = '/home/circles/strym/strym/dbc/toyota_rav4_hybrid.dbc'
#space here to add in info for honda and nissan vehicles
    if vin_details['Make'] == 'NISSAN':
        if vin_details['Model'] == 'Rogue':
            jsonfile = 'nissan_rogue.json'
            if int(vin_details['ModelYear']) >= 2021:
                #TODO: figure out where the dbc files will be
                dbcfile = '/home/circles/strym/strym/dbc/nissan_rogue_2021.dbc'
    print('the DBC is set as %s'%(dbcfile))
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

def decodeBuilder(msg, signals):
    """Cantools message and signals are input."""

    decode = "if (msg_id == %d){\n\
    \tstd::stringstream hex_ss(msg);\n\
    \thex_ss >> std::hex >> n;// making the message hex\n\
    \tbinary = std::bitset<%d>(n).to_string(); // convert hex to binary\n\n"%(msg.frame_id,msg.length*8)

    #for each signal in the set of signals:
    signalString = ""
    for i in signals:
        signalString += findSubstring(i,signals.index(i)) #finds substring and does conversions
#         print(signals.index(i))
    returns = '\treturn returnedVal;\n}\n'

    decode=decode+ signalString + returns

    return decode

def findSubstring(signal, varNum):
    #find the bit substring
    varNum+=1
    pos = bitConversion(signal.start)
    rawVal = "\tstd::string raw%d = binary.substr(%d,%d); //%s\n"%(varNum,pos,signal.length,signal.name)

    if not signal.is_signed:
        rawVal_dec = "\traw_dec = std::stoull(raw%d, 0, 2);\n"%(varNum)
    else:
        rawVal_dec = "float %s_fl;\n\
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


    mySignalCode = rawVal + rawVal_dec + scale + conversion + returner +'\n'
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
                text += "\tros::Publisher %s_pub;\n"%(rostopic)

    text += "\n\tros::Subscriber sub_;\n\
\tdecode_msgs obj;\n\
\tstd::string Time,Buffer,Message,MessageLength;\n\
\tdouble MessageID, Bus;\n\
\tvalues data;\n\
};\n"

    return text

def buildCallbacks(toROS):
    """Use this function to generate the callbacks from the JSON"""


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
#             print(rostopic)
            rosmsg = toROS[canid][rostopic][0][0]
            signals = toROS[canid][rostopic][1]
            count += 1
            text+= '\t\t%s msg%d;\n'%(rosmsg,count)


            if 'PointStamped' in rosmsg:
                ###treat as a pointstamped
                text+= '\t\tmsg%d.header.frame_id = "front_laser_link";\n\
                msg%d.header.stamp = ros::Time(std::stod(Time));\n'%(count,count)

                text+= '\t\tmsg%d.point.x = data.var1; //%s\n\
                msg%d.point.y = data.var2; //%s\n\
                msg%d.point.z = data.var3; //%s\n' %(count,signals[0],count,signals[1],count,signals[2])#check to see if this works for radar signals, may need to change the decode_message

            elif 'Float64' in rosmsg:
                ##treat as a float64
#                 print(canid,signals)
#                 print(signals in toDecode.get(canid))
                text += '\t\tmsg%d.data = data.var%d; //%s\n'%(count,toDecode.get(canid).index(signals[0])+1,signals[0])
            elif 'Point' in rosmsg:
                ##treat as point
                var = ['.x','.y','.z']
                for i in range(0,len(signals)):
                    text += '\tmsg%d%s = data.var%s; //%s\n'%(count,var[i],i+1,signals[i])

            else:
                print('this rosmg not accounted for: ' + rosmsg)



            text += '\t\t%s_pub.publish(msg%d);\n\n'%(rostopic,count)
        text += '\t}\n'
    text += '}\n'
    return text

def generateToDecode(toROS):
    """Using ROS node JSON/dictionary to generate decoding header json"""

    decode = {}
    for canid in toROS:
#         print(toROS[canid])
        for topic in toROS[canid]:
#             print(toROS[canid][topic][1])
            try:
                decode[canid] += toROS[canid][topic][1]
            except:
                decode[canid] = toROS[canid][topic][1]

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

#             print(rosmsg,rostopic)


            text += '\t%s_pub = n_.advertise<%s>("%s",1000);\n'%(rostopic,rosmsg,rostopic)
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
jsonfile, dbcfile = findDBC(vin_details)
#load the correct DBC file to decode CAN to ROS
dbc = initializeDBC_Cantools(dbcfile)

# Opening ROS JSON file
f = open('./'+jsonfile)

# returns JSON object as a dictionary
print(jsonfile)
toROS = json.load(f, object_hook=keystoint)
toDecode = generateToDecode(toROS)
f.close()
#generating the C++ header file
#TODO: for each key in the decoding dictionary, add text to the can_decode.h file
text = ''
for i in toDecode.keys():
    msg, signals = getSignals(i,toDecode,dbc)
    text += decodeBuilder(msg, signals)
text += '\treturn returnedVal;\n}\n'

#output into C++ file(s)
#TODO make them the real files (after testing)
file1 = open('../include/header_package/can_decode_test.h', 'w')
file2 = open('can_decode_base.h','r')
file1.writelines(file2.read())
file2.close()

file1.writelines(text)
file1.close()
########finished creating can_decode.h

###########Generating the subs_fs.cpp node ########

text = ''
text += buildNode(toROS)

#TODO make into the real file
file1 = open('../src/subs_fs_test.cpp', 'w')
file2 = open('../src/subs_fs_base.cpp','r')

file1.writelines(file2.read())
file2.close()

file1.writelines(text)
file1.close()
########Finished generating subs_fs.cpp node #######


#FINAL: confirm working with comparison of rosbags

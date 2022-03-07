import strym as s
import cantools
import json

#TODO: automatically identifying DBC from VIN
toyota_hybrid = s.initializeDBC_Cantools('../../strym/strym/dbc/toyota_rav4_hybrid.dbc')

def signalCheck(signal):
    print(signal.start,signal.length,signal.scale, signal.offset, signal.unit, signal.is_signed)

def getSignals(key, dictionary, dbc):
    """Inputting the msgID as key, the dictionary of signals to go into ROS, and the dbc where the
    signal decoding is defined. You get the cantools msg and signals back."""
    msg = s.findMessageInfo(key,dbc)
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
    \tbinary = std::bitset<{%d}>(n).to_string(); // convert hex to binary\n\n"%(msg.frame_id,msg.length*8)

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
    rawVal = "\tstd::string raw = binary.substr(%d,%d); //%s\n"%(pos,pos+signal.length,signal.name)

    if not signal.is_signed:
        rawVal_dec = "\traw_dec = std::stoull(raw, 0, 2);\n"
    else:
        rawVal_dec = "\tif ((raw)[0] == '0'){\n\
        \traw_dec= std::stoul( raw, 0, 2 );\n\t}\n\telse {\n\
        \traw_dec = std::stoul(findTwosComplement(raw), 0, 2);\n\
        \traw_dec = \traw_dec * -1.0\n"

    scale = "\tscaled = (float)raw_dec * %0.6f + %0.6f;\n"%(signal.scale,signal.offset)


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


def nodeBuilder(dictionary):
    print('test')
    #for each key in the dict, create a topic w/ msg type and title(key)

    #if messageID == key in the pub's dictionary, decode and assign values,then publish

    #from length of signal dict, know if there are emtpy data fields in msg, assign to 0 if so


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
                text+= '\t\tmsg.header.frame_id = "front_laser_link";\n\
                msg.header.stamp = ros::Time(std::stod(Time));\n'

                text+= '\t\tmsg.point.x = data.var1; //%s\n\
                msg.point.y = data.var2; //%s\n\
                msg.point.z = data.var3; //%s\n' %(signals[0],signals[1],signals[2])#check to see if this works for radar signals, may need to change the decode_message

            elif 'Float64' in rosmsg:
                ##treat as a float64
#                 print(canid,signals)
#                 print(signals in toDecode.get(canid))
                text += '\t\tmsg.data = data.var%d; //%s\n'%(toDecode.get(canid).index(signals[0])+1,signals[0])
            elif 'Point' in rosmsg:
                ##treat as point
                text += '\tmsg.x = data.var1; //%s\n\
                  \tmsg.y = data.var2; //%s\n\
                  \tmsg.z = data.var3; //%s\n'%(signals[0],signals[1],signals[2])

            else:
                print('this rosmg not accounted for: ' + rosmsg)



            text += '\t\t%s_pub.publish(msg);\n\n'%(rostopic)
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

#dict for decoding, TODO: identified from VIN
# Opening JSON file
f = open('./toyota_hybrid.json')

# returns JSON object as
# a dictionary
toROS = json.load(f, object_hook=keystoint)
toDecode = generateToDecode(toROS)
f.close()
#generating the C++ header file
#TODO: for each key in the decoding dictionary, add text to the can_decode.h file
text = ''
for i in toDecode.keys():
    msg, signals = getSignals(i,toDecode,toyota_hybrid)
    text += decodeBuilder(msg, signals)
text += '\n}\n'

#TODO: output into C++ file(s)
# print(text)
file1 = open('can_decode_test.h', 'w')

file2 = open('can_decode_base.h','r')
file1.writelines(file2.read())
file2.close()

file1.writelines(text)
file1.close()
########finished creating can_decode.h

###########Generating the subs_fs.cpp node ########

text = ''
text += buildNode(toROS)

file1 = open('../src/subs_fs_test.cpp', 'w')
file2 = open('../src/subs_fs_base.cpp','r')

file1.writelines(file2.read())
file2.close()

file1.writelines(text)
file1.close()




#NEXT: test in ROS

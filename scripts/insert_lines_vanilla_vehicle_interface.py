#Author: Matthew Nice 

import json
import cantools


def keystoint(x):
    try:
        return {int(k): v for k, v in x.items()}
    except:
        return x

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
    # print('the DBC is set as %s'%(dbcfile))
    if vin_details['Make'] == 'HONDA':
        jsonfile='honda_pilot.json'
        dbcfile = '/home/' + user + '/strym/strym/dbc/honda_pilot_2017.dbc'

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

#/etc/libpanda.d has a JSON with the make,model,trim,year
f = open('/etc/libpanda.d/vin_details.json')
vin_details = json.load(f)
f.close()
#find the correct DBC and ROS msg dict based on the vin details
jsonfile, dbcfile = findDBC(vin_details)
#load the correct DBC file to decode CAN to ROS
if dbcfile is not None:
    dbc = initializeDBC_Cantools(dbcfile)
else:
    dbc = cantools.database.Database()

if jsonfile is not None:
    # Opening ROS JSON file
    f = open('./'+jsonfile)

    # returns JSON object as a dictionary
    toROS = json.load(f, object_hook=keystoint)

    f.close()
else:
    toROS = {}
    
# Defining main function
def main():
    count = 0
    lines = []
    if len(toROS) == 0:
        txt = "\t\ttrue"    # publish EVERYTHING
    else:
        txt = "\t\tfalse"
    lines.append(txt)
    print(txt)
    for i in toROS.keys():
        msg = findMessageInfo(i,dbc)
        #if count ==0:
        #    txt = "\t\t(canData->messageID==%d&&canData->dataLength==%d)"%(i,msg.length)
        #    lines.append(txt)
        #else:
        #    txt = "\t\t||(canData->messageID==%d&&canData->dataLength==%d)"%(i,msg.length)
        #    lines.append(txt)
        txt = "\t\t||(canData->messageID==%d&&canData->dataLength==%d)"%(i,msg.length)
        lines.append(txt)
        print(txt)
        count+=1

    # return lines

if __name__=="__main__":
    main()

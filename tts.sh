#!/bin/bash

pico2wave -w 'sounds/test.wav' "The script is running to generate or update all of the sound files needed." && aplay ./sounds/test.wav


#Welcome to each mode
pico2wave -w 'sounds/mode1welcome.wav' "Welcome to, the can coach experiments. This is the beginning of the drive."
pico2wave -w 'sounds/mode2welcome.wav' "Welcome to, the next section. This is the third test of the drive."
pico2wave -w 'sounds/mode3welcome.wav' "Welcome to, velocity matching. This is the, first test of the drive."
pico2wave -w 'sounds/mode4welcome.wav' "Welcome to Mode 4. This is the fourth test of the drive."
pico2wave -w 'sounds/mode0welcome.wav' "Welcome to, the second test of the drive."

#Introduction to each subsection of each mode

#Intro to Mode 1 subsections
#Normal
pico2wave -w 'sounds/normal.wav' "Please drive, as you would normally."
#Instructed
pico2wave -w 'sounds/instructed0.wav' "Please drive, attemting to maintain, a 2.25 second time gap."
#Coached, reused when needed
pico2wave -w 'sounds/coached.wav' "Please drive, according to the feedback sounds. High pitch, means to speed up, into the target zone. Low pitch, means to slow down, into the target zone."

#Intro to Mode 2 subsections
#Instructed
pico2wave -w 'sounds/instructed2-1.wav' "Please drive, attempting to maintain, a 1.35 second time gap."
pico2wave -w 'sounds/instructed2-2.wav' "Please drive, attemting to maintian, a 1.8 second time gap."
pico2wave -w 'sounds/instructed2-3.wav' "Please drive, attempting to maintain, a 2.25 second time gap."
#Coached
#just use coached.wav

#Intro to Mode 3 subsections
#Instructed
pico2wave -w 'sounds/instructed3.wav' "Please drive, attempting to match, the velocity of the leading car."
#Coached
#just use coached.wav

#Intro to Mode 4 subsections
#Coached
pico2wave -w 'sounds/coached4.wav' "There is no 'lead car' for this section. Continue driving safely, as your first priority. Second, comply best you can, with the audio feedback."


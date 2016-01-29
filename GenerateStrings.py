#!/usr/bin/env python
# coding=utf-8
import os
import string
import sys  
reload(sys)  
sys.setdefaultencoding('utf8')
counter=0
Arabiclist=["ق","ش","ع","ر","ت","ط","و","ي","ه","ة","ث","ظ","ء","ا","س","د","ف","غ","ح","ج","ك","ل","ز","خ","ص","ذ","ب","ن","م","١","٢","٣","٤","٥","٦","٧","٨","٩"]
defString="#define ObfuscateStringList {"
def AddFromRange(range):
	for x in range:
		global defString
		global counter
		counter+=1
		y = ""+str(x)
		c = unichr(int(y, 16))
		defString+="\""+c+"\","
def AddFromList(range):
	for x in range:
		global defString
		global counter
		counter+=1
		defString+="\""+x.encode('utf_8')+"\","
if __name__ == '__main__':
	AddFromList(string.ascii_uppercase+string.ascii_lowercase)
	#AddFromList(Arabiclist)
	defString=defString[:-1]
	defString+="}\n#define ObfuscateListSize "+str(counter)
	Path=os.path.dirname(os.path.abspath(__file__))+"/Strings.h"
	text = open(Path, "w")
	text.write(defString.encode('utf_8'))
	text.close()
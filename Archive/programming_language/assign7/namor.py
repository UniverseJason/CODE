import sys
import re

# this helper function will return int base on roman number character
def getNum(myChar):
    if myChar == 'I':
        return 1
    elif myChar == 'V':
        return 5
    elif myChar == 'X':
        return 10
    elif myChar == 'L':
        return 50
    elif myChar == 'C':
        return 100
    elif myChar == 'D':
        return 500
    elif myChar == 'M':
        return 1000
    else:
        return 0

# this function will use regex to check a string if there are any 4 or more contiguous characters
def checkContiguous(myString):
    myRegex = re.compile(r'(.)\1{3,}')
    if(myRegex.search(myString)):
        return True
    else:
        return False

# this function will convert roman number to integer
def romanToInt(roman):
    romanNum = roman.upper()
    
    # first check if this is a valid roman number
    if(checkContiguous(romanNum)):
        return 0

    # this will go the main algorithm
    # small number is on the right side of the big number, use plus
    # if reverse, then minus
    total = 0
    num = getNum(romanNum[0])
    for i in range(1, len(romanNum)):
        if num >= getNum(romanNum[i]):
            total += num
            num = getNum(romanNum[i])
        else:
            total -= num
            num = getNum(romanNum[i])
    
    total += num

    # check if this will exceed range
    if(total > 3999 or total <= 0):
        return 0
    else:
        return total


for i in range(1, len(sys.argv)):
    result = romanToInt(sys.argv[i])
    if(result == 0):
        print("Error")
    else:
        print(sys.argv[i] + " is " + str(result), end = "\n")
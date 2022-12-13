import sys

# this function will convert integer to roman number
def intToRoman(number):

    if(number <= 0 or number > 3999):
        return 0

    mNum = ["", "M", "MM", "MMM"]
    cNum = ["", "C", "CC", "CCC", "CD", "D", "DC", "DCC", "DCCC", "CM"]
    xNum = ["", "X", "XX", "XXX", "XL", "L", "LX", "LXX", "LXXX", "XC"]
    iNum = ["", "I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX"]

    thousand = mNum[number // 1000]
    hundred = cNum[(number % 1000) // 100]
    ten = xNum[(number % 100) // 10]
    one = iNum[number % 10]

    romaNum = thousand + hundred + ten + one

    return romaNum


for i in range(1, len(sys.argv)):
    result = intToRoman(int(sys.argv[i]))
    if(result == 0):
        print("Error")
    else:
        print(sys.argv[i] + " is " + result, end = "\n")
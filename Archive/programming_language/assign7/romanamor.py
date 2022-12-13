from namor import romanToInt
from roman import intToRoman

errCount = 0

for i in range(1, 4001):
    romaNum = intToRoman(i)
    if romaNum == 0:
        print("Error: try to convert " + str(i) + " to roman number")
        errCount += 1
        continue

    intNum = romanToInt(romaNum)
    if intNum == 0:
        print("Error: try to convert " + romaNum + " to integer")
        errCount += 1
        continue

print("checked " + str(i) + " times with total error: " + str(errCount))

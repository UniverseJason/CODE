import math

# this maxProfit will run O(nlogn) time
def maxProfit(priceList):
	listLength = len(priceList)
	
	# Buy and sell on the same day
	if listLength <= 1:
		return 0

	# Divide-and-Conquer by divided the list by 2
	# use floor divide to spilt the list
	leftList = priceList[0:listLength//2]
	rightList = priceList[listLength//2:listLength]

	# keep finding the left and right by Recursion, only for buy and sell on the same list
	LmaxProfit = maxProfit(leftList)
	RmaxPrifit = maxProfit(rightList)

	# if meet the crossing from left to right (should buy on left list and sell on right list)
	# since the buy day must before the sell day,
	# then the sell day must in right list and buy day must in left list.
	# then just find the max in the right for sell day, min in the left list
	crossMaxProfit = max(rightList) - min(leftList)

	# max profit must left in these three list
	maxProfitResult = max(LmaxProfit, RmaxPrifit, crossMaxProfit)
	return maxProfitResult

# This is the helper function to build list for given file name
def buildList(fileName):
	inFile = open(fileName,"r")
	floatNumList = []
	
	for line in inFile:
		currentNum = line.strip('\n')
		floatNumList.append(float(currentNum))

	return floatNumList


bitCoinList = buildList("bitcoin.txt")
gameStopList = buildList("gamestop.txt")
largeInput = buildList("largeInput.txt")
nasdaqList = buildList("nasdaq.txt")
smallInputList = buildList("smallInput.txt")

print("bitcoin.txt: The optimal solution for bitcoin.txt is " + str(round(maxProfit(bitCoinList),2)))
print("gamestop.txt: The optimal solution for gamestop.txt is " + str(round(maxProfit(gameStopList),2)))
print("largeInput.txt: The optimal solution for largeInput.txt is " + str(round(maxProfit(largeInput),2)))
print("nasdaq.txt: The optimal solution for nasdaq.txt is " + str(round(maxProfit(nasdaqList),2)))
print("smallInput.txt: The optimal solution for smallInput.txt is " + str(round(maxProfit(smallInputList),2)))


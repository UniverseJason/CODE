import string
import math
from selenium import webdriver
from selenium.webdriver.common.by import By

# Function
def isNum(value):
    try:
        value + 1
    except TypeError:
        return False
    else:
        return True


# get chrome and website path
# Need clear path for Chrome Driver
option = webdriver.ChromeOptions()
option.add_argument("--user-data-dir="+r"C:/Users/01120/AppData/Local/Google/Chrome/User Data")
driver = webdriver.Chrome(options = option) 
driver.get('https://www.forbes.com/profile/howard-marks/?list=forbes-400&sh=72f563fa7b2d')

# personalnailze
driver.set_window_size(width=500, height=500, windowHandle="current")
driver.set_window_position(x=1000, y=100, windowHandle='current')


# loop start
for i in range(400):
	# 1.1 : get index and head name of the person
	driver.implicitly_wait(10) # seconds
	heading1 = driver.find_element_by_tag_name("h1").text
	removeOne = heading1.replace("#","")
	final = removeOne.replace(",","")
	name = final.split()

	# 1.2 : print index and name
	for i in name:
		if i.isnumeric():
			print(i + ',' , end = '')
			continue
		print(i + ' ', end = '')
	print(',' , end = '')

	
	# 2.1 : get person state
	stateName = driver.find_elements_by_class_name("profile-stats__title")
	state = driver.find_elements_by_class_name("profile-stats__text")
	netWorth = driver.find_elements_by_class_name("profile-info__item-value")

	# ageIDX = 0
	# wealthIDX = 0
	# counter = 0
	# for i in stateName:
	# 	if i.text == "AGE":
	# 		ageIDX = counter
	# 	if i.text == "SOURCE OF WEALTH":
	# 		wealthIDX = counter
	# 	counter += 1
	# print(netWorth[len(netWorth) - 1].text + "," + state[ageIDX].text + "," + state[wealthIDX].text, end = '')

	# 2.1.1 get education infomation and remove ';' and split them by ','
	edu = state[len(state) - 1].text
	change = edu.maketrans(";", ",")
	edu = edu.translate(change)
	separateList = edu.split(",")
	length = len(separateList)

	# 2.2 split degree and university by required format
	degree = [0 for i in range(math.ceil(length/2))]
	university = [0 for i in range(math.ceil(length/2))]
	j = 0
	x = 0
	for i in range(length):
		if i % 2 == 0:
			degree[j] = separateList[i]
			j += 1
		else:
			university[x] = separateList[i]
			x += 1

	# 2.3 print university and degree
	for i in range(len(university)):
		if isNum(university[i]) or isNum(degree[i]):
			continue

		else:
			print(university[i] + "  ", end = '')
	print(",", end = '')

	for i in range(len(degree)):
		if isNum(degree[i]):
			continue

		else:
			print(degree[i] + "  ", end = '')
	print(",", end = '')

	# current URL
	currentUrl = driver.current_url
	print(currentUrl + ", ", end = '')

	# get and print citizenship
	counter = 0
	ResidenceIdx = 0
	CitizenIdx = 0
	for i in stateName:
		if i.text == "RESIDENCE":
			ResidenceIdx = counter
		if i.text == "CITIZENSHIP":
			CitizenIdx = counter
		counter += 1
	print(state[ResidenceIdx].text + "," + state[CitizenIdx].text, end = '')

	# go to next page
	# since it include href element, need get its url for next page
	element = driver.find_element(By.CSS_SELECTOR, ".profile-nav__next")
	url = element.get_attribute("href");
	driver.get(url);

	# new line
	print("")
# loop end

driver.quit()
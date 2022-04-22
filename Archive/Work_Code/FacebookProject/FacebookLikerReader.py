'''
if there are any webpage button not working, use js click()
for example
driver.execute_script("arguments[0].click();", pageBut) # use Js to click since it is not work for regular click()
'''
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.common.action_chains import ActionChains
from selenium.webdriver.common.keys import Keys
from time import sleep
from itertools import groupby
import re
import string

# function
def ListChecker(iterable):
    g = groupby(iterable)
    return next(g, True) and not next(g, False)

def hardOperation():
	MAX_RETRY = 5
	temp_retry = 1
	while True:
		if temp_retry == MAX_RETRY:
			ETLink = driver.find_element_by_xpath("//div[@class='popover-trigger inline']//a[@aria-label='view post on Facebook']").get_attribute("href")
			driver.get(ETLink) 
			FBLink = driver.find_element_by_xpath("//div[@class='fb-post fb_iframe_widget']").get_attribute("data-href")
			reaction = "REACTED or ENGAGE"
			break
		try:
			engageBut = driver.find_element_by_xpath("//a[@role='button' and @aria-label='Engagement']")
			driver.execute_script("arguments[0].click();", engageBut)
			driver.implicitly_wait(5)
			FBName = driver.find_element_by_xpath("//div[@class='engagement-card--title']//strong[@class='fs-exclude-data']").text
			reaction = driver.find_element_by_xpath("//div[@class='engagement-card--title']//span[1]//strong").text
			ETLink = driver.find_element_by_xpath("//div[@class='engagement-card--title']//div[@role='button']//a").get_attribute("href")
			driver.get(ETLink) 
			FBLink = driver.find_element_by_xpath("//div[@class='fb-post fb_iframe_widget']").get_attribute("data-href")
			temp_retry = 1
			break
		except:
			driver.refresh()
			temp_retry += 1
	print(FBName + " " + reaction + ": " + FBLink)

# main code start
option = webdriver.ChromeOptions()
option.add_argument("--user-data-dir="+r"C:/Users/01120/AppData/Local/Google/Chrome/User Data")
driver = webdriver.Chrome(options = option) 
# this line has been removed for security purpose
# sleep(3)

# # get url element
# inFile = open("TempList.txt", "w")
# for i in range(12):
# 	sleep(7)
# 	elems = driver.find_elements_by_xpath("//div[@class='new-contact-card--content']//a[@aria-label]") # find link for each person
# 	for elem in elems:
# 		inFile.write(elem.get_attribute("href") + "\n")
# 	elems.clear()
# 	# go to the next page
# 	nextPageBut = driver.find_element_by_xpath("//li[@aria-label='Next Page']")
# 	driver.execute_script("arguments[0].click();", nextPageBut)
# inFile.close()

# operation in personal page (LOOP)
inFile = open("TempList.txt", "r")
for url in inFile:
	driver.get(url)
	driver.implicitly_wait(5)
	name = driver.find_element_by_xpath("//h3[contains(@class,'profile-header--name')]//span[@class='fs-exclude-data']")
	rawInfo = driver.find_element_by_xpath("//div[@class='profile-header--user']//div[2]").text
	info = re.sub("[^0-9]", "", rawInfo)
	name = name.text.replace(',','')
	print(info + "," + name + ",", end='')

	# check if there is element in the enrichment
	enrichBut = driver.find_element_by_xpath("//a[@role='button' and @aria-label='Enrichment']").click()
	driver.implicitly_wait(2)
	enrichProfile = driver.find_elements_by_xpath("//div[@class='profile--enrichment-social-profile-name']//a[@aria-label]")
	
	# if no element under enrich, find the target post page
	if len(enrichProfile) == 0: 
		print("H,", end='')
		hardOperation()
	else: 
		# first find if there is facebook
		Checker = True
		textChecker = re.search("facebook", enrichProfile[0].text, re.IGNORECASE)
		urlChecker = re.search("facebook", enrichProfile[0].get_attribute("href"), re.IGNORECASE)
		if not textChecker and not urlChecker: # not found
			Checker = False

		# do the operation base on the final checker
		if Checker:
			print("E,", end='')
			for ele in enrichProfile:
				print(ele.text + ": " + ele.get_attribute("href") + ",", end='')
			print("")
		else:
			print("H,", end='')
			for ele in enrichProfile:
				print(ele.text + ": " + ele.get_attribute("href") + ",", end='')
			hardOperation()
# End Loop

driver.quit()
import string
import xlsxwriter
from selenium import webdriver
from selenium.webdriver.common.by import By

option = webdriver.ChromeOptions()
option.add_argument("--user-data-dir="+r"C:/Users/01120/AppData/Local/Google/Chrome/User Data")
driver = webdriver.Chrome(options = option) 
driver.get('https://www.forbes.com/profile/william-stone/?list=forbes-400')

# personalnailze
driver.set_window_size(width=500, height=500, windowHandle="current")
driver.set_window_position(x=1000, y=100, windowHandle='current')

# read file
workbook = xlsxwriter.Workbook('ProNote.xlsx')
worksheet = workbook.add_worksheet()
row = 0
col = 0


for i in range(2):
	try:
		# 1 : get index and head name of the person
		driver.implicitly_wait(10) # seconds

		infor = driver.find_element_by_class_name("profile-text")
		worksheet.write(row, col, infor.text)
		
		# next page
		element = driver.find_element(By.CSS_SELECTOR, ".profile-nav__next")
		url = element.get_attribute("href");
		driver.get(url);
		
		row += 1
		print(row)
	except:
		print("\n Error at row:" + str(row))
		print(url)
		driver.quit()
		workbook.close()
		break

# loop end
print(url)
driver.quit()
workbook.close()

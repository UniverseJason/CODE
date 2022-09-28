from io import TextIOWrapper
import random
import time
import traceback
import myDatabase
from selenium import webdriver
from selenium.webdriver.chrome.service import Service
from webdriver_manager.chrome import ChromeDriverManager
from selenium.webdriver.support import expected_conditions as EC
from selenium.webdriver.common.by import By

# this function will search answer in the set, return null if not found
def searchAnswer(QAset : myDatabase, question : str, questionType : str):
    answer = "null"
    for i in range(len(QAset)):
        if QAset[i].getQuestion() == question:
            if questionType == "Multiple Select Question":
                answer = QAset[i].getMutiSelectAns()
            else:
                answer = QAset[i].getAnswer()
            break
    return answer

def archive(question, answer, file, Qcount):
    file.write("Q" + str(Qcount) + ": " + question + "\nA: " + answer + "\n\n")
    return


# information
inFile = open("ch14_collection.txt", "w")
timeout = 5
username = "ycy354"
password = "longworld1115168"

# chapter webpage in new tap
chapterInfo = "https://utsa.blackboard.com/webapps/blackboard/content/listContent.jsp?course_id=_225599_1&content_id=_7550201_1"

# get the driver
driver = webdriver.Chrome(service=Service(ChromeDriverManager().install()))
driver.get('https://my.utsa.edu/')

# click the login button of the black board
driver.implicitly_wait(timeout)
driver.find_element(By.LINK_TEXT, 'Blackboard Learn').click()

# login
driver.implicitly_wait(timeout)
driver.find_element(By.ID, 'form-username').send_keys(username)
driver.find_element(By.ID, 'form-password').send_keys(password)
driver.find_element(By.NAME, '_eventId_proceed').click()

# enter the homework
driver.implicitly_wait(timeout)
driver.get(chapterInfo)
myLink = driver.find_element(By.XPATH, "(//div[@class='item clearfix'])[1]/h3/a").get_attribute('href')
driver.get(myLink)

# entering the list of questions
driver.implicitly_wait(timeout)
driver.find_element(By.XPATH, "//button[normalize-space()='Begin']").click()
driver.implicitly_wait(timeout)
try:
    driver.find_element(By.XPATH, "//button[normalize-space()='Continue Questions']").click()
except:
    driver.find_element(By.XPATH, "//button[normalize-space()='Start Questions']").click()

driver.implicitly_wait(timeout)
try:
    driver.find_element(By.XPATH, "//button[normalize-space()='Got It']").click()
except:
    pass

QASet = []          # store the question and answer

# get the Question state count
state = driver.find_element(By.XPATH, "//b[@class='pw__concepts-count']").text
state_data = state.split(" of ")
currentNum = int(state_data[0])
totalNum = int(state_data[1])
Qcounter = 0

try:
    while currentNum < totalNum:
        ifSupport = True   # if the question is support question
        ifFound = False     # if the question is found in the choice list
        answerText = []     # for multi-select only
        answer = ""         # for single-select only

        # analyze the questions mod
        driver.implicitly_wait(timeout)
        question_mod = driver.find_element(By.XPATH, "//h2").text
        questionText = driver.find_element(By.XPATH, "//div[@class='prompt']/p").text
        key = searchAnswer(QASet, questionText, question_mod) # search in the set

        # 1 fill-in-the-blank question
        if question_mod == "Fill in the Blank Question":
            # send the key
            blank_list = driver.find_elements(By.XPATH, "//span[@class='input-container span-to-div']/input")
            for bk in blank_list:
                bk.send_keys(key)
            driver.find_element(By.XPATH, "//button[normalize-space()='High']").click()
            driver.implicitly_wait(timeout)
            answer = driver.find_element(By.XPATH, "(//span[@class='correct-answer'])").text

        # 2 multi-choice/TF question
        elif question_mod == "Multiple Choice Question" or question_mod == "True or False Question":
            # get the answer list and compare with the key
            choice_list = driver.find_elements(By.XPATH, "(//mhe-radio-button)")
            if key != "null":
                for choice in choice_list:
                    if choice.text == key:
                        print("Founded the answer!\n")
                        ifFound = True
                        choice.click()
                        break
            else:
                choice_list[random.randint(0, len(choice_list)-1)].click()
            
            # confirm and get the answer
            driver.find_element(By.XPATH, "//button[normalize-space()='High']").click()
            if question_mod == "Multiple Choice Question":
                answer = driver.find_element(By.XPATH, "(//div[@class='choiceText rs_preserve'])").text
            else:
                answer = driver.find_element(By.XPATH, "(//div[@class='answer-container'])").text

        # 3 multi-select question
        elif question_mod == "Multiple Select Question":
            # get the answer list and compare with the key
            choice_list = driver.find_elements(By.XPATH, "(//mhe-checkbox)")
            if key != "null":
                for choice in choice_list:
                    for j in range(len(key)):
                        if choice.text == key[j]:
                            ifFound = True
                            choice.click()
            else:
                # generate 2 different random numbers
                random_num1 = random.randint(0, len(choice_list)-1)
                random_num2 = random.randint(0, len(choice_list)-1)
                while random_num1 == random_num2:
                    random_num2 = random.randint(0, len(choice_list)-1)
                # select the two random choice
                choice_list[random_num1].click()
                choice_list[random_num2].click()
            
            # confirm and get the answer
            driver.find_element(By.XPATH, "//button[normalize-space()='High']").click()
            answerBoxList = driver.find_elements(By.XPATH, "(//div[@class='choiceText rs_preserve'])")
            for i in range(len(answerBoxList)):
                answerText.append(answerBoxList[i].text)
            answer = "\n".join(answerText)
            
        else:
            time.sleep(20)
            ifSupport = False
            print("Error: the question type is not supported yet!\n")
            continue
        
        if ifFound == False:
            Qcounter += 1
            data = myDatabase.myDatabase(questionText, answer, answerText)
            QASet.append(data)
            archive(questionText, answer, inFile, Qcounter)
        
        # update the counter
        state = driver.find_element(By.XPATH, "//b[@class='pw__concepts-count']").text
        state_data = state.split(" of ")
        currentNum = int(state_data[0])

        # check if need resource and then click the next button
        alertTagBt = driver.find_element(By.XPATH, "//awd-lr-buttons-tray[@aria-label='Concept Resources']")
        if "Select a concept resource to continue." in alertTagBt.text:
            read_aboutBt = driver.find_element(By.XPATH, "//button[@class='btn ngx-shared btn-tertiary lr-tray-button']")
            driver.execute_script("arguments[0].scrollIntoView();", read_aboutBt)
            driver.execute_script("arguments[0].click();", read_aboutBt)
            driver.implicitly_wait(timeout)
            driver.find_element(By.XPATH, "//button[normalize-space()='To Questions']").click()
        
        driver.find_element(By.XPATH, "//button[normalize-space()='Next Question']").click()
        time.sleep(1)
    # end of the loop

except:
    traceback.print_exc()

# end the driver
if ifSupport == True:
    time.sleep(10)
    inFile.close()
    driver.quit()
else:
    inFile.close()
    print("Need manually!\n")
#!/usr/bin/python3
import glob
import ntpath
import sys
import os

if(len(sys.argv)<5):
    print("USAGE ERROR: assign4.py dataDir template date outputDir")
    exit(1)

# dirPath pass by arg[1]
# dirPath = "./data"
dirPath = sys.argv[1]
coursePath = dirPath + "/*.crs"
courseList = glob.glob(coursePath)

# template file pass by arg[2]
# temp_file_name = "assign4.template"
temp_file_name = sys.argv[2]
temp = open(temp_file_name,'r')

# date pass by arg[3]
# date = "10/14/2021"
date = sys.argv[3]

# output file dir pass by arg[4]
# output = "./output"
output = sys.argv[4]

# loop through the whole folder
for filename in courseList:
    with open(filename,'r') as inFile:
        # get the name from the file name
        courseFileName = (ntpath.basename(filename)).split('.')[0]
        course_num = courseFileName[-4:]

        # get the department information form first line
        dept = inFile.readline().split(' ')
        dept_code = dept[0]
        dept_name = dept[1].strip()
        
        # get the full course name form the second line
        course_name = inFile.readline().strip()

        # get the course time information from third line
        course_time_line = inFile.readline().split(' ')
        course_sched = course_time_line[0]
        course_start = course_time_line[1]
        course_end = course_time_line[2].strip()

        # get the credit hours form fourth line
        credit_hours = inFile.readline().strip()
    
        # get number of the students form the last line (fifth line)
        num_stu = int(inFile.readline().strip())
    # END of the with as
    
    # if have more than 30 student, generate warning
    if(num_stu >= 30):
        # check directory & .warn file exist
        if(os.path.isdir(output) == False):
            os.mkdir(output)
        outputFile = open(output + "/" + courseFileName + ".warn",'w')
    
        # parsing file according to template
        temp = open(temp_file_name,'r')
        temp_data = temp.read()
        temp_data = temp_data.replace("[[dept_code]]",dept_code)
        temp_data = temp_data.replace("[[dept_name]]",dept_name)
        temp_data = temp_data.replace("[[course_name]]",course_name)
        temp_data = temp_data.replace("[[course_start]]",course_start)
        temp_data = temp_data.replace("[[course_end]]",course_end)
        temp_data = temp_data.replace("[[credit_hours]]",credit_hours)
        temp_data = temp_data.replace("[[num_students]]",str(num_stu))
        temp_data = temp_data.replace("[[course_num]]", course_num)
        temp_data = temp_data.replace("[[date]]",date)
        outputFile.write(temp_data)
        temp.close()
        outputFile.close()
    # END of the if

# END of the for in loop
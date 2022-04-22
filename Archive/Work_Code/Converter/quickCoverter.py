from os import write
import string

inFile = open("Converter\Sex.txt")
outFile = open("Converter\Result.txt",'w')

def sexConvert():
	for i in inFile:
		if i == "Male\n":
			outFile.write("Father\n")
		elif i == "Female\n":
			outFile.write("Mother\n")
		else:
			outFile.write("-1\n")

def titleConvert():
	for i in inFile:
		if i == "Male\n":
			outFile.write("Mr.\n")
		elif i == "Female\n":
			outFile.write("Mis.\n")
		else:
			outFile.write("-1\n")

def SDConvert():
	for i in inFile:
		if i == "Male\n":
			outFile.write("Son\n")
		elif i == "Female\n":
			outFile.write("Daughter\n")
		else:
			outFile.write("-1\n")

titleConvert()
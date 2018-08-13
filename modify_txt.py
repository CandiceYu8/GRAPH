""" add weight as the second number in each line """

## extern library
import random


## start from here
readFile = open("input1.txt", 'r')
writeFile = open("input2.txt", 'w')
readLines = readFile.readlines()
for line in readLines:
    line = line.split()
    writeFile.write(line[0] + " ")
    writeFile.write(str(random.randint(1,60)) + " ")
    writeFile.write(line[1] + "\n")

readFile.close()
writeFile.close()

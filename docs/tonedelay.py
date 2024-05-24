
import re

file = 'eva.ino'

tone = []
delay = []
length=0
with open(file,'r') as r:
    for line in r.readlines():
        l = line.lstrip(' ')
        if l[0:4] == 'tone':
            t = re.search('\d+',l).group()
            tone.append(float(t))
            length+=1
            
        elif l[0:5] == 'delay':
            d = re.search('\d+',l).group()
            delay.append(float(d))

print(tone)
print("\n")
print("##################################################################\n")
print(length)
print("\n")
print("##################################################################\n")
print("##################################################################\n")
print(delay)

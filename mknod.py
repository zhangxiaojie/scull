import os
file=open('/proc/devices')
for line in file:
    if('blan' in line):
        major,devname=line.split()
ret=os.popen('mknod /dev/'+devname+' c '+major+' 0')
f=open('/dev/blan','rb')
f.close()
file.close()

import os
file=open('/proc/devices')
for line in file:
    if('scull' in line):
        major,devname=line.split()
ret=os.popen('mknod /dev/'+devname+' c '+major+' 0')
f=open('/dev/scull','rb')
f.close()
file.close()

import os
myCmd = 'speedtest-cli --no-download > upload.txt'
os.system(myCmd)
myCmd2 = 'speedtest-cli --no-upload > download.txt'
os.system(myCmd2)
#27031998

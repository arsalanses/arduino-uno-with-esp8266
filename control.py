# WiFi Controlled LED using Arduino and ESP8266

import urllib.request
import requests

print('Enter the IP address of ESP8266\nESP8266 IP Address:')
ip = '192.168.1.100'# input()

url = 'http://' + ip + '/?led='

while(True):
    print('Turn led(ON or OFF):')
    status = input()

    if(status == 'ON'):
        content = urllib.request.urlopen(url + 'ON')
        print('STATUS: LED is ON!')
    else:
        # content = urllib.request.urlopen(url + 'OFF').read()
        r = requests.get(url + 'OFF')
        print('STATUS: LED is OFF!')

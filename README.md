# Interative ReverseShell bypass Microsoft Window Defender

A simple ReverseShell written in C++, and listening server written in Python. It is not detected by MDE (tested on 8/20/2024 : Windows 10 22H2 latest)

## I. Installation

- ***Mingw***:
    *note that I try to use Microsoft Visual Studio to build exe but I got a lot of errors with lib and header file, you can use MVS if you want, or install Mingw like me*

```bash
sudo apt-get update
sudo apt-get install mingw-w64 -y
```
## II. Usage
### 1. Build virus
***Note:*** *Inspect source in virus.cpp and change sourceip and port to your remote server that hosts server.py (receive shell from victim) then build it using mingw*
```bash
x86_64-w64-mingw32-g++ -o notepad.exe virus.cpp  -static-libgcc -static-libstdc++ -lwininet
```

### 2. Run listening server
***Note:*** *This server.py will listen incomming GET request, extract Agent header and decode to human readable data (result returns from command)*
```python
python3 server.py
```

Run notepad.exe connection will be established to remote server, run any command.

**Server**:

![plot](./images/serverpy.png)

**Client**

![plot](./images/viruscpp.png)

> [!CAUTION]
> There is no clean code, no penetration test provided, this repo is for educational purposes only. Do not use for illegal purposes. I'm never responsible for illegal use. Educational purpose only!
## Support me (optional)
If you find it useful, you can support me with a cup of coffee.
[![ko-fi](https://ko-fi.com/img/githubbutton_sm.svg)](https://ko-fi.com/Y8Y2123O0D)

# Minecraft (RubyDung?) rd-131655 (pre-Classic) — in C/C++17
*Cave game tech test/Cave Game tech demo*
<br>
**Heavily inspired by [The Code Of Notch](https://github.com/thecodeofnotch/)**

## Features
This project includes the same features found in *The Code of Notch's* repository (https://github.com/thecodeofnotch/rd-131655):
- Cave generation
- 8x8x8 Chunks

## References
- [Minecraft Wiki - Java_Edition_pre-Classic_rd-131655](https://minecraft.gamepedia.com/Java_Edition_pre-Classic_rd-131655)


## Setup
This project is designed for Windows. It might compile on macOS or Linux, but I haven't tested it—no precompiled libraries were used.
1. Clone the project using ```git clone```
2. Compile ```x86_64``` using CMake

## Warnings
#### Avoid replacing GLAD with a different version (newer or older). If you do, you’ll need to comment out the following line near the top of the code:
```#error OpenGL header already included, remove this include, glad already provides it``` 
#### Change it into:
 ```//#error OpenGL header already included, remove this include, glad already provides it```
##### *Why? Because of my "insane" programming skills.*


## Contributing
Feel free to open pull requests or issues as needed.

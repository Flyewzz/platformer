# Nolibgs Hello Worlds !

<p align="center">

<img height="240px" src="http://wiki.arthus.net/assets/cube.gif" alt="3D power !">

<img height="240px" src="http://wiki.arthus.net/assets/polyfun.jpg" alt="3D power !">

<img height="240px" src="http://wiki.arthus.net/assets/hello_gt.jpg" alt="3D power !">

</p>

So you want to begin developping on the original PSX but don't know where to start ?  

This repo is destined to host a bunch of simple examples, each describing how to do one thing.  

The code here will be using **Nugget + PsyQ**, the "Official" Sony SDK but with a modern MIPS toolchain.  

We will not be using libGS, the Extended Graphics Library for the graphic stuff...  

Instead we'll try to devise methods to reproduce libgs functions. This will not necessarly be more efficient, but we'll learn
a lot more stuff !  

## Installation

We'll keep things simple for now. If you want to read about more methods to get things up and running, see the wiki's [Installation methods](https://github.com/ABelliqueux/nolibgs_hello_worlds/wiki/Installation-methods) section. 

### Windows

#### MIPS toolchain setup
1. Download the MIPS toolchain here : [https://static.grumpycoder.net/pixel/mips/g++-mipsel-none-elf-10.3.0.zip](http://static.grumpycoder.net/pixel/mips/g++-mipsel-none-elf-10.3.0.zip)
2. Extract the archive's content in `C:\g++-mipsel-none-elf-10.3.0` and add the `C:\g++-mipsel-none-elf-10.3.0\bin` folder to [your $PATH](https://stackoverflow.com/questions/44272416/how-to-add-a-folder-to-path-environment-variable-in-windows-10-with-screensho#44272417).
3. Test everything is fine by [launching a command prompt](https://www.lifewire.com/how-to-open-command-prompt-2618089) and typing `mipsel-none-elf-gcc.exe --version`. If you get a message like `mipsel-none-gnu-gcc (GCC) 10.3.0`, then it's working !

#### Nugget + PsyQ setup
1. Download the PsyQ converted libraries here : [http://psx.arthus.net/sdk/Psy-Q/psyq-4.7-converted-full.7z](http://psx.arthus.net/sdk/Psy-Q/psyq-4.7-converted-full.7z)  
2. Clone the 'nolibgs_hello_worlds' repo with  
`git clone https://github.com/ABelliqueux/nolibgs_hello_worlds.git --recursive`  
or download this [repository's release](https://github.com/ABelliqueux/nolibgs_hello_worlds/releases/download/v0.1/nolibgs_hello_worlds.zip) and extract **`nolibgs_hello_worlds.zip`**'s content to `C:\no_libgs_hello_worlds\` .
3. Extract the content of `psyq-4.7-converted-full.7z` in `C:\no_libgs_hello_worlds\psyq`. You should now have `C:\no_libgs_hello_worlds\psyq\include` and `C:\no_libgs_hello_worlds\psyq\lib` ;
```
no_libgs_hello_worlds
├── common.mk
├── hello_world
|       ├── hello_world.c
|       ├── Makefile
├── hello_...
└── psyq
    ├── lib
    |    └── *.a 
    └── include
         └── *.h  
```
4. Test everything is working by [launching a command prompt](https://www.lifewire.com/how-to-open-command-prompt-2618089), change to the `C:\no_libgs_hello_worlds\` directory with the following command: `cd C:\no_libgs_hello_worlds\`, then type `make` and hit enter.  
By default, this should build the `hello_world` example, and you should now have a `hello_world.ps-exe` file in `C:\no_libgs_hello_worlds\hello_world`. This a PSX executable that can be run in an emulator like [pcsx-redux](https://github.com/grumpycoders/pcsx-redux/).

### Linux 

#### Install your distribution's MIPS toolchain

In a terminal :

On Debian derivatives (Ubuntu, Mint...) :
```bash
sudo apt-get install gcc-mipsel-linux-gnu g++-mipsel-linux-gnu binutils-mipsel-linux-gnu
```
On Arch derivatives (Manjaro), the mipsel environment can be installed from [AUR](https://wiki.archlinux.org/index.php/Aur) : [cross-mipsel-linux-gnu-binutils](https://aur.archlinux.org/packages/cross-mipsel-linux-gnu-binutils/) and [cross-mipsel-linux-gnu-gcc](https://aur.archlinux.org/packages/cross-mipsel-linux-gnu-gcc/) using your [AURhelper](https://wiki.archlinux.org/index.php/AUR_helpers) of choice:
```bash
trizen -S cross-mipsel-linux-gnu-binutils cross-mipsel-linux-gnu-gcc
```
#### Nugget + PsyQ setup

Let's do it all on the [CLI](https://en.wikipedia.org/wiki/Command-line_interface) !

 1. Install the git client :
```bash
sudo apt-get install git
```
 2. Clone this repository :
```bash
git clone https://github.com/ABelliqueux/nolibgs_hello_worlds.git --recursive
```
 3. Change to the repo's directory and get the PsyQ converted libraries:
```bash
cd nolibgs_hello_worlds
wget http://psx.arthus.net/sdk/Psy-Q/psyq-4.7-converted-full.7z
7z x psyq-4.7-converted-full.7z -o./psyq
```
 4. Try your setup :
```bash
make
```
By default, this should build the `hello_world` example, and you should now have a `hello_world.ps-exe` file in `./hello_world/`. This a PSX executable that can be run in an emulator like [pcsx-redux](https://github.com/grumpycoders/pcsx-redux/).

### MacOS

A [brew](https://brew.sh/) installation script can be found [here.](https://github.com/grumpycoders/pcsx-redux#macos).  

## Compilation

In a terminal, `cd` to your psxdev setup directory and type `make all` to build all examples in their respective directories.  

Alternatively, you can use `make example_name` to only build that example, i.e : `make hello_poly`.  

If you want to remove all the files generated by the compilation process, type `make clean`.  

## Upcoming examples

  * hello_poly_subdiv (polygon subdivision)
  * hello_rsd (rsd format)

# Links and Doc
  
  * [Getting started with PSX dev](https://psx.arthus.net/starting.html)
  * [Ps1 dev ressource center](https://ps1.consoledev.net/)
  * [PsyQ docs](https://psx.arthus.net/sdk/Psy-Q/DOCS/)
  * [psxdev.net](http://psxdev.net/)
  * [psxdev Discord](https://discord.com/invite/N2mmwp)

# Credits, thanks, hugs

Everything here was learnt from some more talented persons, mainly but not excluding others that hang around on the [psxdev discord](https://discord.com/channels/642647820683444236/642848627823345684)
Nicolas Noble, Lameguy64, NDR008, Jaby smoll seamonstah, danhans42, rama, sickle, paul, squaresoft74, and lot mores !

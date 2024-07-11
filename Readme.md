# ETSI-ITS Parser
First of all, let me begin with a preamble. <br>
_AAAAAAAAAAAAAAAAAAAAaaaaAAAAAAAAAAAAAaaaaAAAaaaaaaaaaaaaaaaaaaAAAAAAAAAAaaaaAAAAaaaaaaaaaAAaaaaAAAaaaaAAAaaaAAAAAAAaaaAAAaaaAAAAaaaAAAaaaaAAAaaAAAAaahhHHHHHHHHhhHHHHHHHHHhHHHhhhhHHHhHhhHhHHHhhH!!!!!!!_

Now about the main points: <br>
This is a piece of software designed to convert ETSI-ITS packets stored in `.pcap` file  into usable `.json` structures.
The whole code is wretched beyond reality of the mortal plane as it is written in the _Accursed Language_, God forgive
me for uttering its name, `C++`.

## How do I use it?
### Use provided release
Well there is a release version provided, it works on x64 Linux and is kinda ok.

### Compile it
If you want to compile it, you will have to install several dependencies:
- for the [special Vanetza version](https://code.nap.av.it.pt/mobility-networks/vanetza)
    you will need prerequisites as stated at [Vanetza website](https://www.vanetza.org/how-to-build/#prerequisites)
  - Boost 1.58 or higher, this is kinda ok, as boost is usually
    already provided in most Linux distros
  - GeographicLib 1.37 or higher
  - Crypto++ 5.6.1 or higher
- Vanetza is already included as git submodule, so it is sufficient to hit
    `git submodule update --init --recursive` and it should import it
- RapidJSON is part of the improved Vanetza library
- PcapPlusPlus is included via [vcpkg manifest](vcpkg.json) I do not understand how it is supposed to work as it does
    not, so you are on your own

Now just like compile as you usually do using CMake (personally I do not know a thing about CMake, so I let CLion do
it for me).

### Now the real usage
Just call it as `etsi-its-parser [my/pcap/file.pcap] [hopefully/a/json/file.json]`

It should take just a few moments to actually parse the data, and you should have a standardized `.json` file.

## How does it work?
I botched together PcapPlusPlus, RapidJSON and Vanetza libraries (actually Vanetza and RapidJSON integration is not
botched as I mostly just took the finished work at https://code.nap.av.it.pt/mobility-networks/vanetza).

By ripping code from the `vanetza::geonet::Router` class I managed to properly resolve the geonetworking mess.
Currently, this software works only witch `radiotap` (Wi-Fi) physical layer, as it is the physical layer we work with,
implementing ethernet or something else should be doable.

In the future I may actually write some comments explaining the thing.

## I need to somehow edit it/fix something.
Several things:
1. RIP
2. Good luck

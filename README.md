# CowByte Engine
A game engine by [Roy Niu](mailto:yanqi.roy.niu@gmail.com).

Don't worry about the name.

### Overview
- This is a hobby game engine project by Roy Niu.
- Currently the project is on Ver 0.2, please find the meat in [CowByte/src/CowByteEngine 0.2/](https://github.com/yanqiniu/CowByte/tree/develop/src/CowByteEngine%200.2) 
- In most of the cases develop is the active branch.


### Build
 - The build process still needs to be streamlined...
 - The project uses DirectX 11 SDK. Download [here](https://www.microsoft.com/en-us/download/details.aspx?id=6812)
 - Uses Windows 10 SDK 10.0.16299.0. Older kits might be supported as well but not tested yet.
 - Solution created using Visual Studio 2017.
 - Uses some more recent C++ features like binary literals (C++14) and delete(void* ptr, size_t size) overload (C++17) which should be included with the latest VC++ that comes with VS2017.


### Features
 - As mentioned above, this engine uses DirectX 11. Might support DX12 later. 
 - Implements component model with message bus. 
 - Custom memory allocator (sets of pools).
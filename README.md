# CowByte Engine
A game engine by [Roy Niu](mailto:yanqi.roy.niu@gmail.com).  
![Cow Bite](https://github.com/yanqiniu/CowByte/blob/master/assets/other/cover.jpg)  
Don't worry about the name :D


### Overview
- This is a hobby game engine project by Roy Niu.
- Currently the project is on Ver 0.2, please find the meat in [CowByte/src/CowByteEngine 0.2/](https://github.com/yanqiniu/CowByte/tree/develop/src/CowByteEngine%200.2) 
- In most of the cases develop is the active branch.


### Build
 - The build process still needs to be streamlined...
 - The project uses DirectX 11 SDK. Download [here](https://www.microsoft.com/en-us/download/details.aspx?id=6812).
 - Uses Windows 10 SDK 10.0.16299.0. Older kits might be supported as well but not tested yet.
 - Solution created using Visual Studio 2017.
 - Uses some more recent C++ features like binary literals (C++14) and delete(void* ptr, size_t size) overload (C++17) which should be included with the latest VC++ that comes with VS2017.

### Current State
 - All features in the list mentioned bellow are implemented already.
 - Navigation Camera (WASD to move, Arrow Keys to rotate, Space to rise, LShift to descend).
 - It now renders low-poly cow model exported from Maya:
![Screenshot](https://github.com/yanqiniu/CowByte/blob/master/assets/other/screenshot_3_29_2018.jpg)
 - Material and textures for different meshes.
 - Phong lighting and shading with normal map:
![Screenshot](https://github.com/yanqiniu/CowByte/blob/master/assets/other/screenshot_3_31_2018.PNG)


### Features
 - As mentioned above, this engine uses DirectX 11. Might support DX12 later;
 - Implements component model with message bus;
 - Custom memory allocator (sets of pools);
 - Custom data containers (Queue, Vector, reference count pointer);
 - SIMD math (Intel SSE) including Vec3, Matrix4x4 and Quaternion;
 - Scenegraph;
 - Mesh sharing;
 - Material and Texture support with texture sharing;
 - Dynamic per-pixel phong lighting and shading;
 - Normal map rendering;
 - Custom string and File that supports commenting;
 - Keyboard and mouse input;
 - Exporting mesh from Maya;
 - ...

GpuLib
------

<img width="800px" src="https://i.imgur.com/dQEm83w.gif" />
<img width="800px" src="https://i.imgur.com/oDLY5rY.png" />

GpuLib is a public domain C library that uses 65 modern OpenGL AZDO functions to draw 3D geometry, post-process textures and compute arrays of numbers on GPU.

The contract:

 * SDL2, Linux and Windows (Visual Studio 2015) only. Doesn't target macOS, WebGL or OpenGL ES.
 * No multithreaded or asynchronous CPU <-> GPU interactions. No barriers or sync points except for glFinish calls.
 * Not all modern OpenGL extensions are used, only those which are supported on low-end hardware and latest Mesa.

Naming convention:

 * `buf`: Buffer
 * `tex`: Texture
 * `pix`: Pixel
 * `img`: Image
 * `cbm`: Cubemap
 * `msi`: Multisample Image
 * `cpi`: Compressed Image
 * `smp`: Sampler
 * `pro`: Program Object
 * `ppo`: Pipeline Program Object
 * `fbo`: Framebuffer Object
 * `xfb`: Transform Feedback Object

Special thanks to Nicolas [@nlguillemot](https://github.com/nlguillemot) and Andreas [@ands](https://github.com/ands) for answering my OpenGL questions and Micha [@vurtun](https://github.com/vurtun) for suggestions on how to improve the library!
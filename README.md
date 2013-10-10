FractalSphere
=============
A very simple and **highly experimental** gadget application to play around with qt's 3D functionality.

The basic idea is simply to create spheres which can be changed in size and position. 
Each sphere has as surface a special texture consisting of a dynamically changing fractal, see figure:

![sample1](http://i.imgur.com/cJDEAWY.png?1)

It needs `qt5` and `qt3d5`.
There is a first approach to create a cube-map to make the central sphere act like a mirror. Therefore render-to-texture is implemented but not debugged properly yet!

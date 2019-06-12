# Raytracing
## How to run
---
```
cd partB
make
./raytracer [width] [height] [options]
```
options are as follows:
[(leave blank): full effect](#final)
0: secondary reflection
[1: hardshadow](#hardshadow)
[2: environment mapping](#cubemap)
[3: anti-aliasing](#anti-aliasing)
[4: softshadow](#softshadow)
[5: glossy reflection](#glossy-reflection)
[6: depth of field](#depth-of-field)
## Result
### Final
![full1](raytracer/partB/full1.bmp)
![full2](raytracer/partB/full2.bmp)
### hardshadow
calculate rays emits from the surface back to source and darker those that have crossed over objects
![hardshadow1](raytracer/partB/hardshadow1.bmp)
![hardshadow2](raytracer/partB/hardshadow2.bmp)
### cubemap
convert texture xyz coordinates to scene uv coordinates
![environmentmap1](raytracer/partB/environmentmap1.bmp)
![environmentmap2](raytracer/partB/environmentmap2.bmp)
### anti-aliasing
16x supersampling
![antialiasing1](raytracer/partB/antialiasing1.bmp)
![antialiasing2](raytracer/partB/antialiasing2.bmp)
### softshadow
add multiple different light sources to create multipule shadows
![softshadow1](raytracer/partB/softshadow1.bmp)
![softshadow2](raytracer/partB/softshadow2.bmp)
### glossy-reflection
while calculating reflection ray, create some random refelction to make it glossy
![glossy1](raytracer/partB/glossy1.bmp)
![glossy2](raytracer/partB/glossy2.bmp)
### depth-of-field
By casting multiple rays to every pixel, with randomize eye position, adjustable focal distance and aperture. Set the focal distance close to the jade plane, everything else is a bit out of focus, depends on the distance.
![DOF1](raytracer/partB/DOF1.bmp)
![DOF2](raytracer/partB/DOF2.bmp)

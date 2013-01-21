DS Models Example
==================

An example of showing 3D models on the Nintendo DS.

This code uses OpenGL code on the Nintendo DS hardware to display some 3D models with lightning
and different camera angles. The models are hard-coded inside .h files and the PatchObj program
is included if you need to create new ones.

## Compiling

To compile the code you'll need DevKitARM from [DevKitPro](http://devkitpro.org/). Refer to [this page](http://devkitpro.org/wiki/Getting_Started) for instructions on how to install.

After the deed is done a simple `make` inside the project folder will generate an *.nds file.

## Usage

The file can be ran on DS hardware by means of a flash cart or alternatively you can use Nintendo DS emulator like [Desmume](http://desmume.org/).

Press left and right to rotate the camera along the vertical axis, up and down to rotate it along the horizontal axis and `X` to rotate it along the Z axis.

Use the shoulder `L` and `R` buttons to switch the model being shown.

`A` toggles the wireframe view.

Press `Start` to reset the parameters.
# Visualizing Strange Attractors

This project contains code to generate geometry for strange attractors.

![Burke-Shaw Attractor](https://i.imgur.com/q40xPBA.png)

## Background

Strange attractors are a mathematical formulation to generate points in space
by evaluating an attractor function. The case considered here generates a one
dimensional curve in 3D space by evaluating an attractor time series function.

Attractors are usually represented using differential equations, but for the
sake of simplicity, and because accuracy is not that important for
visualization, the functions can be represented as a time series.

More information about strange attractors can be found here:

  * http://www.3d-meier.de/tut5/Seite0.html

## Visualization

These curves can be visualized by looking at the set of generated points.
More interesting images can be produced by generating some geometry, e.g.,
a tube like structure that surrounds the curve.

This code provides functionality to generate this geometry.

  * Generation of a curve sampling by evaluating attractor functions
  * Simplification of these curves to avoid over-tessellation
  * Assigning a consistent local coordinate system to the samples
  * Producing a triangulation around the curve

## Building

The code depends on the MVE library and a few other small libraries. First,
install MVE's dependencies. This command depends on your distribution.

    $ sudo aptitiude install libpng-dev, libjpeg-dev, libtiff-dev

 Clone the MVE repository and the attractors repository:

    $ git clone https://github.com/simonfuhrmann/mve.git
    $ git clone https://github.com/simonfuhrmann/attractors.git

 Build MVE and attractors:

    $ make -Cmve -j8
    $ make -Cattractors/src -j8

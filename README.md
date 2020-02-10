#MandelbulbUI_V2

This is a complete rewrite of my MandelbulbUI program. The UI is made with Qt 5.

This application is supposed to take certain inputs and then calculate a the 3d Mandelbulb fractal.
To be precise, the output consists of a set of coordinates which are in the Mandelbulb set. All points
lie on a 3d eqidistant grid. It is possible to filter out everything but the 'hull'. A point is on the hull
if and only if at least one of its 26 neighbouring points is not in the set.

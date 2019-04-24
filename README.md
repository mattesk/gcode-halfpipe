

CREATE G-CODE TO MILL A HALFPIPE WITH A BALL ENDMILL
====================================================

Using FreeCAD now for a ~year or so and it happen to be quite an interesting learning curve for me.
Lately I started using the Path workbench module to feed my 'homebrew' cnc machine. 

For one of my projects I needed to mill a round channel / halfpipe with a D=32mm, which is tradionally done with a ball endmill.
Hard to find and pricy for 32mm. I did not feel like spending $$$. So why not using a smaller ball endmill with the cnc machine.
 
Finally managed to use FreeCAD Path workbench by using the 3d pocket to create some g-code using a square endmill
Milling the newly created path of the round channel, I quickly came to realize that the finish is not that great using square endmill. 
(unless you run step down rate of thousands). Not very ideal.

I was convinced a ball endmill could do the job faster and with a better finish.

Tried to use a ball end mill instead, but was not successful to configure it properly in FreeCAD 0.18. 
The dialog offers ball endmill, but it seems not to work for me (and be used still as a square end mill). 

Not giving up I finally came up with the idea to create my own gcode. The math is really not that bad. 
Using a round endmill it is pretty much up to defining the cut radius, calc the coordinates for the position and then running the endmill back and forth. 
So I created a small c program to calculate the needed coordinates.



    $ halfpipe -b 12.5 -l 30 -x 0 -y 80 -z 4
    (--- halfpipe D=32.00 L=30.00 zcut=4.00 center x/y/z= 0.00 80.00 12.00 hpR=16.00 bemR=6.25 cutR=9.75 )
    G0 Z10.000
    G0 X0.000 Y86.894
    G0 Z5.000
    (halfpipe path 45 degrees: ysin/zcos=0.707/0.707 hpcoff_yz= 6.89 6.89 bemx/y/z= 0.00 86.89 -1.14)
    G1 X0.000 Y86.894 Z-1.144 F50.000
    G1 X30.000 Y86.894 Z-1.144 F200.000
    G1 X30.000 Y73.106 Z-1.144 F200.000
    G1 X0.000 Y73.106 Z-1.144 F200.000
    G1 X0.000 Y86.894 Z-1.144 F200.000
    (halfpipe path 30 degrees: ysin/zcos=0.500/0.866 hpcoff_yz= 4.87 8.44 bemx/y/z= 0.00 84.88 -2.69)
    G1 X0.000 Y84.875 Z-2.694 F50.000
    G1 X30.000 Y84.875 Z-2.694 F200.000
    G1 X30.000 Y75.125 Z-2.694 F200.000
    G1 X0.000 Y75.125 Z-2.694 F200.000
    G1 X0.000 Y84.875 Z-2.694 F200.000
    (halfpipe path 15 degrees: ysin/zcos=0.259/0.966 hpcoff_yz= 2.52 9.42 bemx/y/z= 0.00 82.52 -3.67)
    G1 X0.000 Y82.523 Z-3.668 F50.000
    G1 X30.000 Y82.523 Z-3.668 F200.000
    G1 X30.000 Y77.477 Z-3.668 F200.000
    G1 X0.000 Y77.477 Z-3.668 F200.000
    G1 X0.000 Y82.523 Z-3.668 F200.000
    (halfpipe path 00 degrees: ysin/zcos=0.000/1.000 hpcoff_yz= 0.00 9.75 bemx/y/z= 0.00 80.00 -4.00)
    G1 X0.000 Y80.000 Z-4.000 F50.000
    G1 X30.000 Y80.000 Z-4.000 F200.000
    G1 X30.000 Y80.000 Z-4.000 F200.000
    G1 X0.000 Y80.000 Z-4.000 F200.000
    G1 X0.000 Y80.000 Z-4.000 F200.000
    G0 Z10.000 

These snippets I then cut&paste into gcode file create by FreeCAD using a custom path operation (see sample g-code)
For a better finish I used 5 degrees steps, though 15 degrees already beat the the square end mill approach by miles.

For me it saved me spending $$$ on a special sized (probably custom) ball end mill.

# gauge-vision
Digitalising pointer gauge readings using machine vision.
The project is only starting and library is for research purposes only right now.

# Examples of outputs
![Gauge](/images/dial3.jpeg?raw=true "Gauge")
![Gauge with marked pointer](/Examples/pointer.jpg?raw=true "Pointer")
![Gauge value bar](/Examples/textwin.jpg?raw=true "Digitalized value")

![Gauge](/images/dial4.jpeg?raw=true "Gauge")
![Gauge with marked pointer](/Examples/pointer2.jpg?raw=true "Pointer")
![Gauge value bar](/Examples/textwin2.jpg?raw=true "Digitalized value")

# requirements
Requires OpenCV (tested for 3.0, should work with other versions)

# building
Simply use the provided CMakeLists. No arguments are necessary.

# running
Run the DialsRead executable in your binary folder.
You should go through some frames of the same image (currently) and have some pictures showing steps in digitizing the image in the end. There should be a bar showing the digitized value of the pointer gauge.

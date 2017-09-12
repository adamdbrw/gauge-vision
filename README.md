# gauge-vision
Digitalising pointer gauge readings using machine vision.
The project is only starting and library is for research purposes only right now.

# Examples of outputs
<img src="images/dial3.jpeg?" height="200" width="200"><img src="Examples/binary.jpg?" height="200" width="200"><img src="Examples/pointer.jpg?" height="200" width="200"><img src="Examples/textwin.jpg?" height="200" width="200">

<!--
/<img src="images/dial4.jpeg?" height="200" width="200"><img src="Examples/binary2.jpg?" height="200" width="200"><img src="Examples/pointer2.jpg?" height="200" width="200"><img src="Examples/textwin2.jpg?" height="200" width="200">
-->
# requirements
Requires OpenCV (tested for 3.0, should work with other versions)

# building
Simply use the provided CMakeLists. No arguments are necessary.

# running
Run the DialsRead executable in your binary folder.
You should go through some frames of the same image (currently) and have some pictures showing steps in digitizing the image in the end. There should be a bar showing the digitized value of the pointer gauge.

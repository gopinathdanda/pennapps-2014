<h1>roVR</h1>

A virtual presence Raspberry Pi-bot controlled via Myo armband and Oculus Rift.

<h3>Myo-client side interfacing</h3>

Basic code to send command from Myo armband to the server hosted by Pi.

<h3>Oculus interfacing</h3>

Basic code to render video feed from a webpage onto Oculus screen using three.js and VRRenderer.js.

<h3>Webcam hosting by Pi (No. 1)</h3>

Motion package was used to stream the video from the webcams attached to Raspberry Pi 1 to its own IP address. The video was accessed and streamed onto the Oculus headset directly.

<h3>Python server on Pi (No. 2) controlling motors</h3>

Basic Python server accepting client connection from Myo armband followed by instructions and actuating the motors of the PiBot accordingly.

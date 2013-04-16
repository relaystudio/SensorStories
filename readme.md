# Sensor Stories
A fast way to prototype ideas for sensor-driven interfaces via firmata and video. It's an in development tool that I'm using to create sensor triggered and state defined wireframes for large screen or projection-based interactive spaces. Presenting sketches in such a way tells a compelling and flexible story around how we interact with our things and spaces.
![Sketch](https://raw.github.com/relaystudio/SensorStories/master/sketch.jpg)
### Instructions
Create folders in the `bin/data` directory called mov1, mov2, etc. Place movies called `begin.mov`, `loop.mov`, and `end.mov` in the aforementioned folders.

These files correspond to the introduction animation, the looping animation, and the outro animation for that particular arc of the experience.

Sensors are driven by firmata, and individual sensor events are extended as ofEvents. Individual sensor events will need to be tailored towards the particular configuration. I've included RFID, photosensor, and push button sensors and accompanying schematics.
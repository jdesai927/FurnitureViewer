To run the program, enter as the first argument the name of the scene file and the as the second argument the name of the raytrace configuration file.
I have completed all parts of the assignment, including some of the extra credit.
I've implemented bounding boxes, multiple lights (see my raytracer_config_sample.txt for more information on the new configuration file syntax. There must be exactly 3 lights, with properties having the names listed in the same way as in the aforementioned file),
basic transparency (set the refractive index to 1.0) and physically correct refraction on transparent objects.
See the "Best Images" folder for some of my favorite BMP renderings!
Known bugs:
On transparent objects, there is some noise around the base. This is because the floor and the objects are adjacent, which I couldn't really think of a way to fix without moving the floor down, so I left it as it was.
Giving a refractive index greater than 1.1 for any object causes weird problems. However, for refractive indices 1.1 and below, refraction works correctly.
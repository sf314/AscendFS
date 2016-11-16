Hey, its Stephen!

So we've worked on ascendMaster2.ino, which has all of
the sensor integrations, but it doesn't write to the 
SD card. 

Well testWriteSD.ino *does* write to the SD card, and 
it seems to not have trouble with the AscendLib
library, so it's probably best to integrate the sensors 
into testWriteSD.ino to get everything to work. 

Currently integrated sensors (as of 11/15, 9:30pm) is 
a single temp probe. The rest of the stuff can be 
integrated in a similar fashion.


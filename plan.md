A step back:

Since my printer caught fire during my first attempt at getting a physical gcode running (Note: this is a great experience to talk about when judges want to hear about challenges during the ideation phase), I thought it would be a good idea to step back and look at where I want to go next.

PLAN A: (If the second rpi doesn't come in from Hack Club)

Firstly, it would be smart to remove whatever OS is currently loaded on the boards internal EEPROM and Memory (Which seems to be a version of Marlin, but I cannot be entirely sure), and replace it with a custom Klipper version, complete with proper cold extrusion support and allowing for the correct way to reverse the extruder motor, instead of the janky way that I tried to do it earlier, which resulted in the aforementioned fire. I am still not quite sure if I would like to use my rpi5 for this, as it is currently functioning as a server for a jellyfin and vikunja instance, alongside a cloudflare tunnel, but then again, I am looking to move my vikunja instance into a AWS container and move my jellyfin instance onto my Synology NAS through docker, which would free up the pi for use as a Klipper host.

Todo list summary from the previous paragraph: (note: fill the md brackets with an x when the step is complete to check it off)

- [ ] Move the Vikunja instance to AWS and reconnect it to the cloudflare instance
- [ ] Move the Jellyfin instance into a docker container on the Synology DS212 2TB
- [ ] Install klipper on the ender 3 pro / bioprinter and the rpi5

PLAN B: (Presuming that the second rpi from Hack Club does come in on time)

Firstly, I would still want to perform the first two above tasks (Move the Vikunja instance to AWS and reconnect it to the cloudflare instance and move the Jellyfin instance into a docker container on the Synology DS212 2TB), but they are, due to the qualities inherent in their nature, lower priority than the other, more pressing tasks outlined in the rest of this document, and will most likely be done after all ECs are completed, possible internships are completed, and any other pending work has finished, since they are currently working with no need to change that. I would then install a build of Mainsail OS on the Raspberry Pi Zero 2 W acquired from Hack Club due to hours from this project, flash the klipper file from that OS onto the modded ender 3 pro turned bioprinter, and use the Raspberry Pi Zero 2 W as the permanent host for the ender, so that the Raspberry Pi 5 8GB can remain as my experiment board for selfhosting and if any mistakes are made that would lead to damage to the printer's main board (as seen in the earlier fire), the more expensive rpi5 would not be damaged.

Presuming that one of the sets of above changes were made, the next logical step would be to build a custom klipper .cfg file for my new printer, which I would like to start on as soon as possible due to the complexity of its nature. It would be logical to begin with a stock ender 3 pro profile, and make the following changes, as that allows for the certainty that a step has not been missed.

Modifications to make to the klipper profile from a stock ender 3 pro profile:

- [ ] Cold extrusion
- [ ] Change the Z height to account for the movement of the end stop to allocate for the increased size of the new toolhead
- [x] 0.8 mm nozzle size (the actual nozzle is 0.83mm in diameter but this is easier for slicers to work with and the difference can be tuned out through fow rate, line thickness, etc.) [CHANGED LINE 64]
- [x] Reverse the extruder motor [CHANGED LINE 60]

A step back:

Since my printer caught fire during my first attempt at getting a physical gcode running (Note: this is a great experience to talk about when judges want to hear about challenges during the ideation phase), I thought it would be a good idea to step back and look at where I want to go next.

PLAN A: (If the second rpi doesn't come in from Hack Club)

{

Firstly, it would be smart to remove whatever OS is currently loaded on the boards internal EEPROM and Memory (Which seems to be a version of Marlin, but I cannot be entirely sure), and replace it with a custom Klipper version, complete with proper cold extrusion support and allowing for the correct way to reverse the extruder motor, instead of the janky way that I tried to do it earlier, which resulted in the aforementioned fire. I am still not quite sure if I would like to use my rpi5 for this, as it is currently functioning as a server for a jellyfin and vikunja instance, alongside a cloudflare tunnel, but then again, I am looking to move my vikunja instance into a AWS container and move my jellyfin instance onto my Synology NAS through docker, which would free up the pi for use as a Klipper host.

Todo list summary from the previous paragraph: (note: fill the md brackets with an x when the step is complete to check it off)

- [ ] Move the Vikunja instance to AWS and reconnect it to the cloudflare instance
- [ ] Move the Jellyfin instance into a docker container on the Synology DS212 2TB
- [ ] Install klipper on the ender 3 pro / bioprinter and the rpi5

}

PLAN B: (Presuming that the second rpi from Hack Club does come in on time)


{

Firstly, I would still want to perform the first two above tasks (Move the Vikunja instance to AWS and reconnect it to the cloudflare instance and move the Jellyfin instance into a docker container on the Synology DS212 2TB), but they are, due to the qualities inherent in their nature, lower priority than the other, more pressing tasks outlined in the rest of this document, and will most likely be done after all ECs are completed, possible internships are completed, and any other pending work has finished, since they are currently working with no need to change that. I would then install a build of Mainsail OS on the Raspberry Pi Zero 2 W acquired from Hack Club due to hours from this project, flash the klipper file from that OS onto the modded ender 3 pro turned bioprinter, and use the Raspberry Pi Zero 2 W as the permanent host for the ender, so that the Raspberry Pi 5 8GB can remain as my experiment board for selfhosting and if any mistakes are made that would lead to damage to the printer's main board (as seen in the earlier fire), the more expensive rpi5 would not be damaged.

}

Presuming that one of the sets of above changes were made, the next logical step would be to build a custom klipper .cfg file for my new printer, which I would like to start on as soon as possible due to the complexity of its nature. It would be logical to begin with a stock ender 3 pro profile, and make the following changes, as that allows for the certainty that a step has not been missed.

Modifications to make to the klipper profile from a stock ender 3 pro profile:

- [ ] Cold extrusion
- [ ] Change the Z height to account for the movement of the end stop to allocate for the increased size of the new toolhead
- [x] 0.8 mm nozzle size (the actual nozzle is 0.83mm in diameter but this is easier for slicers to work with and the difference can be tuned out through flow rate, line thickness, etc.) [CHANGED LINE 64]
- [x] Reverse the extruder motor [CHANGED LINE 60]

Once MainsailOS/Klipper has been installed on the rpi of choice, the plan to prevent another fire or severe accedent would be to put the compiled klipper OS for the printer on the SD card. This way, when the printer is turned on for the first time after the accedent, the klipper code will immideatley replace the currently running marlin instance, preventing the execution of any malicious code that caused the fire.

Once klipper is running, then the time for testing with a gelatin and water mix becomes viable. Before the fire, we saw that the printer sucsessfully ran customized files sliced in cura, that, alongside a higher z endstop preventing the nozzle crashing into the bed, would allowed for testing with a gelatiin and water mix to be able to happen immediatley, if the small issue of the extruder motor moving in the wrong direction was fixed. The reason the motor was moving in the wrong direction was the fact that, during the instaltion of the m8 bolt that drives the main extrusion rod, the bolt was placed upside down, due to a lack of instructions and thought into the direction of the rod. Due to this, the pole moved upwards during extrusion, a simple fix in klipper, but not so simple in the version of marlin running on the ender 3 pro turned bioprinter at the time. Now that I know that the expirementing with marlin settings instead of just biting the bullet and installing the clearley superior firmware was the problem that lead to the fire, I have gone ahead and written this file to prevent any more lapses in judgemnt from happening. Every major step from hear on out will be typed into this document, allowing for better documetnation and the prevention of logical errors that could have been prevented with more time spent contemplating the problem. Now that the direction of extrusion has been repaired in the .cfg file, it is time to proceed to the next step: tuning the printer for the printing of a gelatin and water solution.

In theory, the printing of a gelatin and water solution is simple. Firstly, heat up the solution of gelatin and water till it reaches about $x\degree c$, upon which it will transform into a gelatinous liquid that can be inseted into the extruder by placing it below the orifice and revertsing the extruder, effectivley sucking up the liquid. While it is warm, it can be extruded out like any other fillament, and it will cool on the plate in ~30sec, allowing for the next layer to be printed on top. Any additional waste products or failed prints can be reheated up into liquid form and reprinted, allowing for testing without wastage. The purpouse of this step is to allow for tuning within a range of the actual bioink that will be used in the final product to prevent the waste of the final product for tuning putposes, when you could use a $2.99 gelatin mix to tune instead.
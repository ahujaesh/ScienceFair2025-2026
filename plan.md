A step back:

Since my printer caught fire during my first attempt at getting a physical gcode running (Note: this is a great experience to talk about when judges want to hear about challenges during the ideation phase), I thought it would be a good idea to step back and look at where I want to go next.

PLAN A: (If the second Raspberry Pi doesn't come in from Hack Club)

{

Firstly, it would be smart to remove whatever OS is currently loaded on the board's internal EEPROM and Memory (Which seems to be a version of Marlin, but which version I cannot be entirely sure of) and replace it with a custom Klipper version, complete with proper cold extrusion support and allowing for the correct way to reverse the extruder motor, instead of the janky way that I tried to do it earlier, which resulted in the aforementioned fire. I still don't know if I would like to use my Raspberry Pi 5 for this, as it is currently functioning as a server for a Jellyfin and Vikunja instance alongside a Cloudflare tunnel. Still, I am looking to move my Vikunja instance into an AWS container and migrate my Jellyfin instance to my Synology NAS using Docker, which would free up the Raspberry Pi 5 for use as a Klipper host.

Todo list summary from the previous paragraph: (note: fill the md brackets with an x when the step is complete to check it off)

- [ ] Move the Vikunja instance to AWS and reconnect it to the Cloudflare instance
- [ ] Move the Jellyfin instance into a docker container on the Synology DS212 2TB
- [ ] Install Klipper on the Ender 3 Pro / bioprinter and the rpi5

}

PLAN B: (Presuming that the Raspberry Pi Zero 2 W from Hack Club does come in on time)


{

Firstly, I still want to perform the first two tasks above (Move the Vikunja instance to AWS, reconnect it to the Cloudflare instance, and move the Jellyfin instance into a Docker container on the Synology DS212 2TB). Still, these improvements are, due to the qualities inherent in their nature, lower priority than the other, more pressing tasks outlined in the rest of this document. I will complete the tasks after I have finished all ECs, any possible internships, and any other pending work. The services are currently functioning fine, though insecurely, with no urgent need to change that. I would then install a build of Mainsail OS on the Raspberry Pi Zero 2 W acquired from Hack Club due to hours from this project, flash the Klipper file from that OS onto the modded Ender 3 Pro turned bioprinter, and use the Raspberry Pi Zero 2 W as the permanent host for the Ender so that the Raspberry Pi 5 8GB can remain as my experiment board for selfhosting. As the experiment progresses, there is a high chance of another accident occurring that would lead to damage to the printer's main board (as seen in the earlier fire). Since I chose the cheaper board, the more expensive Raspberry Pi 5 will not be damaged, and I could insert it later if needed.

}

Assuming that I made one of the above changes, the next logical step would be to create a custom Klipper .cfg file for my new printer, which I would like to start on as soon as possible due to its complexity. It would be logical to begin with a stock Ender 3 Pro profile and make the following changes, as this ensures that I have not skipped any steps.

Modifications to make to the Klipper profile from a stock Ender 3 Pro profile:

- [x] Cold extrusion
- [x] Change the Z height to account for the movement of the end stop to allocate for the increased size of the new toolhead
- [x] 0.8 mm nozzle size (the actual nozzle is 0.83mm in diameter, but the value of 0.8mm is easier for slicers to work with. I can tune out the difference through flow rate, line thickness, etc.) [CHANGED LINE 65]
- [x] Reverse the extruder motor [CHANGED LINE 61]
- [ ] Change every movement (X, Y, and Z, not E) speed to max 5mm/s 

Once I install MainsailOS/Klipper on the Raspberry Pi of choice (see the .txt files in the "Klipper" directory), the plan to prevent another fire or severe accident would be to put the compiled Klipper OS for the printer on the SD card. This way, when I turn on the printer for the first time after the accident, the Klipper code will immediately replace the currently running marlin instance, preventing the execution of my accidentally malicious code that caused the fire because I will overwrite it before it has a chance to execute.

Once Klipper is running, the next step is testing with a gelatin and water mix. Before the fire, we saw that the printer successfully ran customized files sliced in Cura, which, alongside a higher z endstop preventing the nozzle from crashing into the bed, would allow for testing with a gelatin and water mix to be able to happen immediately, if I fixed the issue of the extruder motor moving in the wrong direction. The reason the motor was moving in the wrong direction was that, during the installation of the M8 bolt that drives the central extrusion rod, I placed the bolt upside down in the slot due to a lack of consideration of the rod's movement direction. Due to this, the pole moved upwards during extrusion, a simple fix in Klipper but not so simple in the version of Marlin running on the bioprinter at the time. Now that I know that experimenting with Marlin settings instead of just biting the bullet and installing Klipper, a firmware that allows for much more customization, was the problem that led to the fire, I have gone ahead and written this file to prevent any more lapses in judgment from happening. Every significant step from here on out will be written in this document, allowing for better documentation and the prevention of logical errors that I could have avoided with more time spent contemplating the problem. Now that I have corrected the extrusion direction in the .cfg file, it is time to proceed to the next step: tuning the printer for printing a gelatin and water solution.

In theory, the printing of a gelatin and water solution is simple. Firstly, heat the solution of gelatin(~10g) and water (~50mL) till it reaches about $45\degree c$ (ideal extrusion temps are 35 to 45 $\degree c$), upon which it will transform into a gelatinous liquid that I can insert into the extruder by placing it below the orifice and reversing the extruder, effectively sucking up the liquid. While it is warm, I can extrude it like any other filament, and it will cool on the plate in ~30 seconds, allowing for the next layer to print on top. To prevent the cooling of the gelatin mix inside the syringe, I will add a solution later, but first, I would like to experiment with seeing how long the gelatin takes to cool; then, I can DIY a solution (such as an Arduino with a relay that heats a wire wrapped around the syringe, powered by a 12v battery, or a hairdryer mount that blows warm air onto the syringe without disturbing the print, possibly through a funnel or other system to direct the air current) and write it here. If it takes long enough to cool, it might be smarter to run small prints instead.  

Any additional waste products or failed prints can be reheated into liquid form and reprinted, allowing for testing without wastage. The purpose of this step is to allow for tuning within a range of the actual bioink that will be used in the final product to prevent the waste of the final product for tuning purposes when you could use a $2.99 gelatin mix to tune instead. (Note: keep the temp below 60°C (140°F) to avoid breaking it down.)
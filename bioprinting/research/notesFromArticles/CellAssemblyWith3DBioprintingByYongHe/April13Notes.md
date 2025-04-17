# April 13th's Notes on the article "Cell Assembly with 3D Bioprinting" by Yong He, Quing Gao, and Yifei Jin


Eshaan Ahuja

April 13, 2025

Science Fair 2025-2026

|Outline of current Idea|
|-|
|The current thought process (prior to any reading of literature on the subject), is to build a modified 2D plotter running on an Arduino Mega and Motor Control Board to allow for the attachment of an extrusion device to eject living simple cells suspended in bioink to form tissues. The Arduino Mega's main movement system is currently being thought of as joystick controlled, with manual movements creating the two dimensional figures in separate petri dishes, and the stacking of layers afterwards allows for the creation of the finished tissue. The goal would be to also modify it to run on a traditional 3D printer software, and allow for the processing of G-Code, allowing for more complex shapes than a manual machine could perform. As of this moment, the Arduino Mega has been configured to run off of a joystick movement system, which would power the rudimentary 2D plotter currently printing in PETG on my Bambu P1S Combo, an 11 hour overnight print.|

|Goal for this note taking session|
|-|
|Identify the feasibility of the creation of these cells and bioinks at the current level of skill, and understand conceptual mechanics of the subject, filling in misconceptions about how the attachment and creation of layers would work, and if this project could work. Alongside this, another goal would be to identify the type of cell that would most accurately fulfill my goal of forming tissues while still conforming to ISEF rules on cells and my current abilities. The article that will be read today, "Cell Assembly with 3D Bioprinting" by Yong He, Quing Gao, and Yifei Jin, is an article establishing various methods of utilizing this cutting edge technology, and should allow me to meet my goals for research tonight. While it was written in 2021, four years before this project started, and quite behind given that this concept of bioprinting has been around for such a short time, it should me modern enough for my purposes.|

## Notes Begin:

- Bioprinting was first demonstrated in 1988 when a standard HP inkjet printer was modified to be able to deposit cells.

- 3D bioprinting has three main categories:

|Extrusion Based|Droplet Based|Projection Based|
|-|-|-|
|Extrusion based generates structures similar to how a standard FDM printer works, with continuous fibers. This is the most commonly used method|Droplet Based works similar to the blob printing style of FDM ([Eg.](https://fullcontrol.xyz/#/models/800020))|Projection Based works similar to standard resin 3D printers, utilizing the photosensitive properties of materials to form layers|

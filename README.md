# crunchlabs-iturret

source from crunchlabs' iturret project



## folders (sketches) in this repo:



### iturret-anti-magazine-spike

mikey hack that reduces the likelihood of the iturret spiking/throwing/RUD'ing the magazine when powered on.

this hack makes several changes:

* reduced maximum pitch to 140° (from 175°). this helps to reduce the momentum change, at max tilt. it also ensures that the wire connector and the ir receiver spring do not contact the microcontroller or the reset button, at full tilt
* increased minimum pitch to 60° (from 10°). this reduces strain on the yaw motor wires.
* changed initialPitchServo to 90° from 100°, which seems to smooth the homing, and reduce the likelihood of RUD



#### Discussion

if the iturret is pointed upward, or downward, at enough of an angle, when it is turned on, the homing motion can be so fast that the magnets on the magazine may not be sufficient to keep the magazine attached. the turret can "spike" (in the football sense of spiking a ball) the magazine.

the servos used in this project have several characteristics that make this process more challenging:

* we cannot control their speed
* we cannot command them to make incremental moves (e.g. 1°)
* we cannot read their position
* regardless of the starting position, the motor will first move toward or to 90 (perhaps to orient itself internally), so we cannot even command the turret to tip all the way upward, reliably. if the turret is already at a tilt > 90°, it will move to 90 and then back, making the problem worse.
* all we can do is command them to move to a particular position

the project manages servo speed indirectly. it knows the angle the servo was commanded to move to, so all following moves can be made to angles relative to that last angle. delays are added to ensure that each step is paused.



------



### iturret-base

the original ("level 1") source for the iturret



------



### iturret - password

source for the password-restricted hack



------



### iturret-roulette

source for the roulette hack


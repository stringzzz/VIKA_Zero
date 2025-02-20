# VIKA_Zero


VIKA_Zero: Vanishing Illusion Key Automator Zero

An ongoing encryption system project.


I chose the name VIKA many years ago when I coded my first few encryption systems, and wow, they were
absolutely horrible. As such, I'm calling this one VIKA Zero to show that the final version will be far
superior to those old ones, which is why it's 'Zero' instead of some higher number (The inspiration for this 
comes from Google's Go playing bots, AlphaGo and AlphaZero).


02-16-2025: The revised informal paper for Candidate A is up along with the revised test code. The mistake with the key schedule
wasn't as bad as I originally thought, but it definitely warranted redoing all the affected tests to be sure the data
was valid.


02-20-2025: Updated the "diffusion slide" functions so it now covers the last byte in the block.
Decided to redo the tests, and found a significant difference in the plaintext avalanche effect.
So, added the new test code, added results to newly revised paper as well. Note that the main system
itself now has the updated diffusion slide functions as well.


Next step is learning more of what I need to move forward with this project, see the informal paper's conclusion for details.


That's all for now,
--stringzzz, Ghostwarez Co.

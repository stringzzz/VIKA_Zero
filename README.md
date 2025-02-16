# VIKA_Zero


VIKA_Zero: Vanishing Illusion Key Automator Zero


I chose the name VIKA many years ago when I coded my first few encryption systems, and wow, they were
absolutely horrible. As such, I'm calling this one VIKA Zero to show that the final version will be far
superior to those old ones, which is why it's 'Zero' instead of some higher number (The inspiration for this 
comes from Google's Go playing bots, AlphaGo and AlphaZero).


I'm putting this project up on here as an extra backup for now, though it is definitely far from complete.
While the "Candidate A" version I'm putting on here does all work, Candidate A is only the tip of the iceberg.


I finished at community college to earn my Associate's Degree in Computer Science, though there is still
so much for me to learn on my path of becoming a professional cryptologist. The three important subjects that come to
mind are Abstract Algebra, Number Theory, and Statistics. This Candidate A system will serve as a base for me
to work off of, and so I will create more candidates as I learn more of what I need. Once I've learned everything
needed and can deeply analyze this system myself, I can finalize this project by choosing one of the candidate systems
to become VIKA_Zero.


The informal paper is now up along with all of the code that was used to do the tests on it. There was also a mistake found only
after writing the paper, but I left the test code as is since the mistake didn't seem to invalidate the tests. The problem
was that the key schedule generated was much larger than it needed to be, where the extra bytes were just not used at all. As 
for the main code found outside of the "test_code" directory, that part is fixed with the new, proper key schedule.


(Message 02-15-2025):

In regards to that mistake, I tracked down the problem from where it was first introduced, and refactored the test code.
The mistake didn't seem to be such a major problem, the genBox function was still using the correct amount of key schedule
bits to produce the boxes, however the key_schedule function was producing too many, and they were distributed to each part
by incorrect sizes, so it wasn't really using the bytes it should have been. This is all fixed in the test code, and I decided
to run the affected tests again. As for the informal paper, I will leave in the previous results, but make a note that there
was a mistake, and after which add new sections for the fixed test code and results in the paper.


Next step is learning more of what I need to move forward with this project, see the informal paper's conclusion for details.


That's all for now,
--stringzzz, Ghostwarez Co.

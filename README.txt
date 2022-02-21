CS101 PROJECT

BUBBLE TROUBLE

The following features has successfully been implemented:
    1. The bubbles now move in a parabolic path and as it hits the ground it gets reflected,
    2. A collision handler between the bubble and the bullet has been implemented. The bullet now destroys the bubble on contact.
    3. A collision detection system between the bubble and shooter has also been implemented. On collision, the shooter loses 1HP.
       Between two collisions, there is a small delay, so that the player get some time to recover.
    4. Bubbles of different sizes spawn in different levels. These larger bubbles into two, on collision until a certain size.
    5. HitPoints for the shooter has been implemented. The shooter has a total of 5HP.
       A clock counting backwards has also been implemented. The shooter gets a total of 45 seconds to destroy all bubbles.
       As a sucessfull collision between bullet and bubble is detected,player score increases by 1. As player and bubble collision happens,
	player score decreases by 1. Also time decreases by 4secs.
    6. A total of five levels has been implemented. Each level get progressively harder.
    7. Added a rate of fire system, so the player cannot spam bullets.



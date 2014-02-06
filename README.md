CircularMovementAI
==================

Originally intended to be a game using a mixture of AI that learns from the players as well as learning through trial and error.

While the game remains incomplete, it shows off a lot of complex AI algorithm work as well as the ability to add in A* Pathfinding.
The A* algorithm and node generation were coded and included but never actually added to any of the agents within the game, so all
it should take is a few lines of code to make it work.

The premise of the game was that we were going to make a Tactics RPG, but with a circle as the boundary of movement rather than square
tiles. Any given agent in the game would be given a distance they could travel, as well as some action points (AP). Action points were
used up while moving and attacking, so a player had to conserve them to do some of the more powerful attacks. To allow a player to both
move and attack in a single turn, we also limited the amount of AP consumed at max movement distance to 80% of the base AP that the
player had. We also allowed a player to "bank" any remaining AP from a turn as long as the total AP the player had did not surpass
200% of their base AP.

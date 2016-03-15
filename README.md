# CS 2 Week 9 Assignment Winter 2016

[Enrico Borba](https://github.com/enricozb/) and [Claire Goeckner-Wald](https://github.com/cgoecknerwald/)

This is an implementation of the Monte-Carlo method (with some modifications) as applied to Othello AI.

## Attempts to Improve AI:

We chose the Monte-Carlo method on recommendation of a former student. The Monte-Carlo method, or Monte-Carlo tree search, does a random sampling of player moves (both from our side and the enemy side), and selects the move with the highest average score. However, we ran a some drawbacks running against ConstantTimePlayer. We were not winning very often, and not infrequently losing very badly. To improve, we decided to sample intelligently, rather than randomly. 

This encompasses several things: realizing that Players will select for corners when given the chance; selecting next to corners is a bad idea; and following a table of priority move picks as found in "Analysis of Monte Carlo Methods in Othello" by Ryan Archer. Thus, the sample was more intelligently selected, by a priority picking table. Furthermore, when we are calculating our collection of win percentages, we place further emphasis on corners and other hot-spots, current scores, and our number of potential moves. (We prefer that our AI has many options for moves and that the enemy AI has few move options.) We do so by calculating a "bonus" or "anti-bonus". So, when we are selecting our next move, we not only take into account win percentage, but also this secondary bonus number. Thus, the AI has improved. 

Our next struggle was with randomization. Since we are playing 10 games against each AI, if our player plays the same way every time, we could lose 0/10. If we implement some sort of randomization, we have a chance of avoiding this pitfall. We will also look into applying Upper Confidence Bounds to the AI.  

## Contributions

Enrico:
Wrote AI. Read about the Monte-Carlo method in "Analysis of Monte Carlo Techniques in Othello" by Ryan Archer.

Claire:
Composed Contributions.txt and AI.txt. Read about the Upper Confidence Bounds in "Analysis of Monte Carlo Techniques in Othello" by Ryan Archer.



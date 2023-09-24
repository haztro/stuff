# blokus-ai

Blokus AI using Monte Carlo Tree Search. 

***mcts.py*** contains a general mcts implementation that can be used for whatever game. ***blokus.py*** contains a (not very efficient) implementation
of the game Blokus and can be used to pit MCTS agents against each other. ***blokus_gui.py*** provides a way to add human players using pygame. 

Here's a sample game of two MCTS agents versing each other with the iteration count set to 10000: 

![blokus_game](duo_game.gif)

The strategy of using the big pieces first seems to emerge. 

### Dependencies
- pygame 
- colorama

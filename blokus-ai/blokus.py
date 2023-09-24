from mcts import * 
import copy
import random
import time
from colorama import Fore, init

# Pieces represented as traverses through grid referenced from 
# bottom right corner connection.

p0 = [[]]	# 1
p1 = [[[1, 0]], 
	[[0, -1]]]	# two
p2 = [[[1, 0], [1, 0]], 
		[[0, -1], [0, -1]]] # straight
p3 = [[[1, 0], [0, 1]], 
		[[1, 0], [0, -1]], 
		[[0, -1], [1, 0]], 
		[[0, -1], [-1, 0]], 
		[[1, 0], [-1, -1]]]	# right angle
p4 = [[[1, 0], [0, 1], [1, 0]],
			[[1, 0], [0, -1], [1, 0]],
			[[1, 0], [-1, -1], [-1, 0]],
			[[0, -1], [1, 1], [0, 1]],
			[[0, -1], [1, 0], [0, -1]],
			[[0, -1], [-1, 0], [0, -1]]]	# stair
p5 = [[[0, -1], [0, -1], [1, 0]],
			[[0, -1], [0, -1], [-1, 0]],
			[[1, 0], [1, 0], [0, 1]],
			[[1, 0], [1, 0], [0, -1]],
			[[1, 0], [0, 1], [0, 1]],
			[[1, 0], [0, -1], [0, -1]],
			[[0, -1], [1, 0], [1, 0]],
			[[0, -1], [-1, 0], [-1, 0]],
			[[1, 0], [-1, -1], [0, -1]],
			[[0, -1], [1, 1], [1, 0]]]	# L shape
p6 = [[[1, 0], [0, -1], [-1, 0]]]	# box
p7 = [[[1, 0], [0, -1], [1, 1]],
		[[1, 0], [0, 1], [1, -1]],
		[[0, -1], [1, 0], [-1, -1]], 
		[[0, -1], [-1, 0], [1, -1]],
		[[0, -1], [1, 0], [-2, 0]], 
		[[1, 0], [0, -1], [0, 2]]]	# T shape
p8 = [[[1, 0], [1, 0], [1, 0]],	
		[[0, -1], [0, -1], [0, -1]]]	# straight
p9 = [[[-1, -1], [1, 0], [1, 0], [-1, -1]],
			[[1, 1], [0, -1], [0, -1], [1, 1]]]	# Cross
p10 = [[[1, 0], [1, 0], [1, 0], [1, 0]],	
			[[0, -1], [0, -1], [0, -1], [0, -1]]]	# Straight
p11 = [[[1, 0], [1, 0], [0, 1], [0, 1]],
			[[1, 0], [1, 0], [0, -1], [0, -1]],
			[[0, -1], [0, -1], [1, 0], [1, 0]], 
			[[0, -1], [0, -1], [-1, 0], [-1, 0]],
			[[1, 0], [1, 0], [-2, -1], [0, -1]]]	# Right angle
p12 = [[[1, 0], [0, 1], [0, 1], [1, 0]],
		[[1, 0], [0, -1], [0, -1], [1, 0]],
		[[0, -1], [1, 0], [1, 0], [0, -1]],
		[[0, -1], [-1, 0], [-1, 0], [0, -1]],
		[[0, -1], [1, 1], [1, 0], [0, 1]],
		[[1, 0], [-1, -1], [0, -1], [-1, 0]]]		# S shape
p13 = [[[1, 0], [0, 1], [1, 0], [0, 1]],
			[[1, 0], [0, -1], [1, 0], [0, -1]],
			[[0, -1], [1, 0], [0, -1], [1, 0]],
			[[0, -1], [-1, 0], [0, -1], [-1, 0]],
			[[-1, -1], [1, 0], [1, 1], [0, 1]],
			[[0, -1], [1, 1], [0, 1], [1, 0]],
			[[1, 0], [-1, -1], [-1, 0], [0, -1]]]	# stair
p14 = [[[1, 0], [1, 0], [1, 0], [0, 1]],
		[[1, 0], [1, 0], [1, 0], [0, -1]],
		[[0, -1], [0, -1], [0, -1], [1, 0]],
		[[0, -1], [0, -1], [0, -1], [-1, 0]],
		[[1, 0], [0, 1], [0, 1], [0, 1]],
		[[1, 0], [0, -1], [0, -1], [0, -1]],
		[[0, -1], [1, 0], [1, 0], [1, 0]],
		[[0, -1], [-1, 0], [-1, 0], [-1, 0]],
		[[1, 0], [-1, -1], [0, -1], [0, -1]],
		[[0, -1], [1, 1], [1, 0], [1, 0]]]		# L shape
p15 = [[[1, 0], [0, 1], [0, 1], [-1, 0]],
		[[1, 0], [0, -1], [0, -1], [-1, 0]],
		[[0, -1], [1, 0], [1, 0], [0, 1]],
		[[0, -1], [1, 1], [1, 0], [0, -1]],
		[[1, 0], [-1, -1], [0, -1], [1, 0]]]		# U shape
p16 = [[[1, 0], [1, 0], [-1, 1], [0, 1]],
		[[1, 0], [1, 0], [-1, -1], [0, -1]],
		[[0, -1], [0, -1], [1, 1], [1, 0]],
		[[0, -1], [0, -1], [-1, 1], [-1, 0]],
		[[1, 0], [1, 1], [0, -1], [0, -1]],
		[[0, -1], [-1, -1], [1, 0], [1, 0]]]		# T shape
p17 = [[[1, 0], [1, 1], [-1, 0], [0, 1]],
		[[1, 0], [1, -1], [-1, 0], [0, -1]],
		[[0, -1], [1, -1], [0, 1], [1, 0]],
		[[0, -1], [-1, -1], [0, 1], [-1, 0]],
		[[1, 1], [0, -1], [1, 0], [0, -1]],
		[[1, -1], [0, 1], [1, 0], [0, 1]],
		[[-1, -1], [1, 0], [0, -1], [1, 0]],
		[[1, -1], [-1, 0], [0, -1], [-1, 0]],
		[[1, 1], [0, -1], [0, -1], [1, 0]],
		[[1, -1], [0, 1], [0, 1], [1, 0]],
		[[1, -1], [-1, 0], [-1, 0], [0, -1]],
		[[-1, -1], [1, 0], [1, 0], [0, -1]],
		[[1, 0], [-1, -1], [-1, 0], [1, -1]]]	# Bad shape
p18 = [[[1, 0], [1, 0], [0, 1], [-1, 0]],
		[[1, 0], [1, 0], [0, -1], [-1, 0]],	
		[[0, -1], [0, -1], [-1, 0], [0, 1]],
		[[0, -1], [0, -1], [1, 0], [0, 1]],
		[[0, -1], [1, 0], [0, 1], [0, 1]],
		[[0, -1], [1, 0], [1, 0], [-1, 1]],
		[[0, -1], [1, 0], [0, 1], [1, 0]],
		[[1, 0], [0, -1], [-1, 0], [0, -1]],
		[[1, 0], [0, -1], [-1, 0], [-1, 0]],
		[[1, 0], [0, -1], [-1, 0], [1, -1]]]	# Box
p19 = [[[1, 0], [0, 1], [1, 0], [1, 0]],
		[[1, 0], [0, -1], [1, 0], [1, 0]],
		[[0, -1], [1, 0], [0, -1], [0, -1]],
		[[0, -1], [-1, 0], [0, -1], [0, -1]],
		[[1, 0], [1, 0], [0, 1], [1, 0]],
		[[1, 0], [1, 0], [0, -1], [1, 0]],
		[[0, -1], [0, -1], [1, 0], [0, -1]],
		[[0, -1], [0, -1], [-1, 0], [0, -1]],
		[[1, 1], [0, -1], [-1, -1], [0, -1]],
		[[1, 0], [-1, -1], [-1, 0], [-1, 0]]]	# Step
p20 = [[[1, 0], [1, 1], [0, -1], [1, 0]],
		[[1, 0], [1, -1], [0, 1], [1, 0]],
		[[0, -1], [1, -1], [-1, 0], [0, -1]],
		[[0, -1], [-1, -1], [1, 0], [0, -1]],
		[[1, 1], [0, -1], [1, 0], [1, 0]],
		[[1, -1], [0, 1], [1, 0], [1, 0]],
		[[1, -1], [-1, 0], [0, -1], [0, -1]],
		[[-1, -1], [1, 0], [0, -1], [0, -1]],
		[[1, -1], [0, 1], [0, 1], [0, 1]],
		[[1, 1], [0, -1], [0, -1], [0, -1]],
		[[1, 1], [0, -1], [1, 0], [1, 0]],
		[[-1, 1], [0, -1], [-1, 0], [-1, 0]]]	# gun shape

ROTATIONS = [[1, 0, 0, -1], [-1, 0, 0, -1], [0, 1, -1, 0], [0, -1, -1, 0]]
PIECES = {0 : p0, 1 : p1, 2 : p2, 3 : p3, 4 : p4, 5 : p5, 6 : p6, 7 : p7,
		  8 : p8, 9 : p9, 10 : p10, 11 : p11, 12 : p12, 13 : p13, 14 : p14,
		  15 : p15, 16 : p16, 17 : p17, 18 : p18, 19 : p19, 20 : p20}
GRID_SIZE = 20
CLEAR = 0

class BlokusState(State):

	def __init__(self):
		self.board_data = [[0 for y in range(GRID_SIZE)] for x in range(GRID_SIZE)]
		self.player_data = {0 : {"pieces" : {k : 1 for k in range(21)}, "finished" : 0, "tiles" : [(-1, -1)]},
							1 : {"pieces" : {k : 1 for k in range(21)}, "finished" : 0, "tiles" : [(GRID_SIZE, -1)]},
							2 : {"pieces" : {k : 1 for k in range(21)}, "finished" : 0, "tiles" : [(GRID_SIZE, GRID_SIZE)]},
							3 : {"pieces" : {k : 1 for k in range(21)}, "finished" : 0, "tiles" : [(-1, GRID_SIZE)]}}
		self.player = 0
		self.winner = -1
		self.num_players = 4
		self.action = None

	def _get_tile(self, x, y):
		""" Get tile state from the board
		Check if the requested tile is within the board then
		return the tile value
		"""
		if x < 0 or x > GRID_SIZE-1 or y < 0 or y > GRID_SIZE-1:
			return -1
		return self.board_data[y][x]

	def _check_corner(self, pos1, pos2, pos3):
		if (self._get_tile(pos2[0], pos2[1]) == CLEAR and
			self._get_tile(pos1[0], pos1[1]) != self.player + 1 and 
			self._get_tile(pos2[0], pos2[1]) != self.player + 1 and 
			self._get_tile(pos3[0], pos3[1]) != self.player + 1):
				return 0
		return -1

	def _get_corners(self):
		""" Gets all the corners
		Go through all the tiles the player has on the board
		and check if any of them have an exposed corner
		"""
		corners = []

		for t in self.player_data[self.player]["tiles"]:
			# Top right
			if not self._check_corner((t[0], t[1]-1), (t[0] + 1, t[1] - 1), (t[0] + 1, t[1])):
				# self.state.player_data[self.state.player]["corners"].append(((t[0] + 1, t[1] - 1), 3))
				corners.append(((t[0] + 1, t[1] - 1), 3))


			# Bot right
			if not self._check_corner((t[0] + 1, t[1]), (t[0] + 1, t[1] + 1), (t[0], t[1] + 1)):
				# self.state.player_data[self.state.player]["corners"].append(((t[0] + 1, t[1] + 1), 0))
				corners.append(((t[0] + 1, t[1] + 1), 0))

			# Bot left
			if not self._check_corner((t[0], t[1] + 1), (t[0] - 1, t[1] + 1), (t[0] - 1, t[1])):
				# self.state.player_data[self.state.player]["corners"].append(((t[0] - 1, t[1] + 1), 1))
				corners.append(((t[0] - 1, t[1] + 1), 1))

			# Top Left
			if not self._check_corner((t[0] - 1, t[1]), (t[0] - 1, t[1] - 1), (t[0], t[1] - 1)):
				# self.state.player_data[self.state.player]["corners"].append(((t[0] - 1, t[1] - 1), 2))
				corners.append(((t[0] - 1, t[1] - 1), 2))

		# self.state.player_data[self.state.player]["tiles"] = []
		return corners

	def _check_for_adjacent(self, x, y):
		""" Check if the tile has tiles of the same colour 
		adjacent to it. 
		Check right, left up and down tiles - if they are the 
		same colour then it fails the test.
		"""
		# Check right tile
		if x + 1 <= GRID_SIZE-1 and self.board_data[y][x + 1] == self.player + 1:
			return 1
		# Check left tile
		if x - 1 >= 0 and self.board_data[y][x - 1] == self.player + 1:
			return 1
		# Check bottom tile
		if y + 1 <= GRID_SIZE-1 and self.board_data[y + 1][x] == self.player + 1:
			return 1
		# Check top tile
		if y - 1 >= 0 and self.board_data[y - 1][x] == self.player + 1:
			return 1
		return 0

	def _get_winner(self):
		scores = {0 : 0, 1 : 0, 2 : 0, 3 : 0}
		for x in range(GRID_SIZE):
			for y in range(GRID_SIZE):
				if not self.board_data[y][x] == 0:
					scores[self.board_data[y][x] - 1] += 1

		key = max(scores, key=scores.get)
		return key

		# if scores[0] == scores[2]:
		# 	return -1
		# else:
		# 	return key

	def _advance_player(self, state, p):
		player = (p + 1) % self.num_players
		while state.player_data[player]["finished"]:
			player = (player + 1) % self.num_players
			if player == p:
				return p
		return player

	def __str__(self):
		string = str(self.player) + "'s turn.\n\n"
		colours = [Fore.RED, Fore.BLUE, Fore.YELLOW, Fore.GREEN]
		
		# Print board
		for x in range(GRID_SIZE):
			for y in range(GRID_SIZE):
				if self.board_data[x][y] == 0:
					string += Fore.WHITE + ". "
				else:
					string += colours[self.board_data[x][y] - 1] + "■ "
			string += Fore.WHITE + "\n"
		string += "\n"

		# Print pieces remaining
		for i in range(4):
			string += colours[i] + "■: "
			string += Fore.WHITE
			for p, j in self.player_data[i]["pieces"].items():
				string += str(p) + ","
			string += str(len(self.player_data[i]["tiles"]))
			string += "\n"

		# Print winner if state terminal
		if self.is_terminal:
			if self.winner != -1:
				string += "\n" + colours[self.winner] + "■ " + Fore.WHITE + "wins.\n"

		return string

	def is_terminal(self):
		return self.player_data[0]["finished"] and self.player_data[1]["finished"] and self.player_data[2]["finished"] and self.player_data[3]["finished"]

	def get_all_actions(self):
		""" Get all possible moves for the player
		We iterate through all the actionable tiles (the clear tiles
		on corners) then for each available piece we check all its
		possible orientations given the corner orientation.
		"""
		legal_moves = []

		for coord, direction in self._get_corners():
			# Get rotation matrix based on direction 
			rot = ROTATIONS[direction]

			# Go through all pieces currently available
			for p_id, p in self.player_data[self.player]["pieces"].items():
				p = PIECES[p_id]
				
				# Go through possible orientations
				for orientation in p:

					next_x = coord[0]
					next_y = coord[1]
					move = [[coord[0], coord[1]]]
					valid = 1
					
					if self._check_for_adjacent(next_x, next_y):
						valid = 0
						break

					# Traverse through tile using the tile data to 
					# determine the direction of travel. 
					for t in orientation:
						
						next_x = next_x + (t[0] * rot[0] + t[1] * rot[1])
						next_y = next_y + (t[0] * rot[2] + t[1] * rot[3])
						move.append([next_x, next_y])

						# Check that we are putting tile into clear space
						if self._get_tile(next_x, next_y) != CLEAR:
							valid = 0
							break

						# Check that tile is not adjacent to existing tile
						if self._check_for_adjacent(next_x, next_y):
							valid = 0
							break

					if valid:
						legal_moves.append((coord, move, p_id))

		return legal_moves

	def do_random_action(self):
		""" Get random move """

		shuffled_corners = self._get_corners()
		random.shuffle(shuffled_corners)
		shuffled_pieces = list(self.player_data[self.player]["pieces"].items())
		random.shuffle(shuffled_pieces)

		for coord, direction in shuffled_corners:
			# Get rotation matrix based on direction 
			rot = ROTATIONS[direction]

			# Go through all pieces currently available
			for p_id, p in shuffled_pieces:
				shuffled_piece = PIECES[p_id]
				random.shuffle(shuffled_piece)
				# Go through possible orientations
				for orientation in shuffled_piece:

					next_x = coord[0]
					next_y = coord[1]
					move = [[coord[0], coord[1]]]
					valid = 1
					
					if self._check_for_adjacent(next_x, next_y):
						valid = 0
						break

					# Traverse through tile using the tile data to 
					# determine the direction of travel. 
					for t in orientation:
						
						next_x = next_x + (t[0] * rot[0] + t[1] * rot[1])
						next_y = next_y + (t[0] * rot[2] + t[1] * rot[3])
						move.append([next_x, next_y])

						# Check that we are putting tile into clear space
						if self._get_tile(next_x, next_y) != CLEAR:
							valid = 0
							break

						# Check that tile is not adjacent to existing tile
						if self._check_for_adjacent(next_x, next_y):
							valid = 0
							break

					if valid:
						return self.do_action((coord, move, p_id))

		return self.do_action(None)

	def do_action(self, action):
		next_state = copy.deepcopy(self)

		if action == None:
			next_state.player_data[self.player]["finished"] = 1
			next_state.player = self._advance_player(next_state, self.player)
			if next_state.is_terminal():
				next_state.winner = next_state._get_winner()
			return next_state

		next_state.action = action

		# Delete piece from inventory
		del next_state.player_data[self.player]["pieces"][action[2]]

		# Update the board with the new move
		for tile in action[1]:
			next_state.player_data[self.player]["tiles"].append([tile[0], tile[1]])
			next_state.board_data[tile[1]][tile[0]] = (self.player + 1)

		# Advance to the next player
		next_state.player = self._advance_player(next_state, self.player)

		return next_state


def run():

	init() 

	state = BlokusState()
	players = {0 : MCTS(0, 1), 1 : MCTS(1, 1), 2 : MCTS(2, 1), 3 : MCTS(3, 5000)}
	results = {0:0,1:0,2:0,3:0,-1:0}
	action = None
	
	while not state.is_terminal():
		t = time.time()
		action = players[state.player].choose(state)
		state = state.do_action(action)
		print(state)
		print("TIME: ", time.time() - t)

if __name__ == "__main__":
	run()
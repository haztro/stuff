from mcts import *
import random
import copy
import time


class TicTacToeState(State):

	def __init__(self):
		self.board_data = [[0 for x in range(3)] for y in range(3)]
		self.player = 0		# Current player
		self.winner = -1	# Who won 1:win, 0:loss, -1:draw
		self.action = None	# The action that led to this state
		self.finished = 0	# Is the game still going

	def __str__(self):
		" Nice print out of board "
		string = ""
		for y in range(3):
			string += "-------------\n"
			string += "|"
			for x in range(3):
				if self.board_data[y][x] == 1:
					string += " O "
				elif self.board_data[y][x] == -1:
					string += " X "
				else:
					string += "   "
				string += "|"
			string += "\n"
		string += "-------------\n"
		return string

	def is_terminal(self):
		return self.finished

	def get_all_actions(self):
		actions = []
		for y in range(3):
			for x in range(3):
				if self.board_data[y][x] == 0:
					actions.append([x, y])
		return actions

	def do_random_action(self):
		" Get "
		actions = self.get_all_actions()
		return self.do_action(random.choice(actions))

	def do_action(self, action):
		""" Update the board with the new action then check for 
		then check if we have three in a row. Probs really
		inefficient.  
		"""
		next_state = copy.deepcopy(self)

		# Update board state with action
		next_state.board_data[action[1]][action[0]] = 1 if self.player else -1
		next_state.action = action

		status = []
		# Rows
		status.append(abs(sum(next_state.board_data[0])))
		status.append(abs(sum(next_state.board_data[1])))
		status.append(abs(sum(next_state.board_data[2])))

		# Columns
		status.append(abs(sum([next_state.board_data[0][0], next_state.board_data[1][0], next_state.board_data[2][0]])))
		status.append(abs(sum([next_state.board_data[0][1], next_state.board_data[1][1], next_state.board_data[2][1]])))
		status.append(abs(sum([next_state.board_data[0][2], next_state.board_data[1][2], next_state.board_data[2][2]])))
		
		# Diagonals
		status.append(abs(sum([next_state.board_data[0][0], next_state.board_data[1][1], next_state.board_data[2][2]])))
		status.append(abs(sum([next_state.board_data[0][2], next_state.board_data[1][1], next_state.board_data[2][0]])))
		
		# We have three in a row so end game otherwise check if moves left on next turn
		if 3 in status:
			next_state.winner = self.player
			next_state.finished = 1
		elif len(next_state.get_all_actions()) == 0:
			next_state.winner = -1
			next_state.finished = 1
			
		# Advance player
		next_state.player ^= 1

		return next_state


def run():
	players = [MCTS(0, 10000), MCTS(1, 10)]
	results = {0 : 0, 1 : 0, -1 : 0}

	t = time.time()
	for i in range(100):
		state = TicTacToeState()

		while not state.is_terminal():
			action = players[state.player].choose(state)
			state = state.do_action(action)

		print(state)
		print(state.winner)
		results[state.winner] += 1

	print(results)
	print("Time: ", time.time() - t, "s")

if __name__ == "__main__":
	run()
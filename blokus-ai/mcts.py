from random import randint
from abc import ABC, abstractmethod
import math

class State(ABC):

	@abstractmethod
	def is_terminal(self):
		" Returns 1 if state is terminal 0 if not "
		pass

	@abstractmethod
	def get_all_actions(self):
		" Get all possible actions, return as list "
		pass

	@abstractmethod
	def do_random_action(self):
		" Perform a random action and return the new state "
		pass

	@abstractmethod
	def do_action(self, action):
		" Perform a given action and return the new state "
		pass


class Node():

	def __init__(self, state, parent):
		self.state = state
		self.parent = parent
		self.children = []
		self.q = 0
		self.n = 0
		self.expansion = state.get_all_actions()

class MCTS():

	def __init__(self, player, iterations):
		self.player = player
		self.nodes = []
		self.iterations = iterations

	def choose(self, state):
		" Perform MCTS and return the next action"
		root = Node(state, None)

		for i in range(self.iterations):
			leaf = self._select(root)
			if not leaf.state.is_terminal() and leaf.expansion:
				leaf = self._expand(leaf)
			reward = self._simulate(leaf)
			self._back_propagate(leaf, reward)

		if not root.children:
			return None
		return self._best_utc(root).state.action
			
	def _select(self, node):
		" Branch out using utc until we find a leaf node "
		current_node = node

		while current_node.children and not current_node.expansion:
			current_node = self._best_utc(current_node)

		return current_node

	def _expand(self, node):
		" Expand the selected node with all possible actions"
		action = node.expansion.pop(randint(0, len(node.expansion)-1))
		next_state = node.state.do_action(action)
		new_node = Node(next_state, node)
		node.children.append(new_node)		
		return new_node

	def _simulate(self, node):
		" Rollout with random play "
		start_node = node
		state = start_node.state
		while not state.is_terminal():
			state = state.do_random_action()

		# If opponent won invert the rewards
		if self.player == state.winner:
			return 1
		elif state.winner == -1:
			return 0.5
		else:
			return 0

	def _back_propagate(self, node, reward):
		" Traverse back through path updating node info"
		current_node = node
		while current_node != None:
			current_node.n += 1
			if not current_node.state.player == self.player:
				current_node.q += reward
			else:
				current_node.q += 1 - reward
			current_node = current_node.parent

	def _best_utc(self, node):
		" Choose the child with the heights utc values "
		choices = [(child.q / child.n) + 0.1 * math.sqrt((2 * math.log(node.n) / child.n)) for child in node.children if child.n > 0]
		return node.children[choices.index(max(choices))]
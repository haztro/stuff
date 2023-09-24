import pygame
from pygame.locals import *
from blokus import *
from colorama import Fore, init

GRID_WIDTH = 20

class BlokusGUI():

	def __init__(self):
		pass

	def draw(self, screen, board_data, action):
		colours = [(255, 255, 255), (255, 0, 0), (0, 0, 255), (255, 255, 0), (0, 255, 0), (0, 0, 0)]
		for y in range(GRID_SIZE):
			for x in range(GRID_SIZE):
				pygame.draw.rect(screen, colours[board_data[y][x]], Rect(x * (GRID_WIDTH - 1) + 0, y * (GRID_WIDTH - 1) + 0, GRID_WIDTH, GRID_WIDTH))
				pygame.draw.rect(screen, (0, 0, 0), Rect(x * (GRID_WIDTH - 1) + 0, y * (GRID_WIDTH - 1) + 0, GRID_WIDTH, GRID_WIDTH), 1)

		if action != None:
			for t in action[1][1]:
				pygame.draw.rect(screen, colours[action[3]], Rect(t[0] * (GRID_WIDTH - 1) + 0, t[1] * (GRID_WIDTH - 1) + 0, GRID_WIDTH, GRID_WIDTH))
				pygame.draw.rect(screen, (0, 0, 0), Rect(t[0] * (GRID_WIDTH - 1) + 0, t[1] * (GRID_WIDTH - 1) + 0, GRID_WIDTH, GRID_WIDTH), 1)


class Human():

	def __init__(self, id):
		self.id = id
		self.selected_piece = 0
		self.orientation = 0
		self.pos = (0, 0)
		self.confirm = 0
		self.finished = 0

	def handle_input(self, event, state):
		self.confirm = 0
		mouse_pos = pygame.mouse.get_pos()
		self.pos = (int(mouse_pos[0] / GRID_WIDTH), int(mouse_pos[1] / GRID_WIDTH))
		keys = pygame.key.get_pressed()

		if keys[K_SPACE]:
			self.finished = 1
			return

		if event.type == pygame.MOUSEBUTTONDOWN:
			if event.button == 1:
				self.confirm = 1
				return
			if event.button == 4:
				if keys[K_LCTRL]:
					p_id = list(state.player_data[self.id]["pieces"].keys())[self.selected_piece]
					self.orientation = (self.orientation + 1) % max(1, (len(PIECES[p_id])))
				else:
					self.selected_piece = (self.selected_piece + 1) % max(1, (len(state.player_data[self.id]["pieces"])))
			elif event.button == 5:
				if keys[K_LCTRL]:
					p_id = list(state.player_data[self.id]["pieces"].keys())[self.selected_piece]
					self.orientation = (self.orientation - 1) % max(1, (len(PIECES[p_id])))
				else:
					self.selected_piece = (self.selected_piece - 1) % max(1, (len(state.player_data[self.id]["pieces"])))



	def check_move(self, state):
		rot = ROTATIONS[0]
		self.selected_piece = max(0, min(self.selected_piece, len(state.player_data[self.id]["pieces"]) - 1))
		p_id = list(state.player_data[self.id]["pieces"].keys())[self.selected_piece]
		p = PIECES[p_id]
		valid = 1
		corners = [c[0] for c in state._get_corners()]

		next_x = self.pos[0]
		next_y = self.pos[1]
		move = [(self.pos[0], self.pos[1])]

		if state._check_for_adjacent(next_x, next_y):
			valid = 0
		if state._get_tile(next_x, next_y) != CLEAR:
			valid = 0

		self.orientation = max(0, min(self.orientation, len(PIECES[p_id]) - 1))

		for t in p[self.orientation]:
			
			next_x = next_x + (t[0] * rot[0] + t[1] * rot[1])
			next_y = next_y + (t[0] * rot[2] + t[1] * rot[3])
			move.append((next_x, next_y))

			if state._get_tile(next_x, next_y) != CLEAR:
				valid = 0
			if valid and state._check_for_adjacent(next_x, next_y):
				valid = 0

		safe = 0
		for m in move:
			if m in corners:
				safe = 1
				break

		valid &= safe

		if valid:
			return (valid, (self.pos, move, p_id), self.confirm, self.id + 1)
		return (valid, (self.pos, move, p_id), self.confirm, 5)

	def choose(self, state, event):
		return self.check_move(state)

def run():
	init()
	pygame.init()
	pygame.display.set_caption('Blokus')
	screen = pygame.display.set_mode(((GRID_SIZE) * GRID_WIDTH, (GRID_SIZE) * GRID_WIDTH))

	gui = BlokusGUI()
	state = BlokusState()
	players = {0 : MCTS(0, 1), 1 : MCTS(1, 1), 2 : MCTS(1, 5000), 3 : Human(3)}
	human_action = None

	while True:
		
		# Handle input
		for event in pygame.event.get():
			if event.type == pygame.QUIT:
				pygame.display.quit()
				pygame.quit()
				return

			# Update human player state with new input
			if type(players[state.player]) == Human:
				players[state.player].handle_input(event, state)


		# Update Game 
		if not state.is_terminal():
			# If not a human player update with MCTS otherwise 
			# check if human has chosen a valid move. 
			if type(players[state.player]) != Human:
				action = players[state.player].choose(state)
				state = state.do_action(action)
				print(state)
			else:
				human_action = players[state.player].choose(state, event)
				if players[state.player].finished:
					state = state.do_action(None)
				if human_action[0] and human_action[2]:
					state = state.do_action(human_action[1])
					print(state)
		else:
			print(state.winner)
			break

		screen.fill((255, 255, 255))

		gui.draw(screen, state.board_data, human_action)

		pygame.display.update()


if __name__ == "__main__":
	run()
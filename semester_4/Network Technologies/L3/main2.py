import random

# Constants
NETWORK_LENGTH = 80
SIMULATION_STEPS = 4000

medium = ["-"] * NETWORK_LENGTH
class Station:
    def __init__(self, position, state, charakter):
        self.position = position
        self.state = state
        self.charakter = charakter

    def change_state(self, new_state):
        self.state = new_state


def propagate_signal(medium, signals, tick):





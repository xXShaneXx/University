
MEDIUM_LENGTH = 80
MAX_ATTEMPTS = 32

class Printer:
    RESET = "\033[0m"

    @staticmethod
    def rgb_to_ansi(r, g, b, background=False):
        return f"\033[{48 if background else 38};2;{r};{g};{b}m"

    @staticmethod
    def get_signal_color(age, max_age=2*MEDIUM_LENGTH):
        """Returns an RGB color based on signal age (fades from bright to dark)"""
        if age > max_age:
            age = max_age
        intensity = int(200 * (age / max_age) ** (1/2))
        return Printer.rgb_to_ansi(intensity, intensity, 255)

    @staticmethod
    def get_jam_color(age, max_age=2*MEDIUM_LENGTH):
        """Returns an RGB color for jamming signals (red fading to dark red)"""
        if age > max_age:
            age = max_age
        intensity = int(200 * (age / max_age) ** (1/2))
        return Printer.rgb_to_ansi(255, intensity, intensity)

    @staticmethod
    def colorize_medium(medium):
        colored_medium = []
        for cell in medium:
            if cell is None:
                colored_medium.append("_")
            else:
                source, age = cell
                if source == "x":
                    color = Printer.get_jam_color(age)
                else:
                    color = Printer.get_signal_color(age)
                colored_medium.append(f"{color}{source}{Printer.RESET}")
        return "".join(colored_medium)

import random
from enum import Enum


class NodeState(Enum):
    IDLE = "idle"
    TRANSMITTING = "transmitting"
    WAITING = "waiting"
    JAMMING = "jamming"
    BACKOFF = "backoff"
    SUCCESS = "success"


class Node:
    """
    Represents a part in a telecommunication medium, simulated as a node array.
    """

    def __init__(self, name: str, position: int, transmission_tick: float = -1) -> None:
        """
        Initialize a Node instance with basic parameters.
            name: Unique identifier for the node.
            position: Physical coordinate (x) of the node in the medium.
            send_time: Scheduled transmission tick for initial message.
        """
        self.name = name
        self.position = position

        self.transmission_tick = random.randint(
            0, 1000) if transmission_tick < 0 else transmission_tick
        self.state = NodeState.IDLE

        self.backoff = 0
        self.attempts = 0
        self.jam_start = 0

    def position(self) -> int:
        """
        Return the physical position of the node in the medium.
        """
        return self.position

    def __str__(self) -> str:
        """
        Return a human-readable string representation of the node.
        """
        return f"{self.name}(x={self.position})"

    def __repr__(self) -> str:
        """
        Return an unambiguous string representation of the node.
        """
        return (f"Node(name='{self.name}', position={self.position}, "
                f"send_time={self.transmission_tick})")
import time
import random

def propagate_signal(medium, signals, tick):
    new_signals = []
    signal_ages = {}

    for signal in signals:
        pos, direction, source, created = signal
        age = tick - created
        signal_ages[(pos, direction, source)] = age

    for signal in signals:
        pos, direction, source, created = signal
        new_pos = pos + direction * 1
        if 0 <= new_pos < len(medium):
            if medium[new_pos] is None:
                medium[new_pos] = (source, tick - created)
            else:
                if medium[new_pos][0] != source:
                    medium[new_pos] = ("x", tick - created)
            new_signals.append((new_pos, direction, source, created))
    return new_signals

def is_medium_idle(medium, position):
    return medium[position] is None or medium[position][0] is None

def run_simulation(nodes):
    medium = [None] * MEDIUM_LENGTH
    signals = []
    log = []
    success = 0
    node_states_per_tick = [{node.name: NodeState.IDLE for node in nodes}]
    node_attempts_per_tick = [{node.name: 0 for node in nodes}]
    node_next_transmission = [{node.name: node.backoff for node in nodes}]
    positions = [" " for _ in range(MEDIUM_LENGTH)]

    for node in nodes:
        positions[node.position] = node.name

    tick = 0
    while True:
        tick += 1
        log.append("")

        signals = propagate_signal(medium, signals, tick)

        for node in nodes:
            if tick == node.transmission_tick and node.state == NodeState.IDLE:
                if is_medium_idle(medium, node.position):
                    medium[node.position] = (node.name, 0)
                    signals.append((node.position, -1, node.name, tick))
                    signals.append((node.position, 1, node.name, tick))
                    node.state = NodeState.TRANSMITTING
                else:
                    node.transmission_tick += 1

            elif node.state == NodeState.TRANSMITTING:
                if medium[node.position] is not None and medium[node.position][0] not in [node.name, None]:
                    node.state = NodeState.BACKOFF
                    if node.attempts < MAX_ATTEMPTS:
                        node.backoff = 2 * MEDIUM_LENGTH * random.randint(0, 2 ** node.attempts)
                        node.transmission_tick = tick + 2 * MEDIUM_LENGTH + 1 + node.backoff
                    node.attempts += 1
                    node.state = NodeState.JAMMING
                    node.jam_start = tick
                    medium[node.position] = ("x", 0)
                    log[-1] += f"{node} collision "
                else:
                    if tick >= node.transmission_tick + 2 * MEDIUM_LENGTH:
                        node.state = NodeState.SUCCESS
                        success += 1
                        log[-1] += f"{node} successful "
                    else:
                        medium[node.position] = (node.name, 0)
                        signals.append((node.position, -1, node.name, tick))
                        signals.append((node.position, 1, node.name, tick))
            elif node.state == NodeState.JAMMING:
                if tick >= node.jam_start + 2 * MEDIUM_LENGTH:
                    node.state = NodeState.IDLE
                else:
                    medium[node.position] = ("x", 0)
                    signals.append((node.position, -1, "x", tick))
                    signals.append((node.position, 1, "x", tick))

        node_attempts_per_tick.append({node.name: node.attempts for node in nodes})
        node_states_per_tick.append({node.name: node.state for node in nodes})
        node_next_transmission.append({node.name: node.transmission_tick for node in nodes})
        colored_medium = Printer.colorize_medium(medium)
        log[-1] = colored_medium + " " + log[-1]

        medium = [None for _ in medium]

        if success == len(nodes):
            print(success)
            break

    print("\nSteps")
    for i in range(len(log) - 1, 0, -1):
        if log[i] == "".join(["_" for _ in range(MEDIUM_LENGTH)]) + " ":
            log.pop(i)
        else:
            break

    with open("output.txt", "w") as f:
        for line in log:
            clean_line = re.sub(r'\033\[[0-9;]*[mK]', '', line)
            f.write(clean_line + "\n")

    for i, line in enumerate(log):
        print("\033[2J\033[H", end="")
        print(f"           {"".join(positions)[:MEDIUM_LENGTH]}")
        for j in range(max(0, i - 20), i + 1):
            print(f"Tick {str(j).zfill(5)} {log[j]}")

        print("\nNode States:")
        for node in nodes:
            print(f"Node {node.name}: Attempt={node_attempts_per_tick[i][node.name]} Next={node_next_transmission[i][node.name]} State={node_states_per_tick[i][node.name]}")
        time.sleep(0.05)

if __name__ == "__main__":
    import re
    nodes = [
        Node(chr(97+i), random.randint(0, MEDIUM_LENGTH - 1), random.randint(0, 100))
        for i in range(random.randint(8, 10))
    ]
    run_simulation(nodes)
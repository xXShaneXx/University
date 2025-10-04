import random
from enum import Enum

MEDIUM_LENGTH = 80
MAX_ATTEMPTS = 32

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

        # Add frame persistence - represents the "same data" the station wants to send
        self.frame_to_send = f"Frame_{self.name}_{random.randint(1000, 9999)}"  # Unique frame ID
        self.original_transmission_tick = self.transmission_tick  # Remember when we first wanted to send

    def get_position(self) -> int:
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

def display_medium(medium):
    """Simple display of medium state without colors"""
    display = []
    for cell in medium:
        if cell is None:
            display.append("_")
        else:
            source, age = cell
            display.append(source)
    return "".join(display)

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
                    log[-1] += f"{node} collision (retrying {node.frame_to_send}) "
                else:
                    if tick >= node.transmission_tick + 2 * MEDIUM_LENGTH:
                        node.state = NodeState.SUCCESS
                        node.attempts = 0  # Reset attempts after successful transmission
                        success += 1
                        log[-1] += f"{node} successful (sent {node.frame_to_send}) "
                        # Generate new frame for next transmission
                        node.frame_to_send = f"Frame_{node.name}_{random.randint(1000, 9999)}"
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

        # Simple display without colors
        medium_display = display_medium(medium)
        log[-1] = medium_display + " " + log[-1]

        medium = [None for _ in medium]

        if success == len(nodes):
            print(f"Simulation completed. All {success} nodes transmitted successfully.")
            break

    # Clean up empty log entries
    for i in range(len(log) - 1, 0, -1):
        if log[i] == "_" * MEDIUM_LENGTH + " ":
            log.pop(i)
        else:
            break

    # Write to output file
    with open("output.txt", "w") as f:
        f.write("CSMA/CD Simulation Results\n")
        f.write("=" * 50 + "\n")
        f.write(f"Medium Length: {MEDIUM_LENGTH}\n")
        f.write(f"Number of Nodes: {len(nodes)}\n")
        f.write("Node Positions: " + ", ".join([f"{node.name}@{node.position}" for node in nodes]) + "\n")
        f.write("=" * 50 + "\n\n")

        for i, line in enumerate(log):
            f.write(f"Tick {str(i).zfill(4)}: {line}\n")

        f.write("\n" + "=" * 50 + "\n")
        f.write("Final Node Statistics:\n")
        for node in nodes:
            f.write(f"Node {node.name}: Position={node.position}, Attempts={node.attempts}, State={node.state.value}\n")

    # Simple console output
    print("\nSimulation Summary:")
    print("=" * 30)
    for node in nodes:
        print(f"Node {node.name}: Position={node.position}, Attempts={node.attempts}, Final State={node.state.value}")
    print(f"\nTotal simulation ticks: {len(log)}")
    print("Results saved to output.txt")

if __name__ == "__main__":
    # Create nodes with same transmission times to force collisions for demonstration
    nodes = [
        Node('a', 10, 5),   # All nodes try to transmit at tick 5
        Node('b', 30, 5),
        Node('c', 50, 5),
    ]

    print("Starting CSMA/CD Simulation")
    print("Node configuration:")
    for node in nodes:
        print(f"  {node} - Frame: {node.frame_to_send}")
    print()

    run_simulation(nodes)
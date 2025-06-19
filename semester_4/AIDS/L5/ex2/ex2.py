import random
from collections import defaultdict

def generate_tree(n, root):
    if n < 1 or root < 0 or root >= n:
        raise ValueError("Nieprawidłowe dane wejściowe")
    nodes = list(range(n))
    nodes.remove(root)
    random.shuffle(nodes)
    tree = defaultdict(list)
    parents = [root]
    for node in nodes:
        parent = random.choice(parents)
        tree[parent].append(node)
        tree[node].append(parent)
        parents.append(node)
    return dict(tree)

def build_tree(tree, root):
    # Converts undirected adjacency list to rooted tree (children only)
    children = defaultdict(list)
    def dfs(u, parent):
        for v in tree[u]:
            if v != parent:
                children[u].append(v)
                dfs(v, u)
    dfs(root, -1)
    return children

def inform_order(tree, root):
    children = build_tree(tree, root)
    order = {}
    dp = {}
    def dfs(u):
        child_times = []
        for v in children[u]:
            dfs(v)
            child_times.append((dp[v], v))
        # Sort children by their dp descending
        child_times.sort(reverse=True)
        dp[u] = 0
        for i, (child_dp, v) in enumerate(child_times):
            dp[u] = max(dp[u], child_dp + i + 1)
        # Save the order in which to inform children
        order[u] = [v for _, v in child_times]
    dfs(root)
    return order, dp

def main():
    n = 6
    root = 0
    tree = generate_tree(n, root)
    print("Adjacency list:")
    for node, neighbors in tree.items():
        print(f"{node}: {neighbors}")
    order, dp = inform_order(tree, root)
    print("\nOrder to inform children for each node:")
    for node in range(n):
        print(f"{node}: {order.get(node, [])}")
    print(f"\nMinimum rounds needed: {dp[root]}")

if __name__ == "__main__":
    main()
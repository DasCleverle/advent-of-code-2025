Graph ParseGraph(string input) {
    var lines = input.Split(Environment.NewLine);
    var nodes = new Dictionary<string, Node>();

    foreach (var line in lines) {
        var colon = line.IndexOf(':');

        if (colon == -1) {
            continue;
        }

        var name = line[..colon];
        var neighborNames = line[(colon + 1)..].Split(' ', StringSplitOptions.RemoveEmptyEntries | StringSplitOptions.TrimEntries);

        if (!nodes.TryGetValue(name, out var node)) {
            node = new Node { Name = name };
            nodes[node.Name] = node;
        }

        foreach (var neighbor in neighborNames) {
            if (!nodes.TryGetValue(neighbor, out var neighborNode)) {
                neighborNode = new Node { Name = neighbor };
                nodes[neighbor] = neighborNode;
            }

            node.Neighbors.Add(neighborNode);
        }

    }

    var graph = new Graph {
        Nodes = nodes.Values,
        StartNode = nodes["you"]
    };

    return graph;
}

int SearchPaths(Node node, HashSet<Node> visited, Dictionary<string, int> cache) {
    if (node.Name == "out") {
        return 1;
    }

    if (cache.TryGetValue(node.Name, out var cached)) {
        return cached;
    }

    if (!visited.Add(node)) {
        return 0;
    }

    var result = 0;

    foreach (var neighbor in node.Neighbors) {
        var count = SearchPaths(neighbor, visited, cache);
        result += count;
    }

    cache.Add(node.Name, result);
    return result;
}

void Part1(string input) {
    var graph = ParseGraph(input);
    var paths = SearchPaths(graph.StartNode, new HashSet<Node>(), new Dictionary<string, int>());

    Console.WriteLine(paths);
}

void Part2(string input) { }

if (args.Length != 2) {
    Console.Error.WriteLine("Requires 2 arguments");
    Environment.Exit(1);
}

var input = await File.ReadAllTextAsync(args[1]);

switch (args[0]) {
    case "1":
        Part1(input);
        break;

    case "2":
        Part2(input);
        break;
}

public class Graph {

    public required IReadOnlyCollection<Node> Nodes { get; init; }

    public required Node StartNode { get; init; }

}

public class Node {

    public required string Name { get; init; }

    public List<Node> Neighbors { get; } = [];

}

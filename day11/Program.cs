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

static Graph ParseGraph(string input) {
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

    var graph = new Graph { Nodes = nodes };

    return graph;
}

static int SearchPathsPart1(Node node, HashSet<Node> visited, Dictionary<string, int> cache) {
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
        var count = SearchPathsPart1(neighbor, visited, cache);
        result += count;
    }

    cache.Add(node.Name, result);
    return result;
}

static void Part1(string input) {
    var graph = ParseGraph(input);
    var paths = SearchPathsPart1(graph.Nodes["you"], [], new Dictionary<string, int>());

    Console.WriteLine(paths);
}

static ulong SearchPathsPart2(Node node, HashSet<Part2Key> visited, HashSet<string> currentPath, Dictionary<Part2Key, ulong> cache) {
    var passedFft = currentPath.Contains("fft");
    var passedDac = currentPath.Contains("dac");
    var key = new Part2Key(node.Name, passedFft, passedDac);

    if (node.Name == "out") {
        return passedDac && passedFft ? 1UL : 0UL;
    }

    if (cache.TryGetValue(key, out var cached)) {
        return cached;
    }

    if (!visited.Add(key)) {
        return 0;
    }

    var total = 0UL;

    foreach (var neighbor in node.Neighbors) {
        currentPath.Add(neighbor.Name);
        var result = SearchPathsPart2(neighbor, visited, currentPath, cache);
        total += result;

        currentPath.Remove(neighbor.Name);
    }

    cache[key] = total;
    return total;
}

static void Part2(string input) {
    var graph = ParseGraph(input);
    var paths = SearchPathsPart2(graph.Nodes["svr"], [], ["svr"],  new Dictionary<Part2Key, ulong>());

    Console.WriteLine(paths);
}

public readonly record struct Part2Key(string Name, bool PassedFft, bool PassedDac);

public class Graph {

    public required Dictionary<string, Node> Nodes { get; init; }

}

public class Node {

    public required string Name { get; init; }

    public List<Node> Neighbors { get; } = [];

}

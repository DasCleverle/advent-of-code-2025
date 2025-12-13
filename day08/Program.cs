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

static List<JunctionBox> ParseInput(string input) {
    var lines = input.Split(Environment.NewLine);
    var boxes = new List<JunctionBox>();

    foreach (var line in lines) {
        if (line.Length == 0) {
            continue;
        }

        var parts = line.Split(',');

        var x = int.Parse(parts[0]);
        var y = int.Parse(parts[1]);
        var z = int.Parse(parts[2]);

        var box = new JunctionBox(x, y, z);
        boxes.Add(box);
    }

    return boxes;
}

static IEnumerable<Pair> GetPairs(List<JunctionBox> boxes) {
    var pairs = new List<Pair>();

    foreach (var (i, left) in boxes.Index()) {
        foreach (var (j, right) in boxes.Index()) {
            if (i >= j) {
                continue;
            }

            pairs.Add(new Pair(i, j, left.DistanceTo(right)));
        }
    }

    return pairs.OrderBy(x => x.Distance);
}

static void Part1(string input) {
    var boxes = ParseInput(input);
    var pairs = GetPairs(boxes).Take(boxes.Count);

    var unionFind = new UnionFind(boxes.Count);

    foreach (var pair in pairs) {
        unionFind.Union(pair.IndexLeft, pair.IndexRight);
    }

    var sizes = unionFind.Sizes
        .OrderByDescending(x => x)
        .ToArray();

    Console.WriteLine(sizes[0] * sizes[1] * sizes[2]);
}

static void Part2(string input) {
    var boxes = ParseInput(input);
    var pairs = GetPairs(boxes);

    var unionFind = new UnionFind(boxes.Count);

    foreach (var pair in pairs) {
        unionFind.Union(pair.IndexLeft, pair.IndexRight);

        if (unionFind.Max == boxes.Count) {
            var left = boxes[pair.IndexLeft];
            var right = boxes[pair.IndexRight];

            Console.WriteLine((long)left.X * (long)right.X);
            break;
        }
    }
}

public class UnionFind {

    public int[] Parent { get; }

    public int[] Sizes { get; }

    public int Max { get; private set; }

    public UnionFind(int size) {
        Parent = Enumerable.Range(0, size).ToArray();
        Sizes = Enumerable.Repeat(1, size).ToArray();
    }

    public int Find(int item) {
        if (Parent[item] == item) {
            return item;
        }

        return Find(Parent[item]);
    }

    public void Union(int a, int b) {
        var rootA = Find(a);
        var rootB = Find(b);

        if (rootA == rootB) {
            return;
        }

        if (Sizes[rootA] < Sizes[rootB]) {
            Parent[rootA] = rootB;
            Sizes[rootB] += Sizes[rootA];
            Max = Math.Max(Max, Sizes[rootB]);
        }
        else {
            Parent[rootB] = rootA;
            Sizes[rootA] += Sizes[rootB];
            Max = Math.Max(Max, Sizes[rootA]);
        }
    }

}

public readonly record struct JunctionBox(int X, int Y, int Z) {

    public double DistanceTo(JunctionBox other) {
        return Math.Sqrt(
            Math.Pow(X - other.X, 2)
            + Math.Pow(Y - other.Y, 2)
            + Math.Pow(Z - other.Z, 2)
        );
    }

}

public readonly record struct Pair(int IndexLeft, int IndexRight, double Distance);

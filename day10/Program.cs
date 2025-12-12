using System.Diagnostics;
using System.Text;
using System.Text.RegularExpressions;
using Google.OrTools.LinearSolver;

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

static List<Panel> ParseInput(string input) {
    var lines = input.Split(Environment.NewLine);
    var panels = new List<Panel>();

    foreach (var line in lines) {
        var closingBracket = line.IndexOf(']');
        var openingBrace = line.IndexOf('{');

        if (closingBracket == -1 || openingBrace == -1) {
            continue;
        }

        var targetMaskString = line[1..closingBracket];
        var maskSize = targetMaskString.Length;
        var targetMask = new Mask(0L, maskSize);

        for (var i = 0; i < maskSize; i++) {
            if (targetMaskString[i] == '#') {
                targetMask |= 1UL << (maskSize - 1 - i);
            }
        }

        var buttonIndexes = line[(closingBracket + 1)..(openingBrace - 1)]
            .Split(' ', StringSplitOptions.RemoveEmptyEntries | StringSplitOptions.TrimEntries);

        var buttonMasks = new List<Mask>();

        foreach (var list in buttonIndexes) {
            var matches = Regexes.ButtonIndexRe.Matches(list);
            var mask = new Mask(0, maskSize);

            foreach (Match match in matches) {
                var index = int.Parse(match.Groups[1].Value);
                mask |= 1UL << (maskSize - 1 - index);
            }

            buttonMasks.Add(mask);
        }

        var joltageMatches = Regexes.JoltageRe.Matches(line[openingBrace..]);
        var joltages = new int[joltageMatches.Count];

        for (var i = 0; i < joltages.Length; i++) {
            joltages[i] = int.Parse(joltageMatches[i].Groups[1].Value);
        }

        var panel = new Panel {
            TargetMask = targetMask,
            Buttons = buttonMasks,
            TargetJoltages = joltages
        };

        panels.Add(panel);
    }

    return panels;
}

static long Solve(Panel panel) {
    var queue = new Queue<(Mask Mask, int Presses)>();
    queue.Enqueue((new Mask(0UL, panel.TargetMask.Length), 0));

    var minPresses = long.MaxValue;

    while (queue.TryDequeue(out var item)) {
        var (mask, presses) = item;

        if (presses >= minPresses) {
            continue;
        }

        foreach (var button in panel.Buttons) {
            var newMask = mask ^ button;

            if (newMask == panel.TargetMask) {
                minPresses = Math.Min(minPresses, presses + 1);
                continue;
            }

            queue.Enqueue((newMask, presses + 1));
        }
    }

    return minPresses;
}

static void Part1(string input) {
    var panels = ParseInput(input);
    var total = 0L;

    foreach (var panel in panels) {
        total += Solve(panel);
    }

    Console.WriteLine(total);
}

static void Part2(string input) {
    var panels = ParseInput(input);
    var total = 0d;

    foreach (var panel in panels) {
        using var solver = Solver.CreateSolver("CBC_MIXED_INTEGER_PROGRAMMING");

        var target = panel.TargetJoltages;
        var matrix = panel.Buttons
            .Select(x => x.GetBinaryVector())
            .ToArray();

        var dimension = target.Length;
        var width = matrix.Length;

        var coefficients = new Variable[matrix.Length];
        for (var i = 0; i < width; i++) {
            coefficients[i] = solver.MakeIntVar(0, double.MaxValue, ((char)((byte)'a' + i)).ToString());
        }

        for (var d = 0; d < dimension; d++) {
            var expressions = new LinearExpr[width];

            for (var i = 0; i < width; i++) {
                expressions[i] = matrix[i][d] * coefficients[i];
            }

            solver.Add(expressions.Sum() == target[d]);
        }

        solver.Minimize(coefficients.Sum());

        var status = solver.Solve();

        if (status == Solver.ResultStatus.OPTIMAL) {
            var solution = solver.Objective().Value();
            total += solution;
        }
    }

    Console.WriteLine(total);
}

public class Panel {

    public Mask TargetMask { get; init; }

    public List<Mask> Buttons { get; init; } = [];

    public int[] TargetJoltages { get; init; } = [];

}

[DebuggerDisplay("Mask = {Bits}, Bits = {DebuggerDisplay()}")]
public readonly struct Mask : IEquatable<Mask> {

    public Mask(ulong bits, int length) {
        Bits = bits;
        Length = length;
    }

    public ulong Bits { get; }

    public int Length { get; }

    public int[] GetBinaryVector() {
        var vector = new int[Length];

        for (var i = 0; i < Length; i++) {
            var value = (Bits & (1UL << Length - 1 - i)) != 0 ? 1 : 0;
            vector[i] = value;
        }

        return vector;
    }

    public static Mask operator |(Mask mask, ulong l) {
        return new Mask(mask.Bits | l, mask.Length);
    }

    public static Mask operator &(Mask mask, ulong l) {
        return new Mask(mask.Bits & l, mask.Length);
    }

    public static Mask operator ^(Mask mask, ulong l) {
        return new Mask(mask.Bits ^ l, mask.Length);
    }

    public static bool operator ==(Mask a, Mask b) {
        return a.Equals(b);
    }

    public static bool operator !=(Mask a, Mask b) {
        return !a.Equals(b);
    }

    public static implicit operator ulong(Mask m) {
        return m.Bits;
    }

    public string DebuggerDisplay() {
        var sb = new StringBuilder(Length);

        for (var i = Length - 1; i >= 0; i--) {
            sb.Append((Bits & (1UL << i)) != 0 ? '1' : '0');
        }

        return sb.ToString();
    }

    public bool Equals(Mask other) {
        return Bits == other.Bits;
    }

    public override bool Equals(object? obj) {
        return obj is Mask other && Equals(other);
    }

    public override int GetHashCode() {
        return HashCode.Combine(Bits);
    }
}

public static partial class Regexes {
    [GeneratedRegex(@"(\d+)[,)]")]
    public static partial Regex ButtonIndexRe { get; }

    [GeneratedRegex(@"(\d+)[,}]")]
    public static partial Regex JoltageRe { get; }
}

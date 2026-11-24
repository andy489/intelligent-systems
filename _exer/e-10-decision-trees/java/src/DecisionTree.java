import java.io.*;
import java.util.*;

public class DecisionTree {

    // ─── Data ─────────────────────────────────────────────────────────────────

    static String[] headers;
    static int classCol;

    static List<String[]> readCSV(String path) throws IOException {
        List<String[]> rows = new ArrayList<>();
        try (BufferedReader br = new BufferedReader(new FileReader(path))) {
            String line;
            boolean first = true;
            while ((line = br.readLine()) != null) {
                line = line.trim();
                if (line.isEmpty()) continue;
                String[] row = line.split(",");
                if (first) { headers = row; first = false; }
                else        rows.add(row);
            }
        }
        return rows;
    }

    // ─── Entropy / Gain ───────────────────────────────────────────────────────

    static double entropy(List<String[]> rows) {
        Map<String, Integer> cnt = new HashMap<>();
        for (String[] r : rows) cnt.merge(r[classCol], 1, Integer::sum);
        double n = rows.size(), h = 0;
        for (int c : cnt.values()) {
            double p = c / n;
            h -= p * (Math.log(p) / Math.log(2));
        }
        return h;
    }

    static double gain(List<String[]> rows, int attr) {
        double base = entropy(rows);
        Map<String, List<String[]>> parts = new LinkedHashMap<>();
        for (String[] r : rows) parts.computeIfAbsent(r[attr], k -> new ArrayList<>()).add(r);
        double n = rows.size(), rem = 0;
        for (List<String[]> sub : parts.values())
            rem += (sub.size() / n) * entropy(sub);
        return base - rem;
    }

    // ─── Node ─────────────────────────────────────────────────────────────────

    static class Node {
        boolean isLeaf;
        String  label;
        int     attrIdx = -1;
        Map<String, Node> children = new LinkedHashMap<>();
    }

    static String majority(List<String[]> rows) {
        Map<String, Integer> cnt = new HashMap<>();
        for (String[] r : rows) cnt.merge(r[classCol], 1, Integer::sum);
        return cnt.entrySet().stream().max(Map.Entry.comparingByValue()).get().getKey();
    }

    static Node build(List<String[]> rows, List<Integer> attrs, String defaultClass) {
        Node node = new Node();

        if (rows.isEmpty()) {
            node.isLeaf = true; node.label = defaultClass; return node;
        }

        String first = rows.get(0)[classCol];
        if (rows.stream().allMatch(r -> r[classCol].equals(first))) {
            node.isLeaf = true; node.label = first; return node;
        }

        if (attrs.isEmpty()) {
            node.isLeaf = true; node.label = majority(rows); return node;
        }

        int bestAttr = attrs.get(0);
        double bestGain = -1;
        for (int a : attrs) {
            double g = gain(rows, a);
            if (g > bestGain) { bestGain = g; bestAttr = a; }
        }

        node.attrIdx = bestAttr;
        String maj = majority(rows);

        List<Integer> remaining = new ArrayList<>(attrs);
        remaining.remove(Integer.valueOf(bestAttr));

        Map<String, List<String[]>> parts = new LinkedHashMap<>();
        for (String[] r : rows) parts.computeIfAbsent(r[bestAttr], k -> new ArrayList<>()).add(r);

        for (Map.Entry<String, List<String[]>> e : parts.entrySet())
            node.children.put(e.getKey(), build(e.getValue(), remaining, maj));

        return node;
    }

    // ─── Print tree ───────────────────────────────────────────────────────────

    static void printTree(Node node, String prefix) {
        if (node.isLeaf) return;
        for (Map.Entry<String, Node> e : node.children.entrySet()) {
            Node child = e.getValue();
            if (child.isLeaf) {
                System.out.println(prefix + headers[node.attrIdx] + " = " + e.getKey() + " -> " + child.label);
            } else {
                System.out.println(prefix + headers[node.attrIdx] + " = " + e.getKey());
                printTree(child, prefix + "  ");
            }
        }
    }

    // ─── Classify ─────────────────────────────────────────────────────────────

    static String classify(Node node, String[] row) {
        if (node.isLeaf) return node.label;
        Node child = node.children.get(row[node.attrIdx]);
        if (child == null) return node.children.values().iterator().next().label;
        return classify(child, row);
    }

    // ─── Main ─────────────────────────────────────────────────────────────────

    public static void main(String[] args) throws IOException {
        String csvPath = args.length >= 1 ? args[0] : "data/play_tennis.csv";
        String query   = args.length >= 2 ? args[1] : "";

        List<String[]> rows = readCSV(csvPath);
        classCol = headers.length - 1;

        List<Integer> attrs = new ArrayList<>();
        for (int i = 0; i < classCol; i++) attrs.add(i);

        Node root = build(rows, attrs, "");

        if (!query.isEmpty()) {
            System.out.println(classify(root, query.split(",")));
            return;
        }

        System.out.println("Decision Tree:");
        printTree(root, "");

        int correct = 0;
        for (String[] r : rows) if (classify(root, r).equals(r[classCol])) correct++;
        System.out.printf("%nTraining accuracy: %d/%d (%.2f%%)%n", correct, rows.size(),
                100.0 * correct / rows.size());

        String attrList = String.join(" ", Arrays.copyOf(headers, classCol));
        System.out.println("\nEnter attributes (" + attrList + ") or 'quit':");
        Scanner sc = new Scanner(System.in);
        while (sc.hasNextLine()) {
            System.out.print("> ");
            String line = sc.nextLine().trim();
            if (line.equals("quit")) break;
            String[] vals = line.split("\\s+");
            if (vals.length != classCol) {
                System.out.println("Expected " + classCol + " values.");
                continue;
            }
            System.out.println("Prediction: " + classify(root, vals));
        }
    }
}

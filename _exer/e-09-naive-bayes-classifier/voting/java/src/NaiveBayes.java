import java.io.*;
import java.net.URISyntaxException;
import java.util.*;

public class NaiveBayes {

    static final int NUM_ATTRS = 16;
    static final String[] VALUES = {"y", "n", "?"};
    static final int K = VALUES.length;

    public static void main(String[] args) throws IOException, URISyntaxException {
        String jar = NaiveBayes.class.getProtectionDomain().getCodeSource().getLocation().toURI().getPath();
        String dataPath = new File(jar).getParentFile().getParent() + "/data/house-votes-84.data";
        List<String[]> data = new ArrayList<>();

        try (BufferedReader br = new BufferedReader(new FileReader(dataPath))) {
            String line;
            while ((line = br.readLine()) != null) {
                line = line.trim();
                if (!line.isEmpty()) data.add(line.split(","));
            }
        }

        Collections.shuffle(data, new Random(42));

        int n = data.size();
        int numFolds = 10;
        int[] foldSizes = new int[numFolds];
        int base = n / numFolds, extra = n % numFolds;
        for (int i = 0; i < numFolds; i++) foldSizes[i] = base + (i < extra ? 1 : 0);

        double totalAcc = 0;
        int start = 0;
        for (int fold = 0; fold < numFolds; fold++) {
            int end = start + foldSizes[fold];
            List<String[]> test = data.subList(start, end);
            List<String[]> train = new ArrayList<>();
            train.addAll(data.subList(0, start));
            train.addAll(data.subList(end, n));

            // Build model
            Set<String> classSet = new HashSet<>();
            for (String[] row : train) classSet.add(row[0]);
            Map<String, Integer> classCount = new HashMap<>();
            // count[cls][attr][val]
            Map<String, int[][]> counts = new HashMap<>();
            for (String cls : classSet) {
                classCount.put(cls, 0);
                counts.put(cls, new int[NUM_ATTRS][K]);
            }
            for (String[] row : train) {
                String cls = row[0];
                classCount.put(cls, classCount.get(cls) + 1);
                for (int a = 0; a < NUM_ATTRS; a++) {
                    String v = row[a + 1];
                    int vi = valueIndex(v);
                    counts.get(cls)[a][vi]++;
                }
            }

            int correct = 0;
            for (String[] row : test) {
                String trueClass = row[0];
                String best = null;
                double bestScore = Double.NEGATIVE_INFINITY;
                int total = train.size();
                for (String cls : classSet) {
                    int cc = classCount.get(cls);
                    double score = Math.log((double) cc / total);
                    for (int a = 0; a < NUM_ATTRS; a++) {
                        int vi = valueIndex(row[a + 1]);
                        double p = (counts.get(cls)[a][vi] + 1.0) / (cc + K);
                        score += Math.log(p);
                    }
                    if (score > bestScore) { bestScore = score; best = cls; }
                }
                if (best != null && best.equals(trueClass)) correct++;
            }

            double acc = (double) correct / test.size();
            totalAcc += acc;
            System.out.printf("Fold %2d: accuracy = %.4f (%d/%d correct)%n",
                    fold + 1, acc, correct, test.size());
            start = end;
        }
        System.out.printf("Mean accuracy: %.4f%n", totalAcc / numFolds);
    }

    static int valueIndex(String v) {
        switch (v) {
            case "y": return 0;
            case "n": return 1;
            default:  return 2;
        }
    }
}

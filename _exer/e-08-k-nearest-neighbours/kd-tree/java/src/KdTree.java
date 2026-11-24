import java.util.*;

public class KdTree {

    static class Node {
        double[] point;
        int index;
        Node left, right;

        Node(double[] point, int index) {
            this.point = point;
            this.index = index;
        }
    }

    static Node buildTree(double[][] points, int[] indices, int depth) {
        if (indices.length == 0) return null;

        int axis = depth % 2;
        // Sort indices by the splitting axis
        Integer[] boxed = new Integer[indices.length];
        for (int i = 0; i < indices.length; i++) boxed[i] = indices[i];
        Arrays.sort(boxed, (a, b) -> Double.compare(points[a][axis], points[b][axis]));

        int mid = boxed.length / 2;
        Node node = new Node(points[boxed[mid]], boxed[mid]);

        int[] leftIdx = new int[mid];
        int[] rightIdx = new int[boxed.length - mid - 1];
        for (int i = 0; i < mid; i++) leftIdx[i] = boxed[i];
        for (int i = mid + 1; i < boxed.length; i++) rightIdx[i - mid - 1] = boxed[i];

        node.left  = buildTree(points, leftIdx,  depth + 1);
        node.right = buildTree(points, rightIdx, depth + 1);
        return node;
    }

    // best[0] = best index, best[1] reinterpreted as double bits = best dist
    static double bestDist;
    static int    bestIdx;

    static void nearest(Node node, double[] query, int depth) {
        if (node == null) return;

        double dx = node.point[0] - query[0];
        double dy = node.point[1] - query[1];
        double d  = Math.sqrt(dx * dx + dy * dy);

        if (d < bestDist || (d == bestDist && node.index < bestIdx)) {
            bestDist = d;
            bestIdx  = node.index;
        }

        int axis = depth % 2;
        double diff = query[axis] - node.point[axis];

        Node near = diff <= 0 ? node.left  : node.right;
        Node far  = diff <= 0 ? node.right : node.left;

        nearest(near, query, depth + 1);

        if (Math.abs(diff) < bestDist) {
            nearest(far, query, depth + 1);
        }
    }

    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);

        int n = sc.nextInt();
        double[][] points = new double[n][2];
        int[] indices = new int[n];
        for (int i = 0; i < n; i++) {
            points[i][0] = sc.nextDouble();
            points[i][1] = sc.nextDouble();
            indices[i] = i;
        }

        Node root = buildTree(points, indices, 0);

        int q = sc.nextInt();
        for (int i = 0; i < q; i++) {
            double qx = sc.nextDouble();
            double qy = sc.nextDouble();

            bestDist = Double.MAX_VALUE;
            bestIdx  = -1;
            nearest(root, new double[]{qx, qy}, 0);

            System.out.printf("Nearest to (%.2f, %.2f): point %d (%.2f, %.2f) dist=%.4f%n",
                qx, qy, bestIdx, points[bestIdx][0], points[bestIdx][1], bestDist);
        }
    }
}

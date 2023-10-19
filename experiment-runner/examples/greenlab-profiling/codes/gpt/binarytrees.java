//This is GPT generate the same function as binarytrees.java

//input eg. COMMAND LINE: javac binarytrees.java
//COMMAND LINE: java binarytrees 21 

public class binarytrees {
    public static void main(String[] args) {
        int n = Integer.parseInt(args[0]);

        // Create the stretch tree of depth n+1
        Node stretchTree = createTree(n + 1);
        int stretchCheck = computeTreeChecksum(stretchTree);
        System.out.printf("stretch tree of depth %d\t check: %d%n", n + 1, stretchCheck);

        // Create a long-lived tree of depth n
        Node longLivedTree = createTree(n);

        for (int depth = 4; depth <= n; depth += 2) {
            int iterations = 1 << (n - depth + 4);

            int check = 0;

            for (int i = 1; i <= iterations; i++) {
                Node tempTree1 = createTree(depth);
                Node tempTree2 = createTree(depth);
                check += computeTreeChecksum(tempTree1) + computeTreeChecksum(tempTree2);
            }

            System.out.printf("%d\t trees of depth %d\t check: %d%n", iterations * 2, depth, check);
        }

        int longLivedCheck = computeTreeChecksum(longLivedTree);
        System.out.printf("long lived tree of depth %d\t check: %d%n", n, longLivedCheck);
    }

    static class Node {
        Node left, right;
    }

    static Node createTree(int depth) {
        if (depth == 0) {
            return null;
        }
        Node node = new Node();
        node.left = createTree(depth - 1);
        node.right = createTree(depth - 1);
        return node;
    }

    static int computeTreeChecksum(Node node) {
        if (node == null) {
            return 1;
        }
        return 1 + computeTreeChecksum(node.left) + computeTreeChecksum(node.right);
    }
}

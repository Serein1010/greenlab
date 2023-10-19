//This is GPT generate the same function as fasta.java
//input eg. COMMAND LINE: javac DNASequenceGenerator.java
// COMMAND LINE: java DNASequenceGenerator 5000 

import java.util.Random;

public class fasta {
    private static final int IM = 139968;
    private static final int IA = 3877;
    private static final int IC = 29573;
    private static int Seed = 42;
    private static final int LineLength = 60;

    static String ALU =
            "GGCCGGGCGCGGTGGCTCACGCCTGTAATCCCAGCACTTTGG" +
            "GAGGCCGAGGCGGGCGGATCACCTGAGGTCAGGAGTTCGAGA" +
            "CCAGCCTGGCCAACATGGTGAAACCCCGTCTCTACTAAAAAT" +
            "ACAAAAATTAGCCGGGCGTGGTGGCGCGCGCCTGTAATCCCA" +
            "GCTACTCGGGAGGCTGAGGCAGGAGAATCGCTTGAACCCGGG" +
            "AGGCGGAGGTTGCAGTGAGCCGAGATCGCGCCACTGCACTCC" +
            "AGCCTGGGCGACAGAGCGAGACTCCGTCTCAAAAA";

    static class Frequency {
        public final byte c;
        public double p;

        public Frequency(char c, double p) {
            this.c = (byte) c;
            this.p = p * IM;
        }
    }

    static Frequency[] IUB = {
            new Frequency('a', 0.27),
            new Frequency('c', 0.12),
            new Frequency('g', 0.12),
            new Frequency('t', 0.27),

            new Frequency('B', 0.02),
            new Frequency('D', 0.02),
            new Frequency('H', 0.02),
            new Frequency('K', 0.02),
            new Frequency('M', 0.02),
            new Frequency('N', 0.02),
            new Frequency('R', 0.02),
            new Frequency('S', 0.02),
            new Frequency('V', 0.02),
            new Frequency('W', 0.02),
            new Frequency('Y', 0.02)
    };

    static Frequency[] HomoSapiens = {
            new Frequency('a', 0.3029549426680),
            new Frequency('c', 0.1979883004921),
            new Frequency('g', 0.1975473066391),
            new Frequency('t', 0.3015094502008)
    };

    static double Random() {
        Seed = (Seed * IA + IC) % IM;
        return (double) Seed / IM;
    }

    static String GenerateRepeatSequence(String pattern, int length) {
        StringBuilder sequence = new StringBuilder(length);

        while (sequence.length() < length) {
            sequence.append(pattern);
        }

        return sequence.toString().substring(0, length);
    }

    static String GenerateIUBAmbiguityCodes(int length) {
        StringBuilder sequence = new StringBuilder(length);

        while (sequence.length() < length) {
            char baseChar = ' ';
            double randomValue = Random() * IM;

            for (Frequency freq : IUB) {
                randomValue -= freq.p;
                if (randomValue < 0) {
                    baseChar = (char) freq.c;
                    break;
                }
            }

            sequence.append(baseChar);
        }

        return sequence.toString();
    }

    static String GenerateHomoSapiensFrequency(int length) {
        StringBuilder sequence = new StringBuilder(length);

        while (sequence.length() < length) {
            char baseChar = ' ';
            double randomValue = Random() * IM;

            for (Frequency freq : HomoSapiens) {
                randomValue -= freq.p;
                if (randomValue < 0) {
                    baseChar = (char) freq.c;
                    break;
                }
            }

            sequence.append(baseChar);
        }

        return sequence.toString();
    }

    static String InsertNewLines(String sequence) {
        int length = sequence.length();
        int outputLength = length + length / LineLength; // Calculate the length of the output string with newline characters
        char[] outputSequence = new char[outputLength];

        int inputIndex = 0;
        int outputIndex = 0;

        while (inputIndex < length) {
            outputSequence[outputIndex] = sequence.charAt(inputIndex);
            inputIndex++;
            outputIndex++;

            if (outputIndex % (LineLength + 1) == 0) {
                outputSequence[outputIndex] = '\n'; // Insert a newline after 60 characters
                outputIndex++;
            }
        }

        return new String(outputSequence);
    }

    public static void main(String[] args) {
        if (args.length != 1 || !args[0].matches("\\d+")) {
            System.out.println("Usage: java DNASequenceGenerator <sequence_length>");
            return;
        }

        int sequenceLength = Integer.parseInt(args[0]);

        // Example 1: Generate a repeat sequence based on ALU
        String aluSequence = GenerateRepeatSequence(ALU, sequenceLength * 2);
        System.out.println(">ONE Homo sapiens alu");
        System.out.println(InsertNewLines(aluSequence));

        // Example 2: Generate a random DNA sequence based on IUB ambiguity codes
        String iubSequence = GenerateIUBAmbiguityCodes(sequenceLength * 3);
        System.out.println(">TWO IUB ambiguity codes");
        System.out.println(InsertNewLines(iubSequence));

        // Example 3: Generate a random DNA sequence based on human DNA frequencies
        String humanFrequencySequence = GenerateHomoSapiensFrequency(sequenceLength * 5);
        System.out.println(">THREE Homo sapiens frequency");
        System.out.println(InsertNewLines(humanFrequencySequence));
    }
}

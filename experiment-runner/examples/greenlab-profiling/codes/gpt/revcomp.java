
//This is GPT generate the same function as revcomp.java

//input eg. COMMAND LINE: javac ReverseComplement.java
//eg. COMMAND LINE: java ReverseComplement revcomp-input.txt output.txt

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;

public class revcomp {
    public static void main(String[] args) {
        if (args.length != 2) {
            System.out.println("Usage: java ReverseComplement input.txt output.txt");
            return;
        }

        String inputFile = args[0];
        String outputFile = args[1];

        try {
            BufferedReader reader = new BufferedReader(new FileReader(inputFile));
            BufferedWriter writer = new BufferedWriter(new FileWriter(outputFile));

            String line;
            while ((line = reader.readLine()) != null) {
                String reverseComplement = computeReverseComplement(line);
                writer.write(reverseComplement);
                writer.newLine();
            }

            reader.close();
            writer.close();

            System.out.println("Reverse complement completed.");
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static String computeReverseComplement(String sequence) {
        // Your reverse complement logic goes here.
        // Implement the mapping of DNA bases to their complements.
        // Example: A -> T, C -> G, G -> C, T -> A.

        StringBuilder result = new StringBuilder();
        for (int i = sequence.length() - 1; i >= 0; i--) {
            char base = sequence.charAt(i);
            char complement = getComplement(base);
            result.append(complement);
        }

        return result.toString();
    }

    public static char getComplement(char base) {
        switch (base) {
            case 'A':
                return 'T';
            case 'T':
                return 'A';
            case 'C':
                return 'G';
            case 'G':
                return 'C';
            case 'M':
                return 'K';
            case 'R':
                return 'Y';
            case 'W':
                return 'W';
            case 'S':
                return 'S';
            case 'Y':
                return 'R';
            case 'K':
                return 'M';
            case 'V':
                return 'B';
            case 'H':
                return 'D';
            case 'D':
                return 'H';
            case 'B':
                return 'V';
            default:
                return base;
        }
    }
}


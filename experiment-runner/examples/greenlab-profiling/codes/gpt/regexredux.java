//This is GPT generate the same function as regexredux.java

//input eg. COMMAND LINE: javac FastaManipulator.java
//COMMAND LINE: java FastaManipulator regexredux-input.txt 

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class regexredux {
    public static void main(String[] args) throws IOException {
        if (args.length != 1) {
            System.err.println("Usage: java RegexRedux <input_file>");
            System.exit(1);
        }

        String inputFile = args[0];

        // Read FASTA format data from the input file
        StringBuilder fastaData = new StringBuilder();
        try (BufferedReader reader = new BufferedReader(new FileReader(inputFile))) {
            String line;
            while ((line = reader.readLine()) != null) {
                if (!line.startsWith(">")) {
                    fastaData.append(line);
                }
            }
        } catch (IOException e) {
            System.err.println("Error reading input file: " + e.getMessage());
            System.exit(1);
        }

        // Calculate and print the original sequence length
        int originalLength = fastaData.length();
        System.out.println("Original Sequence Length: " + originalLength);

        // Define the regular expressions and patterns
        String[] regexPatterns = {
            "agggtaaa|tttaccct",
            "[cgt]gggtaaa|tttaccc[acg]",
            "a[act]ggtaaa|tttacc[agt]t",
            "ag[act]gtaaa|tttac[agt]ct",
            "agg[act]taaa|ttta[agt]cct",
            "aggg[acg]aaa|ttt[cgt]ccct",
            "agggt[cgt]aa|tt[acg]accct",
            "agggta[cgt]a|t[acg]taccct",
            "agggtaa[cgt]|[acg]ttaccct"
        };

        // Count matches for each pattern and print the results
        for (String patternString : regexPatterns) {
            Pattern pattern = Pattern.compile(patternString);
            Matcher matcher = pattern.matcher(fastaData.toString());
            int count = 0;
            while (matcher.find()) {
                count++;
            }
            System.out.println("Pattern: " + patternString + " Count: " + count);
        }

        // Apply magic regex patterns
        String[] magicRegexPatterns = {
            "tHa[Nt]",
            "aND|caN|Ha[DS]|WaS",
            "a[NSt]|BY",
            "<[^>]*>",
            "\\|[^|][^|]*\\|"
        };

        for (String patternString : magicRegexPatterns) {
            fastaData = new StringBuilder(fastaData.toString().replaceAll(patternString, ""));
        }

        // Calculate and print the modified sequence length
        int modifiedLength = fastaData.length();
        System.out.println("Modified Sequence Length: " + modifiedLength);

        // Print the sequence length after applying all patterns
        System.out.println("Final Sequence Length: " + modifiedLength);
    }
}

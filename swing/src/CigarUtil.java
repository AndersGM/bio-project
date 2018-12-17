import framework.Lighter;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class CigarUtil {
    public static void highlightFromCigar(String cigar, Lighter lighter, int start) {
        Pattern cigarPattern = Pattern.compile("[\\d]+[a-zA-Z]");

        int curpos = start - 1;

        Matcher matcher = cigarPattern.matcher(cigar);
        while (matcher.find()) {
            String[] parts = matcher.group().split("(?<=\\d)(?=\\D)");
            try {
                int i = Integer.parseInt(parts[0]);
                String op = parts[1];

                switch (op.toUpperCase()) {
                    case "M":
                        lighter.match(curpos, curpos += i);
                        break;
                    case "S":
                        lighter.substitution(curpos, curpos += i);
                        break;
                    case "D":
                        lighter.deletion(curpos, curpos += i);
                        break;
                    case "I":
                        lighter.insertion(curpos, curpos += i);
                        break;
                    default:
                        System.out.println("Unsupported character " + op);
                        curpos += i;
                }
            } catch (Exception e) {

            }
        }
    }
}

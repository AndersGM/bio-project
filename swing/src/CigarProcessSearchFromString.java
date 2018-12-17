import framework.Lighter;
import framework.QueryNotFoundException;
import framework.Search;

import javax.swing.*;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.List;

public class CigarProcessSearchFromString implements Search {
    private String path = "/home/anders/Projects/bio-project/bwt/bwt";
    private JTextArea textArea;
    private Lighter lighter;

    public CigarProcessSearchFromString(JTextArea textArea) {
        this.textArea = textArea;
    }

    @Override
    public List<String> search(String query) throws QueryNotFoundException {
        try {
            return invokeProcess(query);
        } catch (IOException e) {
            e.printStackTrace();
            return new ArrayList<>();
        }
    }

    private List<String> invokeProcess(String query) throws IOException, QueryNotFoundException {
        Process process = new ProcessBuilder(path, "-f", "hejsa", "-q", query, "-d", "2").start();
        InputStream is = process.getInputStream();

        InputStreamReader isr = new InputStreamReader(is);
        BufferedReader br = new BufferedReader(isr);
        String line;

        List<String> list = new ArrayList<>();

        while ((line = br.readLine()) != null) {
            System.out.println(line);

            if (line.isEmpty()) {
                System.out.println("skipping empty");
                continue;
            }

            list.add(line);
        }

        try {
            process.waitFor();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        if (process.exitValue() == 1) {
            // to few arguments
            System.out.println("to few arguments");
        }

        if (process.exitValue() == 2) {
            throw new QueryNotFoundException();
        }

        return list;
    }
}

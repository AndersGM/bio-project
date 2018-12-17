import framework.QueryNotFoundException;
import framework.Search;

import javax.swing.*;
import java.awt.*;
import java.io.*;
import java.util.ArrayList;
import java.util.List;

public class ProcessApproximateSearchFromFile implements Search {
    private String path = "/home/anders/Projects/bio-project/bwt/bwt";
    private String fileName;
    private Integer d = 1;
    private Component frame;


    public ProcessApproximateSearchFromFile(String fileName, Component frame) {
        this.fileName = fileName;
        this.frame = frame;
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
        var list = new ArrayList<String>();
        if (query.length() <= d) {
            if (JOptionPane.showConfirmDialog(frame, "You are search for a query with length less than or equal edit distance. Proceed?") != 0) {
                return list;
            }
        }

        var p = new ProcessBuilder(path, "-f", fileName, "-q", query, "-d", d.toString(), "-s", "16", "-r", "dummy", "-a", "abcdefghijklmnop");

        Process process = p.start();
        InputStream is = process.getInputStream();


        InputStreamReader isr = new InputStreamReader(is);
        BufferedReader br = new BufferedReader(isr);
        String line;

        while ((line = br.readLine()) != null) {
            if (line.isEmpty()) {
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
            System.out.println("too few arguments");
        }

        if (process.exitValue() == 2) {
            throw new QueryNotFoundException();
        }

        System.out.println(list.size());
        return list;
    }
}

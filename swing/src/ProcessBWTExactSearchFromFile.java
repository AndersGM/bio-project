import framework.QueryNotFoundException;
import framework.Search;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.List;

public class ProcessBWTExactSearchFromFile implements Search {
    private String path = "../bwt/bwt";
    private String fileName;


    public ProcessBWTExactSearchFromFile(String fileName) {
        this.fileName = fileName;
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
        var p = new ProcessBuilder(path, "-f", fileName, "-q", query, "-s", "16", "-r", "dummy", "-a", "abcdefghijklmnop");

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
        return list;
    }
}

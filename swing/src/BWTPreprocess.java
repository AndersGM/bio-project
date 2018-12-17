import framework.PreProcess;

import java.io.File;
import java.io.IOException;

public class BWTPreprocess implements PreProcess {
    private String fileName;

    public BWTPreprocess(String fileName) {
        this.fileName = fileName;
    }

    @Override
    public void preprocess() throws InterruptedException, IOException {
        fileName = fileName.replace(".x", "");

        File file = new File(fileName + ".o");

        if (!file.exists()) {
            Runtime runtime = Runtime.getRuntime();
            runtime.exec("/home/anders/Projects/bio-project/bwt/mksary " + fileName + ".x" + " " + fileName + ".sa").waitFor();//suffix array
            Process p = runtime.exec("/home/anders/Projects/bio-project/bwt/bwt -f " + fileName + " -s 16 -b dummy -a abcdefghijklmnop");
            p.waitFor();
        }
    }
}

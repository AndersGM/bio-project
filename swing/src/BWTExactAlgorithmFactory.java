import framework.AlgorithmFactory;
import framework.PreProcess;
import framework.Search;
import framework.TextPresenter;

import javax.swing.*;
import java.awt.*;

public class BWTExactAlgorithmFactory implements AlgorithmFactory {
    private JTextArea textArea;
    String fileName;

    public BWTExactAlgorithmFactory(JTextArea textArea, String fileName) {
        this.textArea = textArea;
        this.fileName = fileName;
    }

    @Override
    public TextPresenter createTextPresenter() {
        return new FileTextPresenter(textArea, fileName);
    }

    @Override
    public Search createSearch() {
        return new ProcessBWTExactSearchFromFile(fileName);
    }

    @Override
    public PreProcess createPipeline() {
        return new BWTPreprocess(fileName);
    }
}

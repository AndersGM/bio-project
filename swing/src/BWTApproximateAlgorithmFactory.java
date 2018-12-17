import framework.Search;

import javax.swing.*;
import java.awt.*;

public class BWTApproximateAlgorithmFactory extends BWTExactAlgorithmFactory {
    private final Component frame;

    public BWTApproximateAlgorithmFactory(JTextArea textArea, String fileName, Component frame) {
        super(textArea, fileName);
        this.frame=frame;
    }

    @Override
    public Search createSearch() {
        return new ProcessApproximateSearchFromFile(fileName,frame);
    }
}

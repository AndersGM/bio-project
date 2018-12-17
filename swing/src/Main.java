import framework.*;

import javax.swing.*;
import java.awt.*;
import java.io.IOException;
import java.util.Comparator;

// TODO settings dialog of configs
// TODO debug log, to show running time
// TODO overview of search results, and filter by match,
// TODO search button should be alligned with field
// TODO allow search by pressing enter
// TODO should be possible for the search implementation to add extra fields and to limit input characters
// TODO make it possible to save a file in a directory (i.e. project directory) with project settings
// TODO make button to step through
// TODO should also support SAM
// TODO compare one file to another, i.e. see where your algorithm differs from my
// load buffer s.t. it no starts immediatly at the beginning of the matching
// make field to
// write results to file, to prevent heap overflow
public class Main {
    public static void main(String[] args) {
        String fileName = "exp";

        JFrame frame = new JFrame("Cigar Watch");
        JTextArea textArea = new JTextArea(10, 30);
        // choose which algorithm to use

        AlgorithmFactory factory = new BWTApproximateAlgorithmFactory(textArea, fileName, frame);

        PreProcess pipeline = factory.createPipeline();
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        JTextField searchField = new JTextField("");
        searchField.setMaximumSize(new Dimension(1920, 25));
        searchField.setPreferredSize(new Dimension(100, 25));
        JButton searchButton = new JButton("Search");
        var listModel = new DefaultListModel<String>();
        textArea.setAutoscrolls(true);
        textArea.setLineWrap(true);
        TextPresenter presenter = factory.createTextPresenter();
        Lighter lighter = new Lighter(textArea.getHighlighter(), presenter);
        Search search = factory.createSearch();

        CigarInterpreter interpreter = new CigarInterpreter(lighter, presenter);
        searchButton.addActionListener(l -> {
            final String query = searchField.getText();
            try {
                if (query == null || query.length() == 0) {
                    JOptionPane.showMessageDialog(frame, "No query entered");
                    return;
                }

                // clear all highlights
                lighter.clear();
                listModel.clear();
                var result = search.search(query);
                result.sort(Comparator.comparingInt(s -> Integer.parseInt(s.split(" ")[0])));
                listModel.addAll(result);
            } catch (QueryNotFoundException e) {
                JOptionPane.showMessageDialog(frame, query + " was not found");
            }
        });

        JPanel panel = new JPanel();
        panel.setLayout(new BoxLayout(panel, BoxLayout.Y_AXIS));
        panel.add(searchField);
        panel.add(searchButton);

        // this should contain split pane with list and text area

        var lister = new JList<>(listModel);
        JScrollPane listScroll = new JScrollPane(lister);
        lister.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
        lister.addListSelectionListener(e -> {
            if (!e.getValueIsAdjusting() && lister.getSelectedIndex() > -1) {
                var s = listModel.get(lister.getSelectedIndex());
                lighter.clear();
                interpreter.highlight(s);
            }
        });

        lister.setVisibleRowCount(5);
        panel.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));

        var splitPane = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT, listScroll, new JScrollPane(textArea));

        panel.add(splitPane);

        frame.getContentPane().add(panel);
        frame.pack();


        frame.setVisible(true);
        frame.setEnabled(false);
        // preprocess
        try {
            pipeline.preprocess();
        } catch (InterruptedException | IOException e) {
            e.printStackTrace();
        }

        frame.setEnabled(true);
    }
}
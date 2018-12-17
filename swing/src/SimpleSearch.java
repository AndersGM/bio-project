import framework.Lighter;
import framework.QueryNotFoundException;

import javax.swing.*;
import javax.swing.text.BadLocationException;

public class SimpleSearch  {
    private JTextArea textArea;

    public SimpleSearch(JTextArea textArea) {
        this.textArea = textArea;
    }

    public void search(Lighter lighter, String query) throws QueryNotFoundException {
        int i = textArea.getText().indexOf(query);
        try {
            lighter.match(i, i + query.length());
        } catch (BadLocationException e) {
            throw new QueryNotFoundException();
        }
    }
}

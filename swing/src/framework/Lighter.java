package framework;

import javax.swing.text.BadLocationException;
import javax.swing.text.DefaultHighlighter;
import javax.swing.text.Highlighter;
import java.awt.*;
import java.util.Stack;

import static java.lang.Math.toIntExact;

public class Lighter {
    private final Highlighter highlighter;
    private TextPresenter presenter;
    // this is used to remember where additional symbols have been inserted, such that they can be removed again
    private Stack<Integer> indices = new Stack<>();

    // remember last highligt, as it will be removed when highlighting insertions because jtextarea would extend the highlight
    private Object lastHighlight = null;
    private Color lastColor = null;
    private int lastStart, lastStop;

    public Lighter(Highlighter highlighter, TextPresenter presenter) {
        this.highlighter = highlighter;
        this.presenter = presenter;
    }

    public void match(int start, int end) throws BadLocationException {
        lastColor = Color.lightGray;
        lastHighlight = highlighter.addHighlight(toIntExact(start), toIntExact(end), new DefaultHighlighter.DefaultHighlightPainter(lastColor));
        lastStart = start;
        lastStop = end;
    }

    public void deletion(int start, int end) throws BadLocationException {
        lastColor = Color.red;
        lastHighlight = highlighter.addHighlight(start, end, new DefaultHighlighter.DefaultHighlightPainter(lastColor));
        lastStart = start;
        lastStop = end;
    }

    public void insertion(int start, int end) throws BadLocationException {
        if (lastHighlight != null) {
            highlighter.removeHighlight(lastHighlight);
        }

        String x = "";
        for (int i = start; i < end; i++) {
            indices.push(i);
            x = x + "x";
        }

        presenter.insertTextAt(start, x);

        if (lastHighlight != null) {
            highlighter.addHighlight(lastStart, lastStop, new DefaultHighlighter.DefaultHighlightPainter(lastColor));
        }
        lastColor = Color.green;
        lastHighlight = highlighter.addHighlight(toIntExact(start), toIntExact(end), new DefaultHighlighter.DefaultHighlightPainter(lastColor));
        lastStart = start;
        lastStop = end;
    }

    public void clear() {
        highlighter.removeAllHighlights();
        while (!indices.empty()) {
            presenter.removeTextAt(indices.pop());
        }

        lastHighlight = null;
    }

    public void substitution(int start, int end) throws BadLocationException {
        lastColor = Color.pink;
        lastHighlight = highlighter.addHighlight(start, end, new DefaultHighlighter.DefaultHighlightPainter(lastColor));
        lastStart = start;
        lastStop = end;
    }
}

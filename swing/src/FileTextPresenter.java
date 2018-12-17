import framework.BufferToSmallException;
import framework.TextPresenter;

import javax.swing.*;
import java.io.IOException;
import java.io.RandomAccessFile;
import java.nio.charset.Charset;
import java.nio.file.Paths;

import static java.lang.Long.max;

public class FileTextPresenter implements TextPresenter {
    private JTextArea textArea;
    private long offset = 0;
    private RandomAccessFile file;
    private int bufferSize = 10000;

    private String readFileFromOffset()
            throws IOException {

        byte[] encoded = new byte[bufferSize];

        file.seek(offset);
        file.read(encoded, 0, bufferSize);
        return new String(encoded, Charset.defaultCharset());
    }

    public FileTextPresenter(JTextArea textArea, String fileName) {
        this.textArea = textArea;
        textArea.setEditable(false);
        try {
            file = new RandomAccessFile(Paths.get(fileName + ".x").toFile(), "r");
            updateTextField();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private void updateTextField() throws IOException {
        textArea.setText(readFileFromOffset());
    }

    /**
     * assure that the given contents are in the text area
     *
     * @param start
     * @param length
     */
    public void assurePresenceOf(int start, int length) throws IOException, BufferToSmallException {
        // read text into

        if (length > bufferSize) {
            // we got a problem
            throw new BufferToSmallException();
        }

        if (start >= offset && start + length <= offset + bufferSize) {
            return;
        }

        if (start < offset) {
            offset = start - 1;
        } else if (start + length > offset + bufferSize) {
            offset = start - 1;
        }

        offset = max(offset, 0);

        updateTextField();
    }

    /**
     * updates cursor position relative to offset
     *
     * @param position
     */
    public void setCaret(int position) {
        textArea.setCaretPosition((int) (position - offset));
    }

    public long getOffset() {
        return offset;
    }

    @Override
    public void insertTextAt(int position, String text) {
        textArea.insert(text, position);
    }

    @Override
    public void removeTextAt(int position) {
        textArea.replaceRange("", position, position + 1);
    }
}

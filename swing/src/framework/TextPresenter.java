package framework;

import java.io.IOException;

/**
 * responsible for presenting the file contents and assuring that the right text is present
 */
public interface TextPresenter {
    void assurePresenceOf(int start, int length) throws IOException, BufferToSmallException;

    void setCaret(int position);

    long getOffset();

    void insertTextAt(int position, final String text);

    void removeTextAt(int position);
}

import framework.BufferToSmallException;
import framework.Lighter;
import framework.ResultInterpreter;
import framework.TextPresenter;

import java.io.IOException;

import static java.lang.Math.toIntExact;

public class CigarInterpreter implements ResultInterpreter {
    private Lighter lighter;
    private TextPresenter presenter;

    public CigarInterpreter(Lighter lighter, TextPresenter presenter) {
        this.lighter = lighter;
        this.presenter = presenter;
    }

    @Override
    public void highlight(final String query) {
        String[] s = query.split(" ");
        int start = Integer.parseInt(s[0]) + 1;
        // start should be assured in editor
        try {
            presenter.assurePresenceOf(start, query.length());
            presenter.setCaret(start);
            CigarUtil.highlightFromCigar(query, lighter, toIntExact(start - presenter.getOffset()));
        } catch (IOException | BufferToSmallException e) {
            e.printStackTrace();
        }
    }
}

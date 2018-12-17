package framework;

import java.util.List;

/**
 * perform a search and returns strings with matches
 */
public interface Search {
    List<String> search(String query) throws QueryNotFoundException;
}

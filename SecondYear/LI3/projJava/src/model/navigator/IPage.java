package model.navigator;

import java.util.regex.PatternSyntaxException;

public interface IPage {
    /**
     * Turns the page in a string
     */
    String toString();

    /**
     * Looks in the page for string provided
     * @param string
     * @return how many hits
     */
    int contains(String string) throws PatternSyntaxException;

    /**
     * Removes matches that happened
     */
    void clearHits();
}

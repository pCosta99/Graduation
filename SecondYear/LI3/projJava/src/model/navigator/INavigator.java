package model.navigator;

public interface INavigator {
    /**
     * Sets the help message
     * @param message
     */
    void setHelpMessage(String message);

    /**
     * Sets the default message
     * @param message
     */
    void setDefaultMessage(String message);

    /**
     * Activates the status bar
     */
    void triggerStatusBar();

    /**
     * Calls the help message
     */
    void callHelpMessage();

    /**
     * Asks the navigator to move into the next page internally
     */
    void nextPage();

    /**
     * Asks the navigator to move into the previous page internally
     */
    void previousPage();

    /**
     * Gives the current page
     */
    String toString();

    /**
     * Sets the current page as a certain page
     * @param page
     */
    void getPage(int page);

    /**
     * Finds a certain string
     * @param string
     */
    void find(String string);

    /**
     * Looks for the next occurrence of a previous lookup
     */
    void nextOcurrence();

    /**
     * Looks for the previous occurrence of a previous lookup
     */
    void previousOcurrence();
}

package model.navigator;

public interface IStatusBar {
    /**
     * Sets the current page that we are on
     * @param currentPage
     */
    void setCurrentPage(int currentPage);

    /**
     * Adds a new message associated with a type
     * @param type
     * @param message
     */
    void addMessage(String type, String message);

    /**
     * Sets the current message type to be displayed
     * @param type
     */
    void setMessageType(String type);

    /**
     * Stringifies the bar
     */
    String toString();
}

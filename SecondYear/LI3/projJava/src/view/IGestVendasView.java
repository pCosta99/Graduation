package view;

import model.base.ICustomer;
import model.base.IProduct;
import org.apache.commons.lang3.tuple.ImmutablePair;
import org.javatuples.Pair;
import org.javatuples.Triplet;

import java.util.Collection;
import java.util.List;

public interface IGestVendasView {
    /**
     * Displays a collection of products
     * @param products The products we want to print
     */
    void printProductCollection(Collection<IProduct> products);

    /**
     * Displays a table of data per each branch
     * @param lists The data we want to print in a list (each entry is a branch)
     */
    void perBranchTable(List<List<Pair<ICustomer, Float>>> lists);

    /**
     * Displays a table with monthly information about a list of triplets
     * @param list The data we want to display
     * @param headers Headers of the table
     */
    void monthly_triplet_list_view_with_header(List<Triplet<Integer, Integer, Float>> list, String[] headers);

    /**
     * Displays a table with information about a list of triplets
     * @param list The data we want to display
     * @param headers Headers of the table
     */
    void triplet_list_view_with_header(List<Triplet<Object, Object, Object>> list, String[] headers);

    /**
     * Displays a table with information about a list of pairs
     * @param list The data we want to display
     * @param headers Headers of the table
     */
    void pair_list_view_with_header(List<Pair<Object, Object>> list, String[] headers);

    /**
     * Displays info in a table per branch
     * @param list Contains the data we want to display in each branch
     */
    void displayPerBranchInfo(List<ImmutablePair<Integer, Integer>> list);

    /**
     * Displays info in a table per month
     * @param list The info that tells us what happened in each month
     * @param identifier Identifier that explains what we're showing
     */
    void displayPerMonthInfo(List<Object> list, Object identifier);

    /**
     * Displays info in a table
     * @param data Data to be displayed
     * @param identifiers Headers that identify each column
     */
    void displayPerBranchAndPerMonthInfo(Object[][] data, Object[] identifiers);

    /**
     * Display two columns of data
     * @param identifiers Identifiers tell what each line shows
     * @param data Nothing to explain here
     */
    void displayColumns(String[] identifiers, Object[] data);

    /**
     * Clears the screen
     */
    void clearScreen();

    /**
     * Displays an error
     * @param error The error to display
     */
    void displayError(String error);

    /**
     * Calls the first menu form
     */
    void firstMenuForm();

    /**
     * Calls the main menu form
     */
    void menuForm() throws NumberFormatException;

    /**
     * Screens some info for the user
     * @param string
     */
    void screenInfo(String string);
}

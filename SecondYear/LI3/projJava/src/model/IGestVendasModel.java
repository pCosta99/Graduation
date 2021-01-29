package model;

import model.base.ICustomer;
import model.base.IProduct;
import model.base.ISale;
import model.branches.ICustomerBought;
import model.configs_n_stats.IConfigs;
import model.configs_n_stats.IStatistics;
import org.apache.commons.lang3.tuple.ImmutablePair;
import org.javatuples.Pair;
import org.javatuples.Triplet;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.Serializable;
import java.util.List;
import java.util.Map;

public interface IGestVendasModel extends Serializable {
    /**
     *
     * @return all products not bought
     */
    List<IProduct> allNotBoughtProducts();

    /**
     *
     * @param month
     * @return returns a pair with the distinct customers and the number of sales in a certain month per branch
     */
    List<ImmutablePair<Integer,Integer>> globalSalesOnMonthInBranch(int month);

    /**
     *
     * @param customer
     * @return total purchases, the total products bought and the total spent of a customer, per branch
     */
    List<Triplet<Integer, Integer, Float>> infoAboutCustomer(String customer);

    /**
     *
     * @param product
     * @return total purchases, the total products bought and the total spent of a product, per branch
     */
    List<Triplet<Integer, Integer, Float>> productTotalMadeAndTotalCustomers(String product);

    /**
     *
     * @param customer
     * @return mapping between products and their info, all that a customer bought
     */
    Map<IProduct, ICustomerBought> productsBoughtByCustomer(String customer);

    /**
     * For each product, indicate who were the distinct customers who bought it and how many sales it had
     * @return mapping between product and pairs with the above info
     */
    Map<IProduct, Pair<Integer, Integer>> salesAndDistinctCustomersPerProduct();

    /**
     *
     * @return mapping between customers and their total spent
     */
    List<Map<ICustomer, Float>> totalSpentByCustomers();

    /**
     *
     * @return mapping between customers and how many different products they have bought
     */
    Map<ICustomer, Integer> differentProductsBoughtByCustomers();

    /**
     *
     * @param product
     * @return mapping between each customer and how much and how many of a product they bought
     */
    Map<ICustomer, Pair<Integer, Float>> buyersOfCertainProduct(String product);

    /**
     *
     * @return mapping between a product and it's billing divided by month and branchs
     */
    Map<IProduct, Float[][]> allProductsBilling();

    /**
     *
     * @return number of customers in the customer catalog
     */
    int number_of_customers();

    /**
     * Increments the total amount of sales by 1
     */
    void incrementTotal_sales();

    /**
     * Increments the valid sales by 1
     */
    void incrementValid_sales();

    /**
     *
     * @return Copy of the statistics for this GestVendas
     */
    IStatistics getStatistics();

    /**
     * Indicates if a certain product exists in the product catalog
     * @param product
     * @return boolean
     */
    boolean existsInProductCatalog(String product);

    /**
     * Indicates if a certain product exists in the product catalog
     * @param customer
     * @return boolean
     */
    boolean existsInCustomerCatalog(String customer);

    /**
     * Adds a product to the product catalog
     * @param product
     */
    void addToProductCatalog(String product);

    /**
     * Adds a customer to the customer catalog
     * @param customer
     */
    void addToCustomerCatalog(String customer);

    /**
     * Adds a customer with no products bought to each branch modules
     * @param customer
     */
    void addEmptyCustomerToAllBranches(String customer);

    /**
     * Bills a product
     * @param sale
     */
    void billProduct(ISale sale);

    /**
     * Adds the info of a sale to the info stocked in each branch module for the correct customer
     * @param s
     */
    void customerBoughtInBranch(ISale s);

    /**
     * Marks the GestVendas as initialized
     */
    void setInitialized();

    /**
     * Indicates if GestVendas is initialized
     * @return
     */
    boolean isInitialized();

    /**
     * Triggers save to file query mode
     */
    void setSaveToFile();

    /**
     * Indicates if query save mode is on
     * @return boolean
     */
    boolean saveToFile();

    /**
     * Sets the name of the sales file used
     * @param sales_file_name
     */
    void setSales_file_name(String sales_file_name);

    /**
     * Counts the amount of different products bought
     * @return int
     */
    int countDifferentProductsBought();

    /**
     * Counts the amount of customers who bought something
     * @return int
     */
    int countCustomersWhoBought();

    /**
     * Returns the number of products present in the products catalog
     * @return int
     */
    int number_of_products();

    /**
     * Indicate the total billing by month and by branch paired with the total
     * @return pair
     */
    Pair<Float, List<Float[]>> totalAndBillingByMonthByBranch();

    /**
     * Returns the amount of distinct customers by month
     * @return Array of integer
     */
    List<Integer[]> amountOfDistinctCustomersByMonth();

    /**
     * Indicates the total sales per month
     * @return Array containing the sales per month
     */
    Integer[] totalSalesPerMonth();

    /**
     * Indicates how many zero value sales happened
     * @return int
     */
    int zeroValueSales();


    /**
     * Indicates the total billed
     * @return float
     */
    Float totalBilling();

    /**
     * Saves the object as a object stream in the file path given
     * @param fileName
     * @throws FileNotFoundException
     * @throws IOException
     */
    void saveState(String fileName) throws IOException;

    /**
     * Loads the object stream corresponding to a IGestVendasModel in the file path given
     * @param fileName
     * @return IGestVendasModel
     * @throws IOException
     * @throws ClassNotFoundException
     */
    IGestVendasModel loadState(String fileName) throws IOException, ClassNotFoundException;

    /**
     *
     * @return Copy of configs used in this GestVendas
     */
    IConfigs getConfigs();

    void addEmptyProductToFac(String l);

    void setZeroValueSales(int zero_value_sales);
}

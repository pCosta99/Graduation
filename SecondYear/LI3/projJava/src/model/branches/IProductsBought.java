package model.branches;

import model.base.ICustomer;
import model.base.IProduct;
import model.base.ISale;
import org.javatuples.Pair;
import org.javatuples.Triplet;

import java.io.Serializable;
import java.util.Map;
import java.util.Set;

public interface IProductsBought extends Serializable {
    /**
     * Amount of purchases in a month
     * @param month
     * @return
     */
    int getPurchasesInMonth(int month);

    /**
     * Info about each product bought
     * @return mapping between product and info
     */
    Map<IProduct, ICustomerBought> getProductsBought();

    /**
     * Accumulates a sale in the correct product info
     * @param sale
     */
    void customerBought(ISale sale);

    /**
     * Indicates how many different products were bought
     * @return int
     */
    int differentProductsBought();

    Set<IProduct> products();

    /**
     * Indicates the total spent
     * @return float
     */
    float totalSpent();

    /**
     * Indicates if the customer bought at least one product
     * @return boolean
     */
    boolean didCustomerBuy();

    /**
     * Returns the info about a product bought
     * @param product
     * @return ICustomerBought
     */
    ICustomerBought getCustomerBought(IProduct product);

    /**
     * returns the amount and total spent in a product
     * @param product
     * @return Pair
     */
    Pair<Integer, Float> amountAndTotalSpentOfProduct(IProduct product);

    /**
     * Indicates if the customer bought a certain product
     * @param product
     * @return boolean
     */
    boolean didCustomerBuyProduct(IProduct product);

    /**
     * Accumulates the sales per product and the customers who bought it on the given map
     * @param info
     * @param customer
     */
    void salesPerProduct(Map<IProduct, Pair<Set<ICustomer>, Integer>> info, ICustomer customer);

    /**
     * Accumulates how many customers, the amount purchased and the total profit in the given arrays
     * @param product
     * @param howManyCustomers
     * @param amountPurchased
     * @param totalProfit
     */
    void productTotalMadeAndTotalCustomers(IProduct product, Integer[] howManyCustomers, Integer[] amountPurchased, Float[] totalProfit);

    /**
     * Folds the info stored in this structure into total purchases, total products bought and total spent by month
     * @return Trio
     */
    Triplet<Integer[], Integer[], Float[]> foldInfo();

    /**
     * Accumulates the amount of distinct customers in each month on the given array
     * @param distinctCustomersByMonth
     */
    void amountOfDistinctCustomersByMonth(Integer[] distinctCustomersByMonth);

    /**
     * Indicates if the customer bought anything in the given month
     * @param month
     * @return boolean
     */
    boolean boughtInMonth(int month);

    /**
     * Copy of this structure
     * @return IProductsBought
     */
    IProductsBought clone();

    Set<IProduct> getProductsKeys();
}

package model.branches;

import model.base.ICustomer;
import model.base.IProduct;
import model.base.ISale;
import org.javatuples.Pair;
import org.javatuples.Triplet;

import java.io.Serializable;
import java.util.Map;
import java.util.Set;

public interface IBranch extends Serializable {
    /**
     * Returns the distinct customers in a month
     * @param month
     * @return int
     */
    int getDistinctCustomers(int month);

    /**
     * Indicates if a customer bought in this branch or not
     * @param customer
     * @return boolean
     */
    boolean didCustomerBuy(ICustomer customer);

    /**
     * Clones the info about a customer and the product he bought
     * @param customer
     * @param product
     * @return ICustomerBought
     */
    ICustomerBought getCustomerBought(ICustomer customer, IProduct product);

    /**
     * Returns a set containing the customers who bought at least one product
     * @return Set of ICustomer's
     */
    Set<ICustomer> customersWhoBought();

    /**
     * Returns the amount of distinct customers by month
     * @return Array of integer
     */
    Integer[] amountOfDistinctCustomersByMonth();

    /**
     * Returns the distinct customers that bought in a month
     * @param month
     * @return Set of ICustomer's
     */
    Set<ICustomer> distinctCustomerSetInMonth(int month);

    /**
     * Returns the total purchases, the total products bought and the total spent of a customer, all by month
     * @param customer
     * @return trio of monthly organized arrays
     */
    Triplet<Integer[], Integer[], Float[]> infoAboutCustomer(ICustomer customer);

    /**
     * Returns the total purchases, the total products bought and the total spent of a product, all by month
     * @param product
     * @return trio of monthly organized arrays
     */
    Triplet<Integer[], Integer[], Float[]> productTotalMadeAndTotalCustomers(IProduct product);

    /**
     * Returns the products that a customer bought and their info
     * @param customer
     * @return Mapping between products and info for a customer
     */
    Map<IProduct, ICustomerBought> productsBoughtByCustomer(ICustomer customer);

    /**
     * For each product, indicate who were the distinct customers who bought it and how many sales it had
     * @return mapping between product and pairs with the above info
     */
    Map<IProduct, Pair<Set<ICustomer>, Integer>> salesAndDistinctCustomersPerProduct();

    /**
     *
     * @return mapping between customers and their total spent
     */
    Map<ICustomer, Float> totalSpentByCustomers();

    /**
     *
     * @return mapping between customers and how many different products they bought
     */
    Map<ICustomer, Set<IProduct>> differentProductsBoughtByCustomers();

    /**
     * Mapping between customers and how many and how much have they bought and paid for a certain product
     * @param product
     * @return mapping between customers and the info mentioned above
     */
    Map<ICustomer, Pair<Integer, Float>> buyersOfCertainProduct(IProduct product);

    /**
     *
     * @return Copy of the branch
     */
    IBranch clone();

    /**
     * Adds an empty customer to the branch
     * @param customer
     */
    void addEmptyCustomer(ICustomer customer);

    /**
     * Accumulates the info of a sale on a customer
     * @param s
     */
    void customerBought(ISale s);

    /**
     * Distinct customers by month
     * @return
     */
    int[] getDistinctCustomers();
}

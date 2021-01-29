package model.billing;

import model.base.IProduct;
import model.base.ISale;

import java.io.Serializable;
import java.util.List;
import java.util.Map;

public interface IBilling extends Serializable {
    /**
     * Returns all products that were not bought in a list
     * @return List of IProduct's
     */
    List<IProduct> allNotBoughtProducts();

    /**
     * Counts how many different products were sold
     * @return int
     */
    int countDifferentProductsBought();

    /**
     * Returns the number of sales in a branch and in a month
     * @param branch
     * @param month
     * @return int
     */
    int numberOfSalesInBranchAndMonth(int branch, int month);

    /**
     * Returns the mapping between products and their billing
     * @return Mapping of product and their billing
     */
    Map<IProduct, Float[][]> allProductsBilling();

    void addEmptyProductToFac(IProduct product);

    /**
     * Bills a product in the corresponding month
     * @param sale
     */
    void billProduct(ISale sale);

    /**
     * Indicates if a certain product was sold
     * @param product
     * @return boolean
     */
    boolean wasSold(IProduct product);

    /**
     * Retrieves the info about a product
     * @param product
     * @return ProductInfo
     */
    IProductInfo getProductInfo(IProduct product);

    /**
     * Clones the billing structure
     * @return IBilling
     */
    IBilling clone();

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
     * Indicate the total billing by month and by branch
     * @return List of monthly billings
     */
    List<Float[]> totalBillingByMonthByBranch();

    /**
     * Indicates the total sales per month
     * @return Array containing the sales per month
     */
    Integer[] totalSalesPerMonth();

    /**
     * Returns the number of sales in each month and in each branch
     * @return a[i][j] = sales in branch i month j
     */
    int[][] salesByMonthNBranch();

    /**
     * Returns the total number of sales in a month (across all branches)
     * @param month
     * @return int
     */
    int monthlyTotalOfSales(int month);
}

package model.billing;

import model.base.IProduct;
import model.base.ISale;

import java.io.Serializable;

public interface IProductInfo extends Serializable {

    /**
     *
     * @return number of branches
     */
    int getNumber_of_branches();

    /**
     *
     * @return number of types
     */
    int getNumber_of_types();

    /**
     *
     * @return number of months
     */
    int getNumber_of_months();

    /**
     *
     * @return copy of the product that the info belongs to
     */
    IProduct getProduct();

    /**
     *
     * @return a[t][b][m] -> sales in type t, branch b and month m
     */
    int[][][] getSales();

    /**
     *
     * @return p[t][b][m] -> profit in type t, branch b and month m
     */
    double[][][] getProfit();

    /**
     *
     * @return total amount of sales per type
     */
    int[] getTotal_sales();

    /**
     *
     * @return total amount of sales
     */
    int getNumberOfSales();

    /**
     *
     * @return total profit per type
     */
    double[] getTotal_profit();

    /**
     *
     * @return total profit
     */
    Float getAllProfit();

    /**
     * Indicates if the product was bought any time
     * @return boolean
     */
    boolean wasSold();

    /**
     * Accumulates a sale on the current info
     * @param sale
     */
    void accumulateSaleOnInfo(ISale sale);

    /**
     *
     * @return f[m][b] -> profit in month m and branch b
     */
    Float[][] collectProductInfo();

    /**
     *
     * @param month
     * @param branch
     * @return profit in a branch
     */
    Float getProfitInBranch(int month, int branch);

    /**
     * Copies the IProductInfo
     * @return IProductInfo
     */
    IProductInfo clone();
}

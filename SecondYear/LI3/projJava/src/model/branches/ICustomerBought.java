package model.branches;

import model.base.IProduct;
import model.base.ISale;

import java.io.Serializable;

public interface ICustomerBought extends Serializable {
    /**
     *
     * @return total spent in normal mode per month
     */
    Float[] getTotalGastoNormal();

    /**
     *
     * @return total spent in promotion mode per month
     */
    Float[] getTotalGastoPromocao();

    /**
     *
     * @return total amount of purchases in normal mode per month
     */
    Integer[] getNComprasNormal();

    /**
     *
     * @return total amount of purchases in promotion mode per month
     */
    Integer[] getNComprasPromocao();

    /**
     *
     * @return product bought
     */
    IProduct getProduct();

    /**
     *
     * @return Copy of the structure
     */
    ICustomerBought clone();

    /**
     *
     * @param promocao
     * @return number of purchases in a type
     */
    int getNCompras(boolean promocao);

    /**
     *
     * @return number of purchases across all types
     */
    int getNCompras();

    /**
     *
     * @param month
     * @return number of purchases across all types in a month
     */
    int getNComprasInMonth(int month);

    /**
     *
     * @param month
     * @return total spent in a month across all types
     */
    float getTotalSpentInMonth(int month);

    /**
     * Indicates if the product was bought in a certain month
     * @param month
     * @return boolean
     */
    boolean wasSoldInMonth(int month);

    /**
     *
     * @return total spent across all types
     */
    float getTotalSpent();

    /**
     * Accumulates a sale on the current info
     * @param sale
     */
    void accumulateSaleOnCustomerBought(ISale sale);

    /**
     * Accumulates another structure of the same info on the current one
     * @param cb
     */
    void accumulateCbOnCustomerBought(ICustomerBought cb);
}

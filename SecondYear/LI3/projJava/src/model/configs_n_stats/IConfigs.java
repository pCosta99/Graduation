package model.configs_n_stats;

import java.io.Serializable;

public interface IConfigs extends Serializable {
    /* Getters */

    String getLogPath();

    /**
     *
     * @return length of products
     */
    int getProduct_length();

    /**
     *
     * @return length of product letters
     */
    int getProduct_letters_length();

    /**
     *
     * @return length of product numbers
     */
    int getProduct_numbers_length();

    /**
     *
     * @return products upper limit
     */
    int getProduct_upper_limit();

    /**
     *
     * @return products lower limit
     */
    int getProduct_lower_limit();

    /**
     *
     * @return products path
     */
    String getProductsPath();

    /**
     *
     * @return customer length
     */
    int getCustomer_length();

    /**
     *
     * @return customer letters length
     */
    int getCustomer_letters_length();

    /**
     *
     * @return customer numbers length
     */
    int getCustomer_numbers_length();

    /**
     *
     * @return customers upper limit
     */
    int getCustomer_upper_limit();

    /**
     *
     * @return customers lower limit
     */
    int getCustomer_lower_limit();

    /**
     *
     * @return customers path
     */
    String getCustomersPath();

    /**
     *
     * @return minimum valid price
     */
    double getMin_price();

    /**
     *
     * @return maximum valid price
     */
    double getMax_price();

    /**
     *
     * @return minimum valid amount
     */
    int getMin_amount();

    /**
     *
     * @return maximum valid amount
     */
    int getMax_amount();

    /**
     *
     * @return number of branches
     */
    int getNumber_of_branches();

    /**
     *
     * @return number of months
     */
    int getNumber_of_months();

    /**
     *
     * @return number of types
     */
    int getNumber_of_types();

    /**
     *
     * @return sales path
     */
    String getSalesPath();

    /**
     *
     * @return serialize data path
     */
    String getSerializeDataPath();

    /**
     *
     * @return Copy of this structure
     */
    IConfigs clone();
}

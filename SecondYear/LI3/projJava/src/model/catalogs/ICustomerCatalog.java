package model.catalogs;

import model.base.ICustomer;

import java.io.Serializable;

public interface ICustomerCatalog extends Serializable {
    /**
     * Adds a customer to the catalog
     * @param customer
     */
    void addToCatalog(ICustomer customer);

    /**
     * Indicate if a certain customer exists in the catalog or not
     * @param customer
     * @return boolean
     */
    boolean existsInCatalog(ICustomer customer);

    /**
     * Returns the number of customers in the catalog
     * @return int
     */
    int number_of_customers();

    /**
     *
     * @return Copy of the catalog
     */
    ICustomerCatalog clone();
}

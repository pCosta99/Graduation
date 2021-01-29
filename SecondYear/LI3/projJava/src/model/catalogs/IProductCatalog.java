package model.catalogs;

import model.base.IProduct;

import java.io.Serializable;
import java.util.Map;

public interface IProductCatalog extends Serializable {
    /**
     * Adds a code to the catalog
     * @param code
     */
    void addToCatalog(IProduct code);

    /**
     * Indicate if a certain code exists in the catalog or no
     * @param code
     * @return boolean
     */
    boolean existsInCatalog(IProduct code);

    /**
     *
     * @return Copy of the catalog
     */
    IProductCatalog clone();

    /**
     * Indicates the number of products in the catalog
     * @return int
     */
    int number_of_products();
}

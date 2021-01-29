package model.base;

import java.io.Serializable;

public interface IProduct extends Serializable {
    /**
     * Returns the code of the product
     * @return String
     */
    String getCode();

    /**
     * Sets the code of the product to the one provided
     * @param code
     */
    void setCode(String code);

    /**
     * Clones a product
     * @return ICustomer
     */
    IProduct clone();

    /**
     * Checks if the product equals the object
     * @param o
     * @return boolean
     */
    boolean equals(Object o);

    /**
     * Transforms the product into a int
     * @return int
     */
    int hashCode();
}

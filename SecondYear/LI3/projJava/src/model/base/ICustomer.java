package model.base;

import java.io.Serializable;

public interface ICustomer extends Serializable{
    /**
     * Returns the code of the customer
     * @return String
     */
    String getCode();

    /**
     * Sets the code of the customer to the one provided
     * @param code
     */
    void setCode(String code);

    /**
     * Clones a customer
     * @return ICustomer
     */
    ICustomer clone();

    /**
     * Checks if the customer equals the object
     * @param o
     * @return boolean
     */
    boolean equals(Object o);

    /**
     * Transforms the customer into a int
     * @return int
     */
    int hashCode();
}

package model.base;

import java.io.Serializable;

public interface ISale extends Serializable {
     /**
      * Returns the product of the sale
      * @return IProduct
      */
     IProduct getProduct();

     /**
      * Returns the price of the sale
      * @return float
      */
     float getPrice();

     /**
      * Returns the amount of the sale
      * @return int
      */
     int getAmount();

     /**
      * Returns the type of the sale
      * @return char
      */
     char getType();

     /**
      * Returns the customer of the sale
      * @return ICustomer
      */
     ICustomer getCustomer();

     /**
      * Returns the month of the sale
      * @return int
      */
     int getMonth();

     /**
      * Returns the branch of the sale
      * @return int
      */
     int getFilial();

     /**
      * Transforms the sale into a string
      * @return String
      */
     String toString();
}

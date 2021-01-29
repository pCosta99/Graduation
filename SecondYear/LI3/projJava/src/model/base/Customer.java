package model.base;

import java.io.Serializable;
import java.util.Arrays;

public class Customer implements ICustomer, Comparable, Serializable {
    private String code;

    public Customer(String code){
        this.code = code;
    }

    public Customer(Customer c){
        this.code = c.getCode();
    }

    @Override
    public String getCode() {
        return code;
    }

    @Override
    public void setCode(String code) {
        this.code = code;
    }

    @Override
    public ICustomer clone(){
        return new Customer(this);
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Customer customer = (Customer) o;
        return code.equals(customer.code);
    }

    @Override
    public int hashCode() {
        return Arrays.hashCode(new Object[]{code});
    }

    @Override
    public String toString(){
        return code;
    }
    @Override
    public int compareTo(Object customer) {
        ICustomer customer1 = (ICustomer) customer;
        return this.code.compareTo(customer1.getCode());
    }
}

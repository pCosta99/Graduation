package model.base;

import java.io.Serializable;
import java.util.Arrays;

public class Product implements IProduct, Comparable, Serializable {
    private String code;

    public Product(String code){
        this.code = code;
    }

    public Product(Product p){
        this.code = p.getCode();
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
    public IProduct clone(){
        return new Product(this);
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Product product = (Product) o;
        return code.equals(product.code);
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
    public int compareTo(Object product) {
        IProduct product1 = (IProduct) product;
        return this.code.compareTo(product1.getCode());
    }
}

package model.base;

import java.io.Serializable;

public class Sale implements ISale, Serializable {
    private IProduct product;
    private Float price;
    private int amount;
    private char type;
    private ICustomer customer;
    private int month;
    private int filial;

    /**
     *
     * Parameterized constructor
     * @param sale
     */
    public Sale(String[] sale) {
        this.product = new Product(sale[0]);
        this.price = Float.parseFloat(sale[1]);
        this.amount = Integer.parseInt(sale[2]);
        this.type = sale[3].charAt(0);
        this.customer = new Customer(sale[4]);
        this.month = Integer.parseInt(sale[5]);
        this.filial = Integer.parseInt(sale[6]);
    }

    public IProduct getProduct() {
        return product.clone();
    }

    public float getPrice() {
        return price;
    }

    public int getAmount() {
        return amount;
    }

    public char getType() {
        return type;
    }

    public ICustomer getCustomer() {
        return customer.clone();
    }

    public int getMonth() {
        return month;
    }

    public int getFilial() {
        return filial;
    }

    public String toString(){
        StringBuilder sb = new StringBuilder();
        sb.append(product).append("; ");
        sb.append(customer).append("; ");
        sb.append(price).append("; ");
        sb.append(amount).append("; ");
        sb.append(type).append("; ");
        sb.append(month).append("; ");
        sb.append(filial);
        return sb.toString();
    }
}

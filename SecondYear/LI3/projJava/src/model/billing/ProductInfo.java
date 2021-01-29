package model.billing;

import model.base.IProduct;
import model.base.ISale;

import java.io.Serializable;
import java.util.Arrays;

public class ProductInfo implements IProductInfo, Serializable {
    private IProduct product;
    private int[][][] sales;
    private double[][][] profit;
    private int[] total_sales;
    private double[] total_profit;
    private int number_of_branches;
    private int number_of_types;
    private int number_of_months;

    /**
     * Parametrized constructor
     */
    public ProductInfo(IProduct product, int[][][] sales, double[][][] profit, int[] total_sales, double[] total_profit, int number_of_branches, int number_of_types, int number_of_months){
        this.product = product;
        this.sales = sales;
        this.profit = profit;
        this.total_profit = total_profit;
        this.total_sales = total_sales;
        this.number_of_branches = number_of_branches;
        this.number_of_types = number_of_types;
        this.number_of_months = number_of_months;
    }

    /**
     * Empty constructor
     */
    public ProductInfo(IProduct product, int number_of_branches, int number_of_types, int number_of_months){
        this.product = product;
        this.sales = new int[number_of_types][number_of_branches][number_of_months];
        this.profit = new double[number_of_types][number_of_branches][number_of_months];
        this.total_profit = new double[number_of_types];
        this.total_sales = new int[number_of_types];
        this.number_of_branches = number_of_branches;
        this.number_of_types = number_of_types;
        this.number_of_months = number_of_months;
    }

    /**
     * Copy constructor
     */
    public ProductInfo(ProductInfo pi){
        this.product = pi.getProduct();
        this.sales = pi.getSales();
        this.profit = pi.getProfit();
        this.total_sales = pi.getTotal_sales();
        this.total_profit = pi.getTotal_profit();
        this.number_of_branches = pi.getNumber_of_branches();
        this.number_of_types = pi.getNumber_of_types();
        this.number_of_months = pi.getNumber_of_months();
    }

    @Override
    public int getNumber_of_branches() {
        return number_of_branches;
    }

    @Override
    public int getNumber_of_types() {
        return number_of_types;
    }

    @Override
    public int getNumber_of_months() {
        return number_of_months;
    }

    /**
     * Getters and setters
     */

    @Override
    public IProduct getProduct() {
        return product.clone();
    }

    @Override
    public int[][][] getSales() {
        return sales.clone();
    }

    @Override
    public double[][][] getProfit() {
        return profit.clone();
    }

    @Override
    public int[] getTotal_sales() {
        int[] array = new int[number_of_months];
        for(int t = 0; t < 2; t++)
            for(int i = 0; i < number_of_months; i++)
                for(int j = 0; j < number_of_branches; j++)
                    array[i] += sales[t][j][i];
        return array;
    }

    @Override
    public int getNumberOfSales(){
        return Arrays.stream(total_sales).reduce(0, Integer::sum);
    }

    @Override
    public double[] getTotal_profit() {
        return total_profit;
    }

    @Override
    public Float getAllProfit(){
        return (float) Arrays.stream(total_profit).sum();
    }

    @Override
    public boolean wasSold(){
        return (total_sales[0] + total_sales[1] > 0);
    }

    /**
     * Accumulates a sale on the current product info
     * @param sale
     */
    @Override
    public void accumulateSaleOnInfo(ISale sale){
        int type;
        if(sale.getType() == 'P') type = 1;
        else type = 0;

        this.sales[type][sale.getFilial()-1][sale.getMonth()-1] += sale.getAmount();
        this.profit[type][sale.getFilial()-1][sale.getMonth()-1] += sale.getPrice() * sale.getAmount();
        this.total_sales[type] += sale.getAmount();
        this.total_profit[type] += sale.getAmount() * sale.getPrice();
    }

    @Override
    public Float[][] collectProductInfo() {
        Float[][] array = new Float[number_of_months][number_of_branches];
        for(int i = 0; i < number_of_months; i++) Arrays.fill(array[i], (float) 0);
        for (int i = 0; i < number_of_types; i++) {
            for (int j = 0; j < number_of_months; j++) {
                for (int k = 0; k < number_of_branches; k++) {
                    array[j][k] += (float) profit[i][k][j];
                }
            }
        }
        return array;
    }

    @Override
    public Float getProfitInBranch(int month, int branch){
        return (float) (this.profit[0][branch-1][month-1] + this.profit[1][branch-1][month-1]);
    }

    /**
     * Clones the object
     * @return ProductInfo
     */
    @Override
    public IProductInfo clone() {
        return new ProductInfo(this);
    }
}

package model.configs_n_stats;

import java.io.Serializable;

public class Statistics implements IStatistics, Serializable {
    private int total_sales;
    private int valid_sales;
    private String sales_file_name;
    private int zero_value_sales;

    public Statistics(){
        total_sales = valid_sales = 0;
        sales_file_name = "";
    }

    public Statistics(int total_sales, int valid_sales, String sales_file_name, int zero_value_sales){
        this.total_sales = total_sales;
        this.valid_sales = valid_sales;
        this.sales_file_name = sales_file_name;
        this.zero_value_sales = zero_value_sales;
    }

    public Statistics(IStatistics statistics){
        this.total_sales = statistics.getTotal_sales();
        this.valid_sales = statistics.getValid_sales();
        this.sales_file_name = statistics.getSales_file_name();
        this.zero_value_sales = statistics.getZeroValueSales();
    }

    @Override
    public int getTotal_sales(){
        return total_sales;
    }

    @Override
    public int getValid_sales(){
        return valid_sales;
    }

    @Override
    public String getSales_file_name(){
        return sales_file_name;
    }

    @Override
    public void setSales_file_name(String sales_file_name){
        this.sales_file_name = sales_file_name;
    }

    @Override
    public void incrementTotal_sales(){
        total_sales++;
    }

    @Override
    public void incrementValid_sales(){
        valid_sales++;
    }

    @Override
    public void setZeroValueSales(int zero_value_sales) {
        this.zero_value_sales = zero_value_sales;
    }

    @Override
    public int getZeroValueSales(){
        return zero_value_sales;
    }

    @Override
    public IStatistics clone(){
        return new Statistics(this);
    }
}

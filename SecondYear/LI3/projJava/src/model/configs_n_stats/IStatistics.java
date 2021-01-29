package model.configs_n_stats;

import java.io.Serializable;

public interface IStatistics extends Serializable {
    /**
     *
     * @return total amount of sales
     */
    int getTotal_sales();

    /**
     *
     * @return total valid sales
     */
    int getValid_sales();

    /**
     *
     * @return sales file name
     */
    String getSales_file_name();

    void setSales_file_name(String sales_file_name);

    void incrementTotal_sales();

    void incrementValid_sales();

    int getZeroValueSales();

    /**
     *
     * @return Copy of this structure
     */
    IStatistics clone();

    void setZeroValueSales(int zero_value_sales);
}

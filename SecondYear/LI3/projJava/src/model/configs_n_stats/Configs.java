package model.configs_n_stats;

import utilities.Utilities;

import java.io.*;
import java.nio.file.Paths;
import java.util.Properties;

/**
 * Class that holds public information needed for the program execution
 */
public class Configs implements IConfigs, Serializable {
    // Product related
    private int product_length = 6;
    private int product_letters_length = 2;
    private int product_numbers_length = 4;
    private int product_upper_limit = 9999;
    private int product_lower_limit = 1000;
    private String productsPath = Utilities.makePath(Paths.get("").toAbsolutePath().toString(), "db", "Produtos.txt");

    // Customer related
    private int customer_length = 5;
    private int customer_letters_length = 1;
    private int customer_numbers_length = 4;
    private int customer_upper_limit = 5000;
    private int customer_lower_limit = 1000;
    private String customersPath = Utilities.makePath(Paths.get("").toAbsolutePath().toString(), "db", "Clientes.txt");

    // Sales related
    private double min_price = 0.0;
    private double max_price = 999.99;
    private int min_amount = 1;
    private int max_amount = 200;
    private int number_of_branches = 3;
    private int number_of_months = 12;
    private int number_of_types = 2;
    private String salesPath = Utilities.makePath(Paths.get("").toAbsolutePath().toString(), "db", "Vendas_1M.txt");
    private String serializeDataPath = Utilities.makePathToFile(Paths.get("").toAbsolutePath().toString(), "gestVendas.dat");
    private String logPath = Utilities.makePathToFile(Paths.get("").toAbsolutePath().toString(),"q11_log.txt");

    public Configs(){
        try {
            new Configs(Utilities.makePath(Paths.get("").toAbsolutePath().toString(), "db", "config.txt"));
        } catch (Exception e){
        }
    }

    public Configs(String configsPath){
        InputStream inputStream;
        try {
            Properties prop = new Properties();

            inputStream = new FileInputStream(new File(configsPath));

            prop.load(inputStream);

            // get the property value and print it out
            number_of_branches = Integer.parseInt(prop.getProperty("NUM_BRANCHES", "3"));
            number_of_months = Integer.parseInt(prop.getProperty("NUM_MESES", "12"));
            number_of_types = Integer.parseInt(prop.getProperty("NUM_TYPES", "2"));
            product_letters_length = Integer.parseInt(prop.getProperty("PRODUCTS_LETTERS", "4"));
            product_numbers_length = Integer.parseInt(prop.getProperty("PRODUCTS_NUMBERS", "2"));
            product_length = product_letters_length + product_numbers_length;
            product_upper_limit = Integer.parseInt(prop.getProperty("PRODUCTS_UL","9999"));
            product_lower_limit = Integer.parseInt(prop.getProperty("PRODUCTS_LL","1000"));
            customer_letters_length = Integer.parseInt(prop.getProperty("CUSTOMERS_LETTERS", "1"));
            customer_numbers_length = Integer.parseInt(prop.getProperty("CUSTOMERS_NUMBERS", "4"));
            customer_upper_limit = Integer.parseInt(prop.getProperty("CUSTOMERS_UL","5000"));
            customer_lower_limit = Integer.parseInt(prop.getProperty("CUSTOMERS_LL","1000"));
            customer_length = customer_letters_length + customer_numbers_length;
            min_price = Double.parseDouble(prop.getProperty("MIN_PRICE", "0.0"));
            max_price = Double.parseDouble(prop.getProperty("MAX_PRICE", "999.99"));
            min_amount = Integer.parseInt((prop.getProperty("MIN_AMOUNT", "0")));
            max_amount = Integer.parseInt((prop.getProperty("MAX_AMOUNT", "200")));
            productsPath = prop.getProperty("PRODUCTS_PATH", productsPath);
            customersPath = prop.getProperty("CUSTOMERS_PATH", customersPath);
            salesPath = prop.getProperty("SALES_PATH", salesPath);
            serializeDataPath = prop.getProperty("SERIALIZE_PATH", serializeDataPath);
            logPath = prop.getProperty("LOG_Q11_PATH", logPath);
        } catch (Exception e) {
            System.out.println("Couldn't load config so we're going with default configuration!");
        }
    }

    public Configs(Configs configs){
        this.product_length = configs.getProduct_length();
        this.product_letters_length = configs.getProduct_letters_length();
        this.product_numbers_length = configs.getProduct_numbers_length();
        this.product_upper_limit = configs.getProduct_upper_limit();
        this.product_lower_limit = configs.getProduct_lower_limit();
        this.productsPath = configs.getProductsPath();
        this.customer_length = configs.getCustomer_length();
        this.customer_letters_length = configs.getCustomer_letters_length();
        this.customer_numbers_length = configs.getCustomer_numbers_length();
        this.customer_upper_limit = configs.getCustomer_upper_limit();
        this.customer_lower_limit = configs.getCustomer_lower_limit();
        this.customersPath = configs.getCustomersPath();
        this.min_price = configs.getMin_price();
        this.max_price = configs.getMax_price();
        this.min_amount = configs.getMin_amount();
        this.max_amount = configs.getMax_amount();
        this.number_of_branches = configs.getNumber_of_branches();
        this.number_of_months = configs.getNumber_of_months();
        this.number_of_types = configs.getNumber_of_types();
        this.salesPath = configs.getSalesPath();
        this.serializeDataPath = configs.getSerializeDataPath();
        this.logPath = configs.getLogPath();
    }

    /* Getters */
    @Override
    public String getLogPath() {
        return logPath;
    }

    @Override
    public int getProduct_length() {
        return product_length;
    }

    @Override
    public int getProduct_letters_length() {
        return product_letters_length;
    }

    @Override
    public int getProduct_numbers_length() {
        return product_numbers_length;
    }

    @Override
    public int getProduct_upper_limit() {
        return product_upper_limit;
    }

    @Override
    public int getProduct_lower_limit() {
        return product_lower_limit;
    }

    @Override
    public String getProductsPath() {
        return productsPath;
    }

    @Override
    public int getCustomer_length() {
        return customer_length;
    }

    @Override
    public int getCustomer_letters_length() {
        return customer_letters_length;
    }

    @Override
    public int getCustomer_numbers_length() {
        return customer_numbers_length;
    }

    @Override
    public int getCustomer_upper_limit() {
        return customer_upper_limit;
    }

    @Override
    public int getCustomer_lower_limit() {
        return customer_lower_limit;
    }

    @Override
    public String getCustomersPath() {
        return customersPath;
    }

    @Override
    public double getMin_price() {
        return min_price;
    }

    @Override
    public double getMax_price() {
        return max_price;
    }

    @Override
    public int getMin_amount() {
        return min_amount;
    }

    @Override
    public int getMax_amount() {
        return max_amount;
    }

    @Override
    public int getNumber_of_branches() {
        return number_of_branches;
    }

    @Override
    public int getNumber_of_months() {
        return number_of_months;
    }

    @Override
    public int getNumber_of_types() {
        return number_of_types;
    }

    @Override
    public String getSalesPath() {
        return salesPath;
    }

    @Override
    public String getSerializeDataPath(){
        return serializeDataPath;
    }

    @Override
    public IConfigs clone(){
        return new Configs(this);
    }
}

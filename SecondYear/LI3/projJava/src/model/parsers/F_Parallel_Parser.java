package model.parsers;

import com.jakewharton.fliptables.FlipTableConverters;
import model.IGestVendasModel;
import model.base.ISale;
import model.base.Sale;
import model.configs_n_stats.IConfigs;
import model.configs_n_stats.IStatistics;

import java.io.File;
import java.io.IOException;
import java.io.PrintWriter;
import java.nio.file.Files;
import java.nio.file.Paths;

public class F_Parallel_Parser implements IParser {
    private IConfigs configs;
    private double salesReadingElapsedTime;

    public F_Parallel_Parser(IConfigs configs){
        this.configs = configs.clone();
    }

    /**
     * Tells us if a customer is valid or not
     * @param customer
     * @return boolean
     */
    @Override
    public boolean validCustomer(String customer) throws NumberFormatException {
        if(customer.length() < configs.getCustomer_length()) return false;
        String letters = customer.substring(0, configs.getCustomer_letters_length());
        int code = 0;
        // In case the string has the wrong format
        try {
            code = Integer.parseInt(customer.substring(configs.getCustomer_letters_length(), configs.getCustomer_length()));
        } catch (NumberFormatException e){
            System.out.println("Invalid format on customers file!");
            return false;
        }
        return customer.length() == configs.getCustomer_length() && letters.matches("[A-Z]+") && code <= configs.getCustomer_upper_limit() && code >= configs.getCustomer_lower_limit();
    }

    /**
     * Tells us if a product is valid or not
     * @param product
     * @return boolean
     */
    @Override
    public boolean validProduct(String product) throws NumberFormatException{
        if(product.length() < configs.getProduct_length()) return false;
        String letters = product.substring(0, configs.getProduct_letters_length());
        // In case the string has the wrong format
        int code;
        try {
            code = Integer.parseInt(product.substring(configs.getProduct_letters_length(), configs.getProduct_length()));
        } catch (NumberFormatException e) {
            System.out.println("Invalid format on products file!");
            return false;
        }
        return letters.matches("[A-Z]+") && code <= configs.getProduct_upper_limit() && code >= configs.getProduct_lower_limit();
    }

    /**
     * Tells us if a sale is valid or not
     * @param sale
     * @return boolean
     */
    @Override
    public boolean validSale(IGestVendasModel model, ISale sale){
        model.incrementTotal_sales();

        if(!model.existsInProductCatalog(sale.getProduct().getCode())) {
            return false;
        }

        if(!model.existsInCustomerCatalog(sale.getCustomer().getCode())){
            return false;
        }

        if(sale.getPrice() < configs.getMin_price() || sale.getPrice() > configs.getMax_price()){
            return false;
        } else if(sale.getAmount() < configs.getMin_amount() || sale.getAmount() > configs.getMax_amount()){
            return false;
        } else if(sale.getType() != 'N' && sale.getType() != 'P'){
            return false;
        } else if(sale.getFilial() < 1 || sale.getFilial() > configs.getNumber_of_branches()){
            return false;
        } else if(sale.getMonth() < 1 || sale.getMonth() > configs.getNumber_of_months()){
            return false;
        }

        model.incrementValid_sales();
        return true;
    }

    /**
     * Creates the product catalog based on the info read from the path
     * @param path
     * @return ProductCatalog and Billing
     * @throws IOException
     */
    @Override
    public void readProducts(IGestVendasModel model, String path) throws IOException{
        try{
            // Get the lines, filter the valid ones and put them into the catalog
            Files.lines(Paths.get(path))
                    .filter(this::validProduct)
                    .forEach(l -> {
                        model.addEmptyProductToFac(l);
                        model.addToProductCatalog(l);
                    });
        } catch(IOException e) {
            throw new IOException("No such file");
        }
    }

    /**
     * Creates the customer catalog based on the info read from the path
     * @param path
     * @return CustomerCatalog
     * @throws IOException
     */
    @Override
    public void readCustomers(IGestVendasModel model, String path) throws IOException{
        try{
            // Get the lines, filter the valid ones and put them into the catalog
            Files.lines(Paths.get(path))
                    .filter(this::validCustomer)
                    .forEach(l -> {
                        model.addToCustomerCatalog(l);
                        model.addEmptyCustomerToAllBranches(l);
                    });
        }catch(IOException e) {
            throw new IOException("No such file");
        }
    }

    /**
     * Parses the sales file and puts the data read into Billing and Branches
     * @param path
     * @return void
     * @throws IOException
     */
    @Override
    public void readSales(IGestVendasModel model, String path) throws IOException{
        try{
            // Get the lines, filter the valid ones and put them into the catalog
            Files.lines(Paths.get(path))
                    .parallel()
                    .map(s -> s.split(" "))
                    .map(Sale::new)
                    .filter(s -> validSale(model, s))
                    .forEach(s -> {
                        model.billProduct(s);
                        model.customerBoughtInBranch(s);
                    });
        }catch(IOException e) {
            throw new IOException("No such file");
        }
    }

    @Override
    public void logSales(IGestVendasModel model, String logPath) throws IOException {
        File file =  new File(logPath);
        file.createNewFile();

        PrintWriter pw = new PrintWriter(file);
        IStatistics statistics = model.getStatistics();
        int differentProductsBought = model.countDifferentProductsBought();
        int number_of_products = model.number_of_products();
        int number_of_customers = model.number_of_customers();
        int customersWhoBought = model.countCustomersWhoBought();
        int zeroValueSales = statistics.getZeroValueSales();
        float totalBilling = model.totalBilling();

        String[] headers = {"", ""};
        Object[][] data = new Object[10][2];

        data[0][0] = "File name"; data[0][1] = statistics.getSales_file_name();
        data[1][0] = "Invalid sales"; data[1][1] = (statistics.getTotal_sales() - statistics.getValid_sales());
        data[2][0] = "Total of products"; data[2][1] = number_of_products;
        data[3][0] = "Total of different products bought:"; data[3][1] = differentProductsBought;
        data[4][0] = "Total of different products not bought"; data[4][1] = (number_of_products - differentProductsBought);
        data[5][0] = "Total of customers"; data[5][1] = number_of_customers;
        data[6][0] = "Total of customers who purchased something"; data[6][1] = customersWhoBought;
        data[7][0] = "Total of customers who purchased nothing"; data[7][1] = (number_of_customers - customersWhoBought);
        data[8][0] = "Total of zero value sales"; data[8][1] = zeroValueSales;
        data[9][0] = "Total billing"; data[9][1] = totalBilling;

        pw.println(FlipTableConverters.fromObjects(headers, data));

        pw.flush();
        pw.close();
    }
}

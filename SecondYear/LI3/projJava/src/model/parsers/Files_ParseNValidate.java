package model.parsers;

import model.IGestVendasModel;
import model.base.ISale;
import model.base.Sale;
import model.configs_n_stats.IConfigs;
import utilities.QueryTime;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;

public class Files_ParseNValidate implements IParser{
    private IConfigs configs;

    public Files_ParseNValidate(IConfigs configs){
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
                    .map(s -> s.split(" "))
                    .map(Sale::new)
                    .filter(s -> validSale(model, s))
                    .forEach(s -> System.out.print(""));
        }catch(IOException e) {
            throw new IOException("No such file");
        }
    }

    @Override
    public void logSales(IGestVendasModel model, String logPath) throws FileNotFoundException {

    }
}

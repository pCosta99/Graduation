package model.parsers;

import model.IGestVendasModel;
import model.base.ISale;
import model.configs_n_stats.IConfigs;

import java.io.FileNotFoundException;
import java.io.IOException;

public interface IParser {
    /**
     * Indicates if a certain customer is valid format wise
     * @param customer
     * @return boolean
     * @throws NumberFormatException
     */
    boolean validCustomer(String customer) throws NumberFormatException;

    /**
     * Indicates if a certain product is valid format wise
     * @param product
     * @return boolean
     * @throws NumberFormatException
     */
    boolean validProduct(String product) throws NumberFormatException;

    /**
     * Indicates if a certain sale is valid
     * @param model
     * @param sale
     * @return boolean
     */
    boolean validSale(IGestVendasModel model, ISale sale);

    /**
     * Reads all products from the path provided into the model given
     * @param model
     * @param path
     * @throws IOException
     */
    void readProducts(IGestVendasModel model, String path) throws IOException;

    /**
     * Reads all products from the path provided into the model given
     * @param model
     * @param path
     * @throws IOException
     */
    void readCustomers(IGestVendasModel model, String path) throws IOException;

    /**
     * Reads all products from the path provided into the model given
     * @param model
     * @param path
     * @throws IOException
     */
    void readSales(IGestVendasModel model, String path) throws IOException;

    void logSales(IGestVendasModel model, String logPath) throws IOException;
}

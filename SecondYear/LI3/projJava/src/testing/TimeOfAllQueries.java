package testing;

import com.jakewharton.fliptables.FlipTable;
import model.GestVendasModel;
import model.IGestVendasModel;
import model.base.Customer;
import model.base.ICustomer;
import model.base.IProduct;
import model.base.Product;
import model.branches.ICustomerBought;
import model.configs_n_stats.Configs;
import model.configs_n_stats.IConfigs;
import model.parsers.*;
import org.apache.commons.lang3.tuple.ImmutablePair;
import org.javatuples.Pair;
import org.javatuples.Triplet;
import utilities.QueryTime;
import utilities.Utilities;

import java.io.IOException;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

public class TimeOfAllQueries {
    private static final String productsPath = Utilities.makePath(Paths.get("").toAbsolutePath().toString(), "db", "Produtos.txt");
    private static final String customersPath = Utilities.makePath(Paths.get("").toAbsolutePath().toString(), "db", "Clientes.txt");
    private static final String path_v1m = Utilities.makePath(Paths.get("").toAbsolutePath().toString(), "db", "Vendas_1M.txt");
    private static final String path_v3m = Utilities.makePath(Paths.get("").toAbsolutePath().toString(), "db", "Vendas_3M.txt");
    private static final String path_v5m = Utilities.makePath(Paths.get("").toAbsolutePath().toString(), "db", "Vendas_5M.txt");

    public static void main(String[] args) throws IOException {
        /* Setup configs and create model */
        IConfigs configs = new Configs();
        IGestVendasModel model = new GestVendasModel(configs);

        /* Put the different sales paths in an array */
        String[] paths = new String[3];
        paths[0] = path_v1m; paths[1] = path_v3m; paths[2] = path_v5m;

        /* Setup a parser */
        IParser br_parser = new BR_Parser(configs);

        /* Create a holding place for the times */
        List<List<Double>> times = new ArrayList<>();

        /* Define initial data set */
        int month = 2; String customer = "Z5000"; String product = "AF1184";

        /* Go through each sales file and parse it with all different parsers */
        for(int i = 0; i < 3; i++){
            /* Parse products and customers right ahead */
            model = new GestVendasModel(configs);
            br_parser.readCustomers(model, customersPath);
            br_parser.readProducts(model, productsPath);
            br_parser.readSales(model, paths[i]);
            List<Double> times_of_sales =  new ArrayList<>();
            for(int j = 0; j < 10; j++){
                double total_time = 0;
                for(int k = 0; k < 10; k++) {
                    QueryTime.start();
                    switch (j){
                        case 0:
                            model.allNotBoughtProducts();
                            break;
                        case 1:
                            model.globalSalesOnMonthInBranch(month);
                            break;
                        case 2:
                            model.infoAboutCustomer(customer);
                            break;
                        case 3:
                            model.productTotalMadeAndTotalCustomers(product);
                            break;
                        case 4:
                            model.productsBoughtByCustomer(customer);
                            break;
                        case 5:
                            model.salesAndDistinctCustomersPerProduct();
                            break;
                        case 6:
                            model.totalSpentByCustomers();
                            break;
                        case 7:
                            model.differentProductsBoughtByCustomers();
                            break;
                        case 8:
                            model.buyersOfCertainProduct(product);
                            break;
                        case 9:
                            model.allProductsBilling();
                            break;
                    }
                    total_time += QueryTime.stop();
                }
                double avg_time = total_time / 10;
                System.out.println("---------------------------------------------------------------------------");
                System.out.println("Query: " + (j+1) + "\nFile: " + paths[i] + "\nTime: " + avg_time);
                System.out.println("---------------------------------------------------------------------------");
                times_of_sales.add(avg_time);
            }
            times.add(times_of_sales);
        }
        /* print data */
        String[] headers = {"", "Vendas_1M", "Vendas_3M", "Vendas_5M"};
        String[][] data = new String[10][4];
        data[0][0] = "Query 1";
        data[1][0] = "Query 2";
        data[2][0] = "Query 3";
        data[3][0] = "Query 4";
        data[4][0] = "Query 5";
        data[5][0] = "Query 6";
        data[6][0] = "Query 7";
        data[7][0] = "Query 8";
        data[8][0] = "Query 9";
        data[9][0] = "Query 10";

        for(int i = 0; i < 10; i++){
            for(int j = 1; j < 4; j++){
                data[i][j] = String.valueOf(times.get(j-1).get(i));
            }
        }

        System.out.println(FlipTable.of(headers, data));
    }
}

package testing;

import model.GestVendasModel;
import model.IGestVendasModel;
import model.base.ICustomer;
import model.base.IProduct;
import model.branches.ICustomerBought;
import model.configs_n_stats.Configs;
import model.configs_n_stats.IConfigs;
import model.parsers.BR_Parser;
import model.parsers.IParser;
import org.javatuples.Pair;
import utilities.QueryTime;
import utilities.Utilities;

import java.io.IOException;
import java.nio.file.Paths;
import java.util.List;
import java.util.Map;

public class QueryTesting {
    private static final String productsPath = Utilities.makePath(Paths.get("").toAbsolutePath().toString(), "db", "Produtos.txt");
    private static final String customersPath = Utilities.makePath(Paths.get("").toAbsolutePath().toString(), "db", "Clientes.txt");
    private static final String path_v1m = Utilities.makePath(Paths.get("").toAbsolutePath().toString(), "db", "Vendas_1M.txt");
    private static final String path_v3m = Utilities.makePath(Paths.get("").toAbsolutePath().toString(), "db", "Vendas_3M.txt");
    private static final String path_v5m = Utilities.makePath(Paths.get("").toAbsolutePath().toString(), "db", "Vendas_5M.txt");

    public static void main(String[] args) throws IOException {
        /* Setup configs and create model */
        IConfigs configs = new Configs();
        IGestVendasModel model = new GestVendasModel(configs);

        /* Setup a parser and fill the model */
        IParser parser = new BR_Parser(configs);
        parser.readProducts(model, productsPath);
        parser.readCustomers(model, customersPath);
        parser.readSales(model, path_v5m);

        /* Run the queries we are interested in and log them */
        /* Query 5 -> TreeMap */
        double q5total = 0;
        for(int i = 0; i < 10; i++) {
            QueryTime.start();
            Map<IProduct, ICustomerBought> treeMapQ5 = model.productsBoughtByCustomer("Z5000");
            double time = QueryTime.stop();
            q5total += time;
            System.out.println("Query 5 took " + time + " seconds using an TreeMap!");
        }
        System.out.println("Query 5 took on average " + q5total/10 + " seconds using an TreeMap!");
        System.out.println("\n\n-------------------------------------------------------------------------\n\n");

        /* Query 6 -> TreeMap */
        double q6total = 0;
        for(int i = 0; i < 10; i++) {
            QueryTime.start();
            Map<IProduct, Pair<Integer, Integer>> treeMapQ6 = model.salesAndDistinctCustomersPerProduct();
            double time = QueryTime.stop();
            q6total += time;
            System.out.println("Query 6 took " + time + " seconds using an TreeMap!");
        }
        System.out.println("Query 6 took on average " + q6total/10 + " seconds using an TreeMap!");
        System.out.println("\n\n-------------------------------------------------------------------------\n\n");

        /* Query 7 -> TreeMap */
        double q7total = 0;
        for(int i = 0; i < 10; i++) {
            QueryTime.start();
            List<Map<ICustomer, Float>> treeMapQ7 = model.totalSpentByCustomers();
            double time = QueryTime.stop();
            q7total += time;
            System.out.println("Query 7 took " + time + " seconds using an TreeMap!");
        }
        System.out.println("Query 7 took on average " + q7total/10 + " seconds using an TreeMap!");
        System.out.println("\n\n-------------------------------------------------------------------------\n\n");

        /* Query 8 -> TreeMap */
        double q8total = 0;
        for(int i = 0; i < 10; i++) {
            QueryTime.start();
            Map<ICustomer, Integer> treeMapQ8 = model.differentProductsBoughtByCustomers();
            double time = QueryTime.stop();
            q8total += time;
            System.out.println("Query 8 took " + time + " seconds using an TreeMap!");
        }
        System.out.println("Query 8 took on average " + q8total/10 + " seconds using an TreeMap!");
        System.out.println("\n\n-------------------------------------------------------------------------\n\n");

        /* Query 9 -> TreeMap */
        double q9total = 0;
        for(int i = 0; i < 10; i++) {
            QueryTime.start();
            Map<ICustomer, Pair<Integer, Float>> treeMapQ9 = model.buyersOfCertainProduct("AF1184");
            double time = QueryTime.stop();
            q9total += time;
            System.out.println("Query 9 took " + time + " seconds using an TreeMap!");
        }
        System.out.println("Query 9 took on average " + q9total/10 + " seconds using an TreeMap!");
        System.out.println("\n\n-------------------------------------------------------------------------\n\n");
    }
}

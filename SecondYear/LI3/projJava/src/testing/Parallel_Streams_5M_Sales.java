package testing;

import model.GestVendasModel;
import model.IGestVendasModel;
import model.configs_n_stats.Configs;
import model.configs_n_stats.IConfigs;
import model.parsers.*;
import utilities.QueryTime;
import utilities.Utilities;

import java.io.IOException;
import java.nio.file.Paths;
import java.util.HashMap;
import java.util.Map;

public class Parallel_Streams_5M_Sales {
    private static final String productsPath = Utilities.makePath(Paths.get("").toAbsolutePath().toString(), "db", "Produtos.txt");
    private static final String customersPath = Utilities.makePath(Paths.get("").toAbsolutePath().toString(), "db", "Clientes.txt");
    private static final String path_v5m = Utilities.makePath(Paths.get("").toAbsolutePath().toString(), "db", "Vendas_5M.txt");

    public static void main(String[] args) throws IOException {
        /* Setup configs and create model */
        IConfigs configs = new Configs();
        IGestVendasModel model = new GestVendasModel(configs);

        /* Launch all the different parser we will be testing */
        IParser br_parser = new BR_Parser(configs);
        Map<String, IParser> parserMap = new HashMap<>();
        parserMap.put("F_Par_NP", new F_Parallel_NP(configs));
        parserMap.put("F_Par_NV", new F_Parallel_NV(configs));
        parserMap.put("F_Par_PV", new F_Parallel_PV(configs));
        parserMap.put("F_Par_Parser", new F_Parallel_Parser(configs));

        /* Parse products and customers right ahead */
        br_parser.readCustomers(model, customersPath);
        br_parser.readProducts(model, productsPath);

        /* Go through each sales file and parse it with all different parsers */
        for(Map.Entry<String, IParser> entry : parserMap.entrySet()){
            double total_time = 0;
            for(int k = 0; k < 10; k++) {
                QueryTime.start();
                entry.getValue().readSales(model, path_v5m);
                total_time += QueryTime.stop();
            }
            double avg_time = total_time / 10;
            System.out.println("---------------------------------------------------------------------------");
            System.out.println("Parser: " + entry.getKey() + "\nFile: " + path_v5m + "\nTime: " + avg_time);
            System.out.println("---------------------------------------------------------------------------");
        }
    }
}

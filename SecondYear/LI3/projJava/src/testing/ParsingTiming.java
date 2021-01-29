package testing;

import model.GestVendasModel;
import model.IGestVendasModel;
import model.configs_n_stats.Configs;
import model.configs_n_stats.IConfigs;
import model.parsers.*;
import utilities.QueryTime;
import utilities.Utilities;
import view.GestVendasView;

import java.io.IOException;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class ParsingTiming {
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

        /* Launch all the different parser we will be testing */
        Map<String, IParser> parserMap = new HashMap<>();
        parserMap.put("BR_NP", new BR_NoParsing(configs));
        parserMap.put("F_NP", new Files_NoParsing(configs));
        parserMap.put("BR_NV", new BR_NoValidation(configs));
        parserMap.put("F_NV", new Files_NoValidation(configs));
        parserMap.put("BR_PV", new BR_ParseNValidate(configs));
        parserMap.put("F_PV", new Files_ParseNValidate(configs));
        IParser br_parser = new BR_Parser(configs);
        IParser f_parser = new Files_Parser(configs);

        /* Parse products and customers right ahead */
        br_parser.readCustomers(model, customersPath);
        br_parser.readProducts(model, productsPath);

        /* Parse them completely first */
        for(int i = 0; i < 3; i++){
            for(int j = 0; j < 2; j++){
                String parser = "BR_Total";
                if(j == 1) parser = "F_Total";
                double total_time = 0;
                for(int k = 0; k < 5; k++) {
                    QueryTime.start();
                    if(j == 0) br_parser.readSales(model, paths[i]);
                    else f_parser.readSales(model, paths[i]);
                    total_time += QueryTime.stop();
                }
                double avg_time = total_time / 5;
                System.out.println("---------------------------------------------------------------------------");
                System.out.println("Parser: " + parser + "\n File: " + paths[i] + "\n Time: " + avg_time);
                System.out.println("---------------------------------------------------------------------------");
            }
        }

        /* Go through each sales file and parse it with all different parsers */
        for(int i = 0; i < 3; i++){
            for(Map.Entry<String, IParser> entry : parserMap.entrySet()){
                double total_time = 0;
                for(int k = 0; k < 10; k++) {
                    QueryTime.start();
                    entry.getValue().readSales(model, paths[i]);
                    total_time += QueryTime.stop();
                }
                double avg_time = total_time / 10;
                System.out.println("---------------------------------------------------------------------------");
                System.out.println("Parser: " + entry.getKey() + "\n File: " + paths[i] + "\n Time: " + avg_time);
                System.out.println("---------------------------------------------------------------------------");
            }
        }
    }
}

package controller;

import model.configs_n_stats.Configs;
import model.configs_n_stats.IConfigs;
import model.navigator.INavigator;
import model.navigator.Navigator;
import model.parsers.BR_Parser;
import model.parsers.IParser;
import utilities.QueryTime;
import exceptions.*;
import model.GestVendasModel;
import model.IGestVendasModel;
import model.base.ICustomer;
import model.base.IProduct;
import model.branches.ICustomerBought;
import model.configs_n_stats.IStatistics;
import org.apache.commons.lang3.tuple.ImmutablePair;
import org.javatuples.Pair;
import org.javatuples.Triplet;
import utilities.Utilities;
import view.GestVendasView;
import view.IGestVendasView;

import java.io.File;
import java.io.IOException;
import java.io.PrintStream;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.*;
import java.util.stream.Collectors;

public class GestVendasController implements IGestVendasController {
    private IGestVendasView view;
    private IGestVendasModel model;
    private IParser parser;
    private Scanner scanner;
    private static final PrintStream console = System.out;

    public GestVendasController(String configsPath){
        IConfigs configs = new Configs(configsPath);
        this.model = new GestVendasModel(configs);
        this.view = new GestVendasView();
        this.parser = new BR_Parser(configs);
        this.scanner = new Scanner(System.in);
    }

    public GestVendasController(){
        IConfigs configs = new Configs();
        this.model = new GestVendasModel(configs);
        this.view = new GestVendasView();
        this.parser = new BR_Parser(configs);
        this.scanner = new Scanner(System.in);
    }

    private void waitForInput(){
        System.out.println("Press enter to continue...");
        scanner.nextLine();
    }

    private void navigate(INavigator navigator){
        navigator.triggerStatusBar();
        navigator.setHelpMessage("Options: \nQuit -> q|quit \nNext page -> np|nextp \nPrevious page -> pp|previousp \nGo to page X -> pX \nHelp -> h|help \nLookup regex -> /regex \nMove on lookup matches -> n/p");
        navigator.setDefaultMessage("Waiting for a mess up...");
        String s = "";

        while(!(s.toLowerCase().equals("quit") || s.toLowerCase().equals("q"))) {
            view.clearScreen();
            view.screenInfo(navigator.toString() + "\n$ ");
            s = scanner.nextLine();
            if(s.toLowerCase().matches("/.+")){
                navigator.find(s.substring(1));
            } else if(s.toLowerCase().equals("h") || s.toLowerCase().equals("help")){
                navigator.callHelpMessage();
            } else if(s.toLowerCase().matches("p[0-9]+")){
                navigator.getPage(Integer.parseInt(s.substring(1)));
            } else if(s.toLowerCase().equals("nextp") || s.toLowerCase().equals("np")) {
                navigator.nextPage();
            } else if(s.toLowerCase().equals("previousp") || s.toLowerCase().equals("pp")) {
                navigator.previousPage();
            } else if(s.toLowerCase().equals("p")){
                navigator.previousOcurrence();
            } else if(s.toLowerCase().equals("n")){
                navigator.nextOcurrence();
            } else navigator.callHelpMessage();
        }
    }

    public void inNavigator(List<String> list, List<String> headers) {
        INavigator navigator = new Navigator(10, list, headers);
        navigate(navigator);
    }

    public void inNavigator(List<String> list){
        INavigator navigator = new Navigator(10, 25, list);
        navigate(navigator);
    }

    private void inNavigator_MultipleHeaders(List<String> list, ArrayList<List<String>> headers){
        INavigator navigator = new Navigator(3, list, headers);
        navigate(navigator);
    }

    private void redirectOutToSaveFile(int query) throws IOException {
        /* If stats doesn't exists, create it */
        File stats_dir = new File(Utilities.makePath(Paths.get("").toAbsolutePath().toString(), "stats"));
        if(!stats_dir.exists()) stats_dir.mkdir();

        /* If the query save file doesn't exists, create it */
        String query_path = Utilities.makePath(Paths.get("").toAbsolutePath().toString(), "stats", "query" + query + ".txt");
        File file =  new File(query_path);
        file.createNewFile();

        // Creating a File object that represents the disk file.
        PrintStream o = new PrintStream(file);

        // Assign o to output stream
        System.setOut(o);
    }

    private void redirectOutToSaveFile(int query, String extra) throws IOException {
        /* If stats doesn't exists, create it */
        File stats_dir = new File(Utilities.makePath(Paths.get("").toAbsolutePath().toString(), "stats"));
        if(!stats_dir.exists()) stats_dir.mkdir();

        /* If the query save file doesn't exists, create it */
        String query_path = Utilities.makePath(Paths.get("").toAbsolutePath().toString(), "stats", "query" + query + "_" + extra + ".txt");
        File file =  new File(query_path);
        file.createNewFile();

        // Creating a File object that represents the disk file.
        PrintStream o = new PrintStream(file);

        // Assign o to output stream
        System.setOut(o);
    }

    private void destroyRedirectOut(){
        System.setOut(console);
    }

    private void serializeModel() throws IOException {
        /* Start timing */
        QueryTime.start();

        /* Save the object in a .dat file */
        model.saveState(model.getConfigs().getSerializeDataPath());

        /* Finish timing and inform user */
        double elapsedTime = QueryTime.stop();
        view.screenInfo("Elapsed time: " + elapsedTime + "\n");
        waitForInput();
    }

    private void deserializeModel() throws IOException, ClassNotFoundException {
        /* Start timing */
        QueryTime.start();

        /* Call the string form to get the path for the .dat */
        view.screenInfo("Path to .dat\n");
        String datPath = scanner.nextLine();

        /* Load the object */
        model = model.loadState(datPath);

        /* Finish timing and inform user */
        double elapsedTime = QueryTime.stop();
        view.screenInfo("Elapsed time: " + elapsedTime + "\n");
        waitForInput();
    }

    private void loadFilesThroughPaths() {
        /* Re-init the parser */
        parser = new BR_Parser(model.getConfigs());

        /* Load model with these files */
        if(model.isInitialized()) model = new GestVendasModel(model);

        /* Call the path view forms and Parse stuff */
        view.screenInfo("Products path:\n");
        String productsPath = scanner.nextLine();
        view.screenInfo("Customers path:\n");
        String customersPath = scanner.nextLine();
        view.screenInfo("Sales path:\n");
        String salesPath = scanner.nextLine();

        /* Start Timing */
        QueryTime.start();
        try {
            parser.readProducts(model, productsPath);
            parser.readCustomers(model, customersPath);
            parser.readSales(model, salesPath);
        }
        catch (IOException e) {
            view.displayError("File path does't exist!");
            return;
        }
        /* Finish Timing */
        double elapsedTime = QueryTime.stop();
        view.screenInfo("Elapsed time: " + elapsedTime + "\n");

        /* Set the sales file name */
        model.setSales_file_name(salesPath);

        /* Mark as initialized */
        model.setInitialized();

        /* Log some info about the sales */
        try {
            parser.logSales(model, model.getConfigs().getLogPath());
        } catch (IOException e) {
            view.displayError("Couldn't locate the file where to log statistic info.");
        }

        waitForInput();
    }

    private void loadDefaultFiles() throws IOException {
        /* Start Timing */
        QueryTime.start();

        /* Re-init the parser */
        parser = new BR_Parser(model.getConfigs());

        /* Load model with these files */
        if(model.isInitialized()) model = new GestVendasModel(model);

        /* Parse stuff */
        parser.readProducts(model, model.getConfigs().getProductsPath());
        parser.readCustomers(model, model.getConfigs().getCustomersPath());
        parser.readSales(model, model.getConfigs().getSalesPath());

        /* Finish Timing */
        double elapsedTime = QueryTime.stop();
        view.screenInfo("Elapsed time: " + elapsedTime + "\n");

        /* Set the sales file name */
        model.setSales_file_name(model.getConfigs().getSalesPath());

        /* Mark as initialized */
        model.setInitialized();

        /* Log some info about the sales */
        try {
            parser.logSales(model, model.getConfigs().getLogPath());
        } catch (IOException e) {
            view.displayError("Couldn't the file where to log statistic info.");
        }

        waitForInput();
    }

    private void query1_allNotBoughtProducts() throws IOException {
        /* Start Timing */
        QueryTime.start();
        /* Get the products */
        List<IProduct> notBoughtProducts = this.model.allNotBoughtProducts();
        /* Sort them alphabetically */
        notBoughtProducts.sort(Comparator.comparing(IProduct::getCode));
        /* Finish Timing and show the user the time elapsed */
        view.screenInfo("Elapsed time: " + QueryTime.stop() + "\n");
        waitForInput();
        /* Pass them to view */
        inNavigator(notBoughtProducts.stream().map(IProduct::getCode).collect(Collectors.toList()));
        //view.printProductCollection(notBoughtProducts);
        /* If saveToFile is on, re-run the same view with the redirect */
        if(model.saveToFile()) {
            redirectOutToSaveFile(1);
            view.printProductCollection(notBoughtProducts);
            destroyRedirectOut();
        }
    }

    private void query2_globalSalesOnMonthInBranch() throws InvalidMonthException, IOException {
        /* Call the month view form */
        int month;
        view.screenInfo("What month?\n");
        try {
            month = scanner.nextInt();
        } catch (NumberFormatException e){
            view.displayError("The month must be represented numerically.");
            waitForInput();
            return;
        }

        /* Throw exception if the input is not valid */
        if(month < 1 || month > 12) throw new InvalidMonthException();

        /* Start Timing */
        QueryTime.start();

        /* Get the values of each branch */
        List<ImmutablePair<Integer,Integer>> list = model.globalSalesOnMonthInBranch(month);

        /* Finish Timing and show the user the time elapsed */
        view.screenInfo("Elapsed time: " + QueryTime.stop() + "\n");
        waitForInput();

        /* Send data to view */
        view.displayPerBranchInfo(list);
        waitForInput();

        /* If saveToFile is on, re-run the same view with the redirect */
        if(model.saveToFile()) {
            redirectOutToSaveFile(2, "Month" + month);
            view.displayPerBranchInfo(list);
            destroyRedirectOut();
        }
    }

    private void query3_infoAboutCustomer() throws InvalidCustomerException, IOException {
        /* Call the customer view form */
        view.screenInfo("What customer?\n");
        String customer = scanner.nextLine();

        /* Throw exception if the code isn't valid */
        if(!parser.validCustomer(customer) || !model.existsInCustomerCatalog(customer)) throw new InvalidCustomerException();

        /* Start Timing */
        QueryTime.start();

        /* Get the values of each branch */
        List<Triplet<Integer, Integer, Float>> list = model.infoAboutCustomer(customer);

        /* Finish Timing and show the user the time elapsed */
        view.screenInfo("Elapsed time: " + QueryTime.stop() + "\n");
        waitForInput();

        /* Pass them to the view */
        String[] headers = {"Month", "Purchases", "Products bought", "Total spent"};
        view.monthly_triplet_list_view_with_header(list, headers);
        waitForInput();

        /* If saveToFile is on, re-run the same view with the redirect */
        if(model.saveToFile()) {
            redirectOutToSaveFile(3, customer);
            view.monthly_triplet_list_view_with_header(list, headers);
            destroyRedirectOut();
        }
    }

    private void query4_productTotalMadeAndTotalCustomers() throws InvalidProductException, IOException {
        /* Call the product view form */
        view.screenInfo("What product?\n");
        String product = scanner.nextLine();

        /* Throw exception if the code isn't valid */
        if(!parser.validProduct(product) || !model.existsInProductCatalog(product)) throw new InvalidProductException();

        /* Start Timing */
        QueryTime.start();

        /* Get the data from model */
        List<Triplet<Integer, Integer, Float>> list = model.productTotalMadeAndTotalCustomers(product);

        /* Finish Timing and show the user the time elapsed */
        view.screenInfo("Elapsed time: " + QueryTime.stop() + "\n");
        waitForInput();

        /* Pass it to the view */
        String[] headers = {"Month", "Number of purchases", "Number of customers", "Profit"};
        view.monthly_triplet_list_view_with_header(list, headers);
        waitForInput();

        /* If saveToFile is on, re-run the same view with the redirect */
        if(model.saveToFile()) {
            redirectOutToSaveFile(4, product);
            view.monthly_triplet_list_view_with_header(list, headers);
            destroyRedirectOut();
        }
    }

    private void query5_productsBoughtByCustomer() throws InvalidCustomerException, IOException {
        /* Call the customer view form */
        view.screenInfo("What customer?\n");
        String customer = scanner.nextLine();

        /* Throw exception if the code isn't valid */
        if(!parser.validCustomer(customer) || !model.existsInCustomerCatalog(customer)) throw new InvalidCustomerException();

        /* Start Timing */
        QueryTime.start();

        /* Get the data from model and sort it as we wish */
        Comparator<ICustomerBought> byTotalPurchases = Comparator.comparing(ICustomerBought::getNCompras);
        Comparator<ICustomerBought> alphabetically = Comparator.comparing(o -> o.getProduct().getCode());
        Map<IProduct, ICustomerBought> productsBoughtByCustomer = model.productsBoughtByCustomer(customer);
        List<Pair<Object, Object>> list = productsBoughtByCustomer.values().stream().sorted(byTotalPurchases.reversed().thenComparing(alphabetically)).map(cb -> new Pair<>((Object) cb.getProduct(), (Object) cb.getNCompras())).collect(Collectors.toList());

        /* Finish Timing and show the user the time elapsed */
        view.screenInfo("Elapsed time: " + QueryTime.stop() + "\n");
        waitForInput();

        /* Pass it to the view */
        String[] headers = {"Product", "Quantity"};
        if(list.size() < 15) {
            view.pair_list_view_with_header(list, headers);
            waitForInput();
        }
        else {
            List<String> strings = new ArrayList<>();
            list.forEach(p -> {
                strings.add(String.valueOf(p.getValue0()));
                strings.add(String.valueOf(p.getValue1()));
            });
            inNavigator(strings, Arrays.asList(headers));
        }

        /* If saveToFile is on, re-run the same view with the redirect */
        if(model.saveToFile()) {
            redirectOutToSaveFile(5, customer);
            view.pair_list_view_with_header(list, headers);
            destroyRedirectOut();
        }
    }

    private void query6_XProductsMostSold() throws OutOfBoundsException, IOException {
        /* Call the int view form */
        int x;
        view.screenInfo("How many?\n");
        try {
            x = scanner.nextInt();
        }
        catch (NumberFormatException e) {
            view.displayError("Must insert a number!");
            waitForInput();
            return;
        }

        /* Throw exception if the number isn't valid */
        if (x > model.number_of_products() || x < 0) throw new OutOfBoundsException();

        /* Start Timing */
        QueryTime.start();

        /* Get the data from model and sort it as we wish */
        Map<IProduct, Pair<Integer, Integer>> map = model.salesAndDistinctCustomersPerProduct();

        /* Finish Timing and show the user the time elapsed */
        waitForInput();
        view.screenInfo("Elapsed time: " + QueryTime.stop() + "\n");

        /* Re-arrange the info to send to view */
        List<Triplet<IProduct, Integer, Integer>> list = map.entrySet().stream().sorted(Comparator.comparing(e -> e.getValue().getValue1())).map(e -> new Triplet<>(e.getKey(), e.getValue().getValue0(), e.getValue().getValue1())).collect(Collectors.toList());
        Collections.reverse(list);

        /* Pass it to the view */
        String[] headers = {"Product", "Distinct customers", "Number of sales"};
        List<Triplet<Object,Object,Object>> displayable = list.subList(0, x).stream().map(t -> new Triplet<>((Object) t.getValue0(), (Object) t.getValue1(), (Object) t.getValue2())).collect(Collectors.toList());
        if(x < 15) {
            view.triplet_list_view_with_header(displayable, headers);
            waitForInput();
        }
        else {
            List<String> strings = new ArrayList<>();
            list.subList(0,x).forEach(p -> {
                strings.add(p.getValue0().getCode());
                strings.add(String.valueOf(p.getValue1()));
                strings.add(String.valueOf(p.getValue2()));
            });
            inNavigator(strings, new ArrayList<>(Arrays.asList("Product", "Distinct customers", "Amount of sales")));
        }

        /* If saveToFile is on, re-run the same view with the redirect */
        if(model.saveToFile()) {
            redirectOutToSaveFile(6, Integer.toString(x));
            view.triplet_list_view_with_header(displayable, headers);
            destroyRedirectOut();
        }
    }

    private void query7_3BiggestSpendersPerBranch() throws IOException {
        /* Start Timing */
        QueryTime.start();

        /* Get the data from model and sort it as we wish */
        List<Map<ICustomer, Float>> total = model.totalSpentByCustomers();

        List<List<Pair<ICustomer, Float>>> list = total.stream().map(m -> m.entrySet().stream().sorted(Map.Entry.comparingByValue()).
                map(e -> new Pair<>(e.getKey().clone(), e.getValue())).collect(Collectors.toList())).
                collect(Collectors.toList());

        /* Reverse and sublist */
        list.forEach(Collections::reverse);
        list = list.stream().map(l -> l.subList(0, 3)).collect(Collectors.toList());

        /* Finish Timing and show the user the time elapsed */
        view.screenInfo("Elapsed time: " + QueryTime.stop() + "\n");
        waitForInput();

        /* Reduce the lists and send them to the view */
        view.perBranchTable(list);
        waitForInput();

        /* If saveToFile is on, re-run the same view with the redirect */
        if(model.saveToFile()) {
            redirectOutToSaveFile(7);
            view.perBranchTable(list);
            destroyRedirectOut();
        }
    }

    private void query8_BiggestBuyersOfDifferentProducts() throws OutOfBoundsException, IOException {
        /* Call the int view form */
        int x;
        view.screenInfo("How many?\n");
        try {
            x = scanner.nextInt();
        }
        catch (NumberFormatException e) {
            view.displayError("Must insert a number!");
            waitForInput();
            return;
        }
        /* Throw exception if the number isn't valid */
        if(x > model.number_of_products() || x < 0) throw new OutOfBoundsException();

        /* Start Timing */
        QueryTime.start();

        /* Get the data from the model */
        List<Pair<Object, Object>> info = model.differentProductsBoughtByCustomers().entrySet().stream().sorted(Map.Entry.comparingByValue()).map(e -> new Pair<>((Object) e.getKey().clone(), (Object) e.getValue())).collect(Collectors.toList());

        /* Reverse the list */
        Collections.reverse(info);

        /* Finish Timing and show the user the time elapsed */
        view.screenInfo("Elapsed time: " + QueryTime.stop() + "\n");
        waitForInput();

        /* Pass info to the view */
        String[] headers = {"Customer", "Total"};
        if(x < 15) {
            view.pair_list_view_with_header(info, headers);
            waitForInput();
        }
        else {
            List<String> strings = new ArrayList<>();
            info.subList(0,x).forEach(p -> {
                strings.add(String.valueOf(p.getValue0()));
                strings.add(String.valueOf(p.getValue1()));
            });
            inNavigator(strings, new ArrayList<>(Arrays.asList("Customer", "Total")));
        }

        /* If saveToFile is on, re-run the same view with the redirect */
        if(model.saveToFile()) {
            redirectOutToSaveFile(8, Integer.toString(x));
            view.pair_list_view_with_header(info, headers);
            destroyRedirectOut();
        }
    }

    private void query9_biggestBuyersOfCertainProduct() throws InvalidProductException, OutOfBoundsException, IOException {
        /* Call the product view form */
        view.screenInfo("What product?\n");
        String product = scanner.nextLine();

        /* Throw exception if the code isn't valid */
        if(!parser.validProduct(product) || !model.existsInProductCatalog(product)) throw new InvalidProductException();

        int x;
        view.screenInfo("How many?\n");
        try {
            x = scanner.nextInt();
        }
        catch (NumberFormatException e) {
            view.displayError("Must insert a number!");
            waitForInput();
            return;
        }

        /* Throw exception if the number isn't valid */
        if(x > model.number_of_products() || x < 0) throw new OutOfBoundsException();

        /* Start Timing */
        QueryTime.start();

        /* Get the data from the model */
        Map<ICustomer, Pair<Integer, Float>> map = model.buyersOfCertainProduct(product);

        /* Sort the data and pass it to a list */
        Comparator<Map.Entry<ICustomer, Pair<Integer, Float>>> byTotalPurchases = Comparator.comparing(e -> e.getValue().getValue0());
        Comparator<Map.Entry<ICustomer, Pair<Integer, Float>>> alphabetically = Comparator.comparing(e -> e.getKey().getCode());
        List<Triplet<Object, Object, Object>> mostPurchased = map.entrySet().stream().sorted(byTotalPurchases.reversed().thenComparing(alphabetically.reversed())).map(e -> new Triplet<>((Object) e.getKey().clone(), (Object) e.getValue().getValue0(), (Object) e.getValue().getValue1())).collect(Collectors.toList());

        /* Sub the list */
        if(x > mostPurchased.size()) x = mostPurchased.size();

        /* Finish Timing and show the user the time elapsed */
        view.screenInfo("Elapsed time: " + QueryTime.stop() + "\n");
        waitForInput();

        /* Send it to the view */
        String[] headers = {"Customer", "Quantity", "Total spent"};
        if(x < 15) {
            view.triplet_list_view_with_header(mostPurchased, headers);
            waitForInput();
        }
        else {
            List<String> strings = new ArrayList<>();
            mostPurchased.subList(0,x).forEach(p -> {
                strings.add(String.valueOf(p.getValue0()));
                strings.add(String.valueOf(p.getValue1()));
                strings.add(String.valueOf(p.getValue2()));
            });
            inNavigator(strings, new ArrayList<>(Arrays.asList("Customer", "Quantity", "Total spent")));
        }

        /* If saveToFile is on, re-run the same view with the redirect */
        if(model.saveToFile()) {
            redirectOutToSaveFile(9, product + "_" + x);
            view.triplet_list_view_with_header(mostPurchased, headers);
            destroyRedirectOut();
        }
    }

    private void query10_productsBilling(){
        /* Start Timing */
        QueryTime.start();

        /* Get data from model */
        Map<IProduct, Float[][]> allProductsBilling = model.allProductsBilling();

        /* Finish Timing and show the user the time elapsed */
        view.screenInfo("Elapsed time: " + QueryTime.stop() + "\n");
        waitForInput();

        /* Turn the data obtained into navigate able data */
        List<String> months = new ArrayList<>(Arrays.asList("Jan", "Feb", "Mar", "Apr", "May", "June", "July", "Aug", "Sept", "Oct", "Nov", "Dec"));
        ArrayList<List<String>> multipleHeaders = new ArrayList<>();
        List<String> content = new ArrayList<>();
        for(Map.Entry<IProduct, Float[][]> entry : allProductsBilling.entrySet()){
            IProduct product = entry.getKey();
            /* Add the product to the header */
            List<String> header = new ArrayList<>(months);
            header.add(0, product.getCode());
            multipleHeaders.add(header);
            /* Go over the array and start creating the content list */
            Float[][] array = entry.getValue();
            for(int j = 0; j < array[0].length; j++){
                content.add("Branch" + (j+1));
                for(int i = 0; i < array.length; i++){
                    content.add(String.valueOf(array[i][j]));
                }
            }
        }
        inNavigator_MultipleHeaders(content, multipleHeaders);

        /* If saveToFile is on, re-run the same view with the redirect */
        /*if(model.saveToFile()) {
            redirectOutToSaveFile(10);
            view.query10NoNav(allProductsBilling);
            destroyRedirectOut();
        }*/
    }

    private void query11_lastFileInfo() throws IOException {
        String logged_q11_path = model.getConfigs().getLogPath();
        if(!model.isInitialized()){
            Files.lines(Paths.get(logged_q11_path)).forEach(System.out::println);
        } else {
            /* Start Timing */
            QueryTime.start();

            IStatistics statistics = model.getStatistics();
            int differentProductsBought = model.countDifferentProductsBought();
            int number_of_products = model.number_of_products();
            int number_of_customers = model.number_of_customers();
            int customersWhoBought = model.countCustomersWhoBought();
            int zeroValueSales = statistics.getZeroValueSales();
            float total_billing = model.totalBilling();

            /* Finish Timing and show the user the time elapsed */
            view.screenInfo("Elapsed time: " + QueryTime.stop() + "\n");
            waitForInput();

            String[] identifiers = {"File name", "Invalid sales", "Total of products", "Total of different products bought", "Total of different products not bought", "Total of customers",
                                    "Total of customers who purchased something", "Total of customers who purchased nothing", "Total of zero value sales", "Total billing"};
            Object[] data = {statistics.getSales_file_name(), statistics.getTotal_sales() - statistics.getValid_sales(), number_of_products, differentProductsBought,
                             number_of_products - differentProductsBought, number_of_customers, customersWhoBought, number_of_customers - customersWhoBought,
                             zeroValueSales, total_billing};
            view.displayColumns(identifiers, data);
        }
        waitForInput();
    }

    private void query12_currentModelInfo() throws IOException {
        /* Start Timing */
        QueryTime.start();

        int counter = 0;
        Integer[] salesPerMonth = model.totalSalesPerMonth();
        Pair<Float, List<Float[]>> pair = model.totalAndBillingByMonthByBranch();
        List<Float[]> list = pair.getValue1();
        List<Integer[]> distinctCustomers = model.amountOfDistinctCustomersByMonth();
        int nBranches = model.getConfigs().getNumber_of_branches();

        /* Finish Timing and show the user the time elapsed */
        view.screenInfo("Elapsed time: " + QueryTime.stop() + "\n");
        waitForInput();

        while(counter != 2) {
            /* Total purchases per month */
            view.displayPerMonthInfo(Arrays.asList(salesPerMonth), "Purchases per month:");
            /* Total profit per month per branch */
            Float[][] billingMonthBranch = new Float[nBranches][list.size()];
            String[] identifiers = new String[nBranches];
            for(int i = 0; i < nBranches; i++) {
                billingMonthBranch[i] = list.get(i);
                identifiers[i] = "Branch" + (i+1);
            }
            view.screenInfo("Total profit: " + pair.getValue0() + "\n");
            view.displayPerBranchAndPerMonthInfo(billingMonthBranch, identifiers);
            /* Amount of distinct customers per month per branch */
            Integer[][] customersMonthBranch = new Integer[nBranches][distinctCustomers.size()];
            for(int i = 0; i < nBranches; i++) {
                customersMonthBranch[i] = distinctCustomers.get(i);
                identifiers[i] = "Branch" + (i+1);
            }
            view.displayPerBranchAndPerMonthInfo(customersMonthBranch, identifiers);
            if(counter == 0) waitForInput();
            counter++;
            if(model.saveToFile() && counter == 1)  {
                redirectOutToSaveFile(12);
            } else if(!model.saveToFile()) {
                break;
            } else destroyRedirectOut();
        }
    }

    @Override
    public void startController(){
        /* Grab the option with the menu form */
        int option = -1;
        view.firstMenuForm();
        try {
            option = Integer.parseInt(scanner.nextLine());
        }
        catch (NumberFormatException e) {
            System.out.println("The Inserted Value Must be an Integer!");
        }
        while (option != 0) {
            if (!model.isInitialized() && option <= 13 && option != 11) {
                view.displayError("You need to load some data before doing any query!");
                waitForInput();
            } else switch (option) {
                case -1:
                    break;
                case 1:
                    try {
                        query1_allNotBoughtProducts();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                    break;
                case 2:
                    try {
                        query2_globalSalesOnMonthInBranch();
                    } catch (InvalidMonthException e) {
                        view.displayError(e.getMessage());
                        waitForInput();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                    break;
                case 3:
                    try {
                        query3_infoAboutCustomer();
                    } catch (InvalidCustomerException e) {
                        view.displayError(e.getMessage());
                        waitForInput();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                    break;
                case 4:
                    try {
                        query4_productTotalMadeAndTotalCustomers();
                    } catch (InvalidProductException e) {
                        view.displayError(e.getMessage());
                        waitForInput();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                    break;
                case 5:
                    try {
                        query5_productsBoughtByCustomer();
                    } catch (InvalidCustomerException e) {
                        view.displayError(e.getMessage());
                        waitForInput();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                    break;
                case 6:
                    try {
                        query6_XProductsMostSold();
                    } catch (OutOfBoundsException e) {
                        view.displayError(e.getMessage());
                        waitForInput();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                    break;
                case 7:
                    try {
                        query7_3BiggestSpendersPerBranch();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                    break;
                case 8:
                    try {
                        query8_BiggestBuyersOfDifferentProducts();
                    } catch (OutOfBoundsException e) {
                        view.displayError(e.getMessage());
                        waitForInput();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                    break;
                case 9:
                    try {
                        query9_biggestBuyersOfCertainProduct();
                    } catch (InvalidProductException | OutOfBoundsException e) {
                        view.displayError(e.getMessage());
                        waitForInput();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                    break;
                case 10:
                    query10_productsBilling();
                    break;
                case 11:
                    try {
                        query11_lastFileInfo();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                    break;
                case 12:
                    try {
                        query12_currentModelInfo();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                    break;
                case 13:
                    try {
                        serializeModel();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                    break;
                case 14:
                    try {
                        deserializeModel();
                    } catch (IOException | ClassNotFoundException e) {
                        e.printStackTrace();
                    }
                    break;
                case 15:
                        loadFilesThroughPaths();
                    break;
                case 16:
                    try {
                        loadDefaultFiles();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                    break;
                default:
                    view.displayError("Invalid option!");
                    waitForInput();
                    break;
            }
            view.clearScreen();
            view.menuForm();
            option = -1;
            try {
                option = Integer.parseInt(scanner.nextLine());
            }
            catch (NumberFormatException e) {
                view.displayError("Inserted Value Must be an Integer!");
                waitForInput();
            }
        }
    }
}

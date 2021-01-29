package model;

import model.base.*;
import model.billing.Billing;
import model.billing.IBilling;
import model.branches.Branch;
import model.branches.IBranch;
import model.branches.ICustomerBought;
import model.catalogs.*;
import model.configs_n_stats.Configs;
import model.configs_n_stats.IConfigs;
import model.configs_n_stats.IStatistics;
import model.configs_n_stats.Statistics;
import org.apache.commons.lang3.tuple.ImmutablePair;
import org.javatuples.Pair;
import org.javatuples.Triplet;

import java.io.*;
import java.util.*;
import java.util.stream.Collectors;

public class GestVendasModel implements IGestVendasModel, Serializable {
    private IBranch[] branches;
    private IBilling billing;
    private IProductCatalog productCatalog;
    private ICustomerCatalog customerCatalog;
    private IStatistics statistics;
    private IConfigs configs;
    private boolean saveToFile;
    private boolean initialized;

    public GestVendasModel(IConfigs configs){
        this.configs = configs;
        this.branches = new IBranch[configs.getNumber_of_branches()];
        for(int i = 0; i < configs.getNumber_of_branches(); i++) this.branches[i] = new Branch(configs.getNumber_of_months());
        this.billing = new Billing(configs.getNumber_of_branches(), configs.getNumber_of_months(), configs.getNumber_of_types());
        this.productCatalog = new ProductCatalog();
        this.customerCatalog = new CustomerCatalog();
        this.statistics = new Statistics();
        saveToFile = false;
        initialized = false;
    }

    public GestVendasModel(IGestVendasModel model){
        this.configs = model.getConfigs();
        this.branches = new IBranch[configs.getNumber_of_branches()];
        for(int i = 0; i < configs.getNumber_of_branches(); i++) this.branches[i] = new Branch(configs.getNumber_of_months());
        this.billing = new Billing(configs.getNumber_of_branches(), configs.getNumber_of_months(), configs.getNumber_of_types());
        this.productCatalog = new ProductCatalog();
        this.customerCatalog = new CustomerCatalog();
        this.statistics = new Statistics();
        saveToFile = model.saveToFile();
        initialized = model.isInitialized();
    }

    /* -------------------------------------------------  QUERIES ----------------------------------------------------- */

    @Override
    public List<IProduct> allNotBoughtProducts(){
        return this.billing.allNotBoughtProducts();
    }

    @Override
    public List<ImmutablePair<Integer,Integer>> globalSalesOnMonthInBranch(int month){
        List<ImmutablePair<Integer, Integer>> list = new ArrayList<>(configs.getNumber_of_branches() + 1);
        /* For each branch find out the total number of sales and the number of distinct customers */
        for(int i = 1; i <= configs.getNumber_of_branches(); i++){
            list.add(new ImmutablePair<>(branches[i-1].getDistinctCustomers(month), billing.numberOfSalesInBranchAndMonth(i, month)));
        }
        /* Find the global amounts too */
        Set<ICustomer> customers = new HashSet<>();
        Arrays.stream(branches).forEach(b -> customers.addAll(b.distinctCustomerSetInMonth(month)));
        list.add(new ImmutablePair<>(customers.size(), billing.monthlyTotalOfSales(month)));
        return list;
    }

    @Override
    public List<Triplet<Integer, Integer, Float>> infoAboutCustomer(String customer){
        ICustomer c = new Customer(customer);
        List<Triplet<Integer, Integer, Float>> ret = new ArrayList<>();
        List<Triplet<Integer[], Integer[], Float[]>> results =  new ArrayList<>();

        /* Get the info of each branch */
        for(int i = 0; i < configs.getNumber_of_branches(); i++) results.add(branches[i].infoAboutCustomer(c));

        /* Fold it together */
        for(int i = 0; i < configs.getNumber_of_months(); i++){
            int finalI = i;
            Integer m1 = results.stream().map(Triplet::getValue0).map(l -> l[finalI]).reduce(0, Integer::sum);
            Integer m2 = results.stream().map(Triplet::getValue1).map(l -> l[finalI]).reduce(0, Integer::sum);
            Float m3 = results.stream().map(Triplet::getValue2).map(l -> l[finalI]).reduce((float) 0, Float::sum);
            ret.add(new Triplet<>(m1, m2, m3));
        }
        return ret;
    }

    @Override
    public List<Triplet<Integer, Integer, Float>> productTotalMadeAndTotalCustomers(String product){
        IProduct p = new Product(product);
        List<Triplet<Integer, Integer, Float>> ret = new ArrayList<>();
        List<Triplet<Integer[], Integer[], Float[]>> results =  new ArrayList<>();

        /* Get the info of each branch */
        for(int i = 0; i < configs.getNumber_of_branches(); i++) results.add(branches[i].productTotalMadeAndTotalCustomers(p));

        /* Fold it together */
        for(int i = 0; i < configs.getNumber_of_months(); i++){
            int finalI = i;
            Integer m1 = results.stream().map(Triplet::getValue0).map(l -> l[finalI]).reduce(0, Integer::sum);
            Integer m2 = results.stream().map(Triplet::getValue1).map(l -> l[finalI]).reduce(0, Integer::sum);
            Float m3 = results.stream().map(Triplet::getValue2).map(l -> l[finalI]).reduce((float) 0, Float::sum);
            ret.add(new Triplet<>(m1, m2, m3));
        }
        return ret;
    }

    /**
     * Query 5
     * @param customer
     * @return
     */
    @Override
    public  Map<IProduct, ICustomerBought> productsBoughtByCustomer(String customer){
        ICustomer c = new Customer(customer);
        /* Get the products bought in each branch */
        Map<IProduct, ICustomerBought> productsBought = new HashMap<>(branches[0].productsBoughtByCustomer(c));
        for(int i = 1; i < configs.getNumber_of_branches(); i++) {
            branches[i].productsBoughtByCustomer(c).values().forEach(customerBought -> {
                if (productsBought.containsKey(customerBought.getProduct())) {
                    productsBought.get(customerBought.getProduct()).accumulateCbOnCustomerBought(customerBought);
                } else productsBought.put(customerBought.getProduct(), customerBought.clone());
            });
        }

        return productsBought;
    }

    /**
     * Query 6
     * @return
     */
    @Override
    public Map<IProduct, Pair<Integer, Integer>> salesAndDistinctCustomersPerProduct(){
        /* Get the products info in each branch */
        Map<IProduct, Pair<Set<ICustomer>, Integer>> productPairMap =  new HashMap<>(branches[0].salesAndDistinctCustomersPerProduct());
        for(int i = 1; i < configs.getNumber_of_branches(); i++){
            branches[i].salesAndDistinctCustomersPerProduct().forEach((key, value) -> {
                if (productPairMap.containsKey(key)) {
                    Pair<Set<ICustomer>, Integer> pair = productPairMap.get(key);
                    pair.getValue0().addAll(value.getValue0());
                    productPairMap.put(key.clone(), new Pair<>(pair.getValue0(), value.getValue1() + pair.getValue1()));
                } else productPairMap.put(key.clone(), new Pair<>(value.getValue0(), value.getValue1()));
            });
        }

        /* Transform the sets into their size */
        return productPairMap.entrySet().stream().collect(Collectors.toMap(e -> e.getKey().clone(), e -> new Pair<>(e.getValue().getValue0().size(), e.getValue().getValue1())));
    }

    /**
     * Query 7
     * @return
     */
    @Override
    public List<Map<ICustomer, Float>> totalSpentByCustomers(){
        /* Get the info and fold it together */
        List<Map<ICustomer, Float>> total = new ArrayList<>();
        for(int i = 0; i < configs.getNumber_of_branches(); i++) total.add(branches[i].totalSpentByCustomers());
        return total;
    }

    /**
     * Query 8
     * @return
     */
    @Override
    public Map<ICustomer, Integer> differentProductsBoughtByCustomers(){
        /* Get the info and fold it together */
        Map<ICustomer, Set<IProduct>> ret = new HashMap<>(branches[0].differentProductsBoughtByCustomers());
        for(int i = 1; i < configs.getNumber_of_branches(); i++) {
                branches[i].differentProductsBoughtByCustomers().forEach((key, value) -> {
                    if(ret.containsKey(key)){
                        Set<IProduct> set = new HashSet<>(ret.get(key));
                        set.addAll(value);
                        ret.put(key.clone(), set);
                    } else ret.put(key.clone(), value);
                });
        }
        return ret.entrySet().stream().collect(Collectors.toMap(e -> e.getKey().clone(), e -> e.getValue().size()));
    }

    /**
     * Query 9
     * @param product
     * @return
     */
    @Override
    public Map<ICustomer, Pair<Integer, Float>> buyersOfCertainProduct(String product){
        IProduct p = new Product(product);
        /* Get the info and fold it together */
        Map<ICustomer, Pair<Integer, Float>> ret = new HashMap<>(branches[0].buyersOfCertainProduct(p));
        for(int i = 1; i < configs.getNumber_of_branches(); i++){
            branches[i].buyersOfCertainProduct(p).forEach((key, value) -> {
                if(ret.containsKey(key)){
                    ret.put(key, new Pair<>(ret.get(key).getValue0() + value.getValue0(), ret.get(key).getValue1() + value.getValue1()));
                } else ret.put(key.clone(), new Pair<>(value.getValue0(), value.getValue1()));
            });
        }
        return ret;
    }

    @Override
    public Map<IProduct, Float[][]> allProductsBilling(){
        return billing.allProductsBilling();
    }

    /* -------------------------------------------------  QUERIES ----------------------------------------------------- */

    @Override
    public void setInitialized(){
        initialized = true;
    }

    @Override
    public boolean isInitialized(){
        return initialized;
    }

    @Override
    public void setSaveToFile(){
        saveToFile = true;
    }

    @Override
    public boolean saveToFile(){
        return saveToFile;
    }

    @Override
    public void setSales_file_name(String sales_file_name){
        statistics.setSales_file_name(sales_file_name);
    }

    @Override
    public int countDifferentProductsBought(){
        return billing.countDifferentProductsBought();
    }

    @Override
    public Float totalBilling(){
        return billing.totalBilling();
    }

    @Override
    public Pair<Float, List<Float[]>> totalAndBillingByMonthByBranch(){
        return new Pair<>(billing.totalBilling(), billing.totalBillingByMonthByBranch());
    }

    @Override
    public List<Integer[]> amountOfDistinctCustomersByMonth(){
        List<Integer[]> list = new ArrayList<>();
        Arrays.stream(branches).forEach(b -> list.add(b.amountOfDistinctCustomersByMonth()));
        return list;
    }

    @Override
    public Integer[] totalSalesPerMonth(){
        return billing.totalSalesPerMonth();
    }

    @Override
    public int zeroValueSales(){
        return billing.zeroValueSales();
    }

    @Override
    public int countCustomersWhoBought(){
        Set<ICustomer> customers = new HashSet<>();
        for(int i = 0; i < configs.getNumber_of_branches(); i++) branches[i].customersWhoBought().forEach(customer -> customers.add(customer.clone()));
        return customers.size();
    }

    @Override
    public int number_of_products(){
        return this.productCatalog.number_of_products();
    }

    @Override
    public int number_of_customers(){
        return customerCatalog.number_of_customers();
    }

    @Override
    public void incrementTotal_sales(){
        statistics.incrementTotal_sales();
    }

    @Override
    public void incrementValid_sales(){
        statistics.incrementValid_sales();
    }

    @Override
    public IStatistics getStatistics(){
        return statistics.clone();
    }

    @Override
    public boolean existsInProductCatalog(String product) {
        return this.productCatalog.existsInCatalog(new Product(product));
    }

    @Override
    public boolean existsInCustomerCatalog(String customer) {
        return this.customerCatalog.existsInCatalog(new Customer(customer));
    }

    @Override
    public void addEmptyProductToFac(String product) {
        this.billing.addEmptyProductToFac(new Product(product));
    }

    @Override
    public void setZeroValueSales(int zero_value_sales) {
        this.statistics.setZeroValueSales(zero_value_sales);
    }

    @Override
    public void addToProductCatalog(String product) {
        this.productCatalog.addToCatalog(new Product(product));
    }

    @Override
    public void addToCustomerCatalog(String customer) {
        this.customerCatalog.addToCatalog(new Customer(customer));
    }

    @Override
    public void addEmptyCustomerToAllBranches(String customer) {
        Arrays.stream(branches).forEach(b -> b.addEmptyCustomer(new Customer(customer)));
    }

    @Override
    public void billProduct(ISale sale) {
        this.billing.billProduct(sale);
    }

    @Override
    public void customerBoughtInBranch(ISale s) {
        branches[s.getFilial()-1].customerBought(s);
    }

    /* ----------------------------- SERIALIZING ---------------------------------------------- */

    @Override
    public void saveState(String fileName) throws IOException {
        FileOutputStream fos = new FileOutputStream(fileName);
        ObjectOutputStream oos = new ObjectOutputStream(fos);
        oos.writeObject(this);
        oos.flush();
        oos.close();
    }

    @Override
    public IGestVendasModel loadState(String fileName) throws IOException, ClassNotFoundException {
        FileInputStream fis = new FileInputStream(fileName);
        ObjectInputStream ois = new ObjectInputStream(fis);
        GestVendasModel gvm = (GestVendasModel) ois.readObject();
        ois.close();
        return gvm;
    }
    /* ----------------------------- SERIALIZING ---------------------------------------------- */

    @Override
    public IConfigs getConfigs(){
        return this.configs.clone();
    }
}

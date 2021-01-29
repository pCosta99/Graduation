package model.branches;

import model.base.ICustomer;
import model.base.IProduct;
import model.base.ISale;
import org.javatuples.Pair;
import org.javatuples.Triplet;

import java.io.Serializable;
import java.util.*;
import java.util.stream.Collectors;

public class Branch implements IBranch, Serializable {
    /* map<key -> letters | value -> map<key -> numbers, value -> map<key -> product, value -> CustomerBought>>> */
    private Map<ICustomer, IProductsBought> branch;
    private int[] distinctCustomers;
    private int number_of_months;

    public Branch(int number_of_months){
        this.branch = new HashMap<>();
        this.number_of_months = number_of_months;
        this.distinctCustomers = new int[number_of_months];
    }

    public Branch(Branch b){
        this.branch = b.getBranch();
        this.distinctCustomers = b.getDistinctCustomers();
    }

    private Map<ICustomer, IProductsBought> getBranch(){
        return branch.entrySet().stream().collect(Collectors.toMap(e -> e.getKey().clone(), e -> e.getValue().clone()));
    }


    @Override
    public void addEmptyCustomer(ICustomer customer) {
        this.branch.put(customer.clone(), new ProductsBought(number_of_months));
    }

    @Override
    public void customerBought(ISale sale) {
        IProductsBought pb = branch.get(sale.getCustomer());
        pb.customerBought(sale);
        if(pb.getPurchasesInMonth(sale.getMonth()) - 1 == 0) distinctCustomers[sale.getMonth()-1]++;
    }

    @Override
    public int[] getDistinctCustomers() {
        return distinctCustomers.clone();
    }

    @Override
    public int getDistinctCustomers(int month){
        return distinctCustomers[month-1];
    }

    @Override
    public boolean didCustomerBuy(ICustomer customer) {
        return this.branch.get(customer).didCustomerBuy();
    }

    @Override
    public ICustomerBought getCustomerBought(ICustomer customer, IProduct product) {
        return this.branch.get(customer).getCustomerBought(product);
    }

    @Override
    public Set<ICustomer> customersWhoBought(){
        return branch.entrySet().stream().filter(e -> e.getValue().didCustomerBuy()).map(Map.Entry::getKey).map(ICustomer::clone).collect(Collectors.toSet());
    }

    @Override
    public Integer[] amountOfDistinctCustomersByMonth(){
        Integer[] array = new Integer[number_of_months];
        Arrays.fill(array, 0);
        branch.values().forEach(p -> p.amountOfDistinctCustomersByMonth(array));
        return array;
    }

    @Override
    public Set<ICustomer> distinctCustomerSetInMonth(int month){
        return branch.entrySet().stream().filter(e -> e.getValue().boughtInMonth(month)).map(e -> e.getKey().clone()).collect(Collectors.toSet());
    }

    @Override
    public Triplet<Integer[], Integer[], Float[]> infoAboutCustomer(ICustomer customer){
        return branch.get(customer).foldInfo();
    }

    @Override
    public Triplet<Integer[], Integer[], Float[]> productTotalMadeAndTotalCustomers(IProduct product){
        /* Start the arrays */
        Integer[] amountPurchased = new Integer[number_of_months];
        Arrays.fill(amountPurchased, 0);
        Integer[] howManyCustomers = new Integer[number_of_months];
        Arrays.fill(howManyCustomers, 0);
        Float[] totalProfit = new Float[number_of_months];
        Arrays.fill(totalProfit, (float) 0);

        /* Go through the map and fold it */
        branch.values().forEach(e -> e.productTotalMadeAndTotalCustomers(product, howManyCustomers, amountPurchased, totalProfit));

        return new Triplet<>(amountPurchased, howManyCustomers, totalProfit);
    }

    @Override
    public Map<IProduct, ICustomerBought> productsBoughtByCustomer(ICustomer customer){
        return branch.get(customer).getProductsBought();
    }

    @Override
    public Map<IProduct, Pair<Set<ICustomer>, Integer>> salesAndDistinctCustomersPerProduct(){
        Map<IProduct, Pair<Set<ICustomer>, Integer>> ret = new HashMap<>();

        branch.forEach((key, value) -> value.salesPerProduct(ret, key));

        return ret;
    }

    @Override
    public Map<ICustomer, Float> totalSpentByCustomers(){
        return branch.entrySet().stream().collect(Collectors.toMap(e -> e.getKey().clone(), e -> (float) e.getValue().totalSpent()));
    }

    @Override
    public Map<ICustomer, Set<IProduct>> differentProductsBoughtByCustomers(){
        Map<ICustomer, Set<IProduct>> map = new HashMap<>();
        for(Map.Entry<ICustomer, IProductsBought> entry : branch.entrySet()){
            map.put(entry.getKey().clone(), entry.getValue().getProductsKeys());
        }
        return map;
    }

    @Override
    public Map<ICustomer, Pair<Integer, Float>> buyersOfCertainProduct(IProduct product){
        return branch.entrySet().stream().filter(e -> e.getValue().didCustomerBuyProduct(product)).collect(Collectors.toMap(e -> e.getKey().clone(), e -> e.getValue().amountAndTotalSpentOfProduct(product)));
    }

    @Override
    public Branch clone(){
        return new Branch(this);
    }
}

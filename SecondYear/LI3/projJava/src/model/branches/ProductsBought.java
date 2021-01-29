package model.branches;

import model.base.ICustomer;
import model.base.IProduct;
import model.base.ISale;
import org.javatuples.Pair;
import org.javatuples.Triplet;

import java.io.Serializable;
import java.util.*;
import java.util.stream.Collectors;

public class ProductsBought implements IProductsBought, Serializable {
    private Map<IProduct, ICustomerBought> productsBought;
    private int[] monthlyPurchases;
    private int number_of_months;
    private float totalSpent;

    public ProductsBought(int number_of_months){
        productsBought = new HashMap<>();
        this.number_of_months = number_of_months;
        monthlyPurchases = new int[number_of_months];
        totalSpent = 0;
    }

    public ProductsBought(ProductsBought productsBought){
        this.productsBought = productsBought.getProductsBought();
        this.monthlyPurchases = productsBought.getMonthlyPurchases();
    }

    public int[] getMonthlyPurchases() {
        return monthlyPurchases.clone();
    }

    @Override
    public int getPurchasesInMonth(int month){
        return monthlyPurchases[month-1];
    }

    @Override
    public Map<IProduct, ICustomerBought> getProductsBought() {
        return productsBought.entrySet().stream().collect(Collectors.toMap(e -> e.getKey().clone(), e -> e.getValue().clone()));
    }

    @Override
    public void customerBought(ISale sale){
        if(!productsBought.containsKey(sale.getProduct())){
            productsBought.put(sale.getProduct().clone(), new CustomerBought(sale.getProduct(), number_of_months));
        }
        productsBought.get(sale.getProduct()).accumulateSaleOnCustomerBought(sale);
        monthlyPurchases[sale.getMonth()-1]++;
        totalSpent += (sale.getAmount() * sale.getPrice());
    }

    @Override
    public int differentProductsBought(){
        return productsBought.size();
    }

    @Override
    public Set<IProduct> products(){
        return productsBought.keySet();
    }

    @Override
    public float totalSpent(){
        return totalSpent;
    }

    @Override
    public boolean didCustomerBuy() {
        return productsBought.size() > 0;
    }

    @Override
    public ICustomerBought getCustomerBought(IProduct product) {
        return productsBought.get(product).clone();
    }

    @Override
    public Pair<Integer, Float> amountAndTotalSpentOfProduct(IProduct product){
        ICustomerBought cb = productsBought.get(product);
        return new Pair<>(cb.getNCompras(), cb.getTotalSpent());
    }

    @Override
    public boolean didCustomerBuyProduct(IProduct product){
        return productsBought.containsKey(product);
    }

    @Override
    public void salesPerProduct(Map<IProduct, Pair<Set<ICustomer>, Integer>> info, ICustomer customer){
        productsBought.forEach((key, value) -> {
            if(info.containsKey(key)){
                Set<ICustomer> customerSet = info.get(key).getValue0();
                customerSet.add(customer.clone());
                info.put(key.clone(), new Pair<>(customerSet, info.get(key).getValue1() + value.getNCompras()));
            } else {
                Set<ICustomer> customerSet = new HashSet<>();
                customerSet.add(customer.clone());
                info.put(key.clone(), new Pair<>(customerSet, value.getNCompras()));
            }
        });
    }

    @Override
    public void productTotalMadeAndTotalCustomers(IProduct product, Integer[] howManyCustomers, Integer[] amountPurchased, Float[] totalProfit){
        if(productsBought.containsKey(product)){
            ICustomerBought cb = productsBought.get(product);
            for(int i = 1; i <= number_of_months; i++){
                if(cb.wasSoldInMonth(i)) howManyCustomers[i-1]++;
                amountPurchased[i-1] += cb.getNComprasInMonth(i);
                totalProfit[i-1] += cb.getTotalSpentInMonth(i);
            }
        }
    }

    @Override
    public Triplet<Integer[], Integer[], Float[]> foldInfo(){
        /* Initialize the lists */
        Integer[] totalPurchases = new Integer[number_of_months];
        Arrays.fill(totalPurchases, 0);
        Integer[] totalProductsBought = new Integer[number_of_months];
        Arrays.fill(totalProductsBought, 0);
        Float[] totalSpent = new Float[number_of_months];
        Arrays.fill(totalSpent, (float) 0);

        /* Fill it with the obtained info */
        productsBought.values().forEach(cb -> {
            for(int i = 0; i < number_of_months; i++) {
                totalPurchases[i] +=  cb.getNComprasInMonth(i+1);
                totalSpent[i] += cb.getTotalSpentInMonth(i+1);
                if(cb.wasSoldInMonth(i+1)) totalProductsBought[i]++;
            }
        });

        return new Triplet<>(totalPurchases, totalProductsBought, totalSpent);
    }

    @Override
    public void amountOfDistinctCustomersByMonth(Integer[] distinctCustomersByMonth){
        for(int i = 0; i < number_of_months; i++){
            int finalI = i;
            if(productsBought.values().stream().anyMatch(cb -> cb.wasSoldInMonth(finalI +1))) distinctCustomersByMonth[i]++;
        }
    }

    @Override
    public boolean boughtInMonth(int month) {
        return monthlyPurchases[month-1] > 0;
    }

    @Override
    public IProductsBought clone(){
        return new ProductsBought(this);
    }

    @Override
    public Set<IProduct> getProductsKeys() {
        return productsBought.keySet().stream().map(IProduct::clone).collect(Collectors.toSet());
    }
}

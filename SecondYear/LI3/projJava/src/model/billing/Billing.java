package model.billing;

import model.base.IProduct;
import model.base.ISale;

import java.io.Serializable;
import java.util.*;
import java.util.stream.Collectors;

public class Billing implements IBilling, Serializable {
    private Map<IProduct, IProductInfo> billing;
    private float totalBilled;
    private int[][] globalNumberOfSales;
    private int number_of_branches;
    private int number_of_months;
    private int number_of_types;

    public Billing(int number_of_branches, int number_of_months, int number_of_types){
        this.billing = new HashMap<>();
        totalBilled = 0;
        this.number_of_branches = number_of_branches;
        this.number_of_months = number_of_months;
        this.number_of_types = number_of_types;
        this.globalNumberOfSales = new int[number_of_branches][number_of_months];
    }

    public Billing(Billing b){
        this.billing = b.getBilling();
        this.globalNumberOfSales = b.salesByMonthNBranch();
        totalBilled = b.totalBilling();
    }

    private Map<IProduct, IProductInfo> getBilling(){
        return this.billing.entrySet().stream().collect(Collectors.toMap(Map.Entry::getKey, e -> e.getValue().clone()));
    }

    @Override
    public List<IProduct> allNotBoughtProducts(){
        return this.billing.values().stream().filter(p -> !p.wasSold()).map(IProductInfo::getProduct).collect(Collectors.toList());
    }

    @Override
    public int countDifferentProductsBought(){
        return (int) billing.values().stream().filter(IProductInfo::wasSold).count();
    }

    @Override
    public int zeroValueSales(){
        return (int) billing.values().stream().filter(p -> p.getNumberOfSales() == 0).count();
    }

    @Override
    public Float totalBilling(){
        return totalBilled;
    }

    @Override
    public List<Float[]> totalBillingByMonthByBranch(){
        List<Float[]> list = new ArrayList<>();
        for(int i = 0; i < number_of_branches; i++) {
            list.add(new Float[number_of_months]);
            Arrays.fill(list.get(i), (float) 0);
        }
        billing.values().forEach(p -> {
            for(int i = 0; i < number_of_months; i++)
                for(int j = 0; j < number_of_branches; j++)
                    list.get(j)[i] += p.getProfitInBranch(i+1, j+1);
        });
        return list;
    }

    @Override
    public Integer[] totalSalesPerMonth(){
        Integer[] salesPerMonth = new Integer[number_of_months];
        Arrays.fill(salesPerMonth, 0);
        for(int i = 0; i < number_of_branches; i++)
            for(int j = 0; j < number_of_months; j++)
                salesPerMonth[j] += globalNumberOfSales[i][j];
        return salesPerMonth;
    }

    @Override
    public int[][] salesByMonthNBranch() {
        return globalNumberOfSales.clone();
    }

    @Override
    public int monthlyTotalOfSales(int month) {
        int total = 0;
        for(int i = 0; i < number_of_branches; i++){
            total += globalNumberOfSales[i][month-1];
        }
        return total;
    }

    @Override
    public int numberOfSalesInBranchAndMonth(int branch, int month) {
        return this.globalNumberOfSales[branch-1][month-1];
    }

    @Override
    public Map<IProduct, Float[][]> allProductsBilling(){
        return billing.entrySet().stream().collect(Collectors.toMap(e -> e.getKey().clone(), e -> e.getValue().collectProductInfo()));
    }

    @Override
    public void addEmptyProductToFac(IProduct product) {
        this.billing.put(product.clone(), new ProductInfo(product, number_of_branches, number_of_types, number_of_months));
    }

    @Override
    public void billProduct(ISale sale) {
        if(this.billing.containsKey(sale.getProduct())){
            this.billing.get(sale.getProduct()).accumulateSaleOnInfo(sale);
            globalNumberOfSales[sale.getFilial()-1][sale.getMonth()-1]++;
            totalBilled += sale.getAmount() * sale.getPrice();
        }
    }

    @Override
    public boolean wasSold(IProduct product) {
        IProductInfo pi = this.billing.get(product).clone();
        return pi.getTotal_sales()[0] + pi.getTotal_sales()[1] > 0;
    }

    @Override
    public IProductInfo getProductInfo(IProduct product) {
        return this.billing.get(product).clone();
    }

    @Override
    public Billing clone(){
        return new Billing(this);
    }
}

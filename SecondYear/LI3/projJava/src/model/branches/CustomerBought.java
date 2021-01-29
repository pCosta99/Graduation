package model.branches;

import model.base.IProduct;
import model.base.ISale;

import java.io.Serializable;
import java.util.Arrays;

public class CustomerBought implements ICustomerBought, Serializable {
    private IProduct product;
    private Integer[] n_compras_normal;
    private Integer[] n_compras_promocao;
    private Float[] total_gasto_normal;
    private Float[] total_gasto_promocao;
    private int number_of_months;

    public CustomerBought(IProduct product, int number_of_months) {
        this.product = product;
        this.number_of_months = number_of_months;
        this.n_compras_normal = new Integer[number_of_months];
        Arrays.fill(this.n_compras_normal, 0);
        this.n_compras_promocao = new Integer[number_of_months];
        Arrays.fill(this.n_compras_promocao, 0);
        this.total_gasto_normal = new Float[number_of_months];
        Arrays.fill(this.total_gasto_normal, (float) 0);
        this.total_gasto_promocao = new Float[number_of_months];
        Arrays.fill(this.total_gasto_promocao, (float) 0);
    }

    public CustomerBought(IProduct p, Integer[] compras_normal, Integer[] compras_promocao, Float[] lucros_normal, Float[] lucros_promocao) {
        this.product = p.clone();
        this.n_compras_normal = compras_normal.clone();
        this.n_compras_promocao = compras_promocao.clone();
        this.total_gasto_normal = lucros_normal.clone();
        this.total_gasto_promocao = lucros_promocao.clone();
    }

    public CustomerBought(CustomerBought c) {
        this.product = c.getProduct();
        this.n_compras_normal = c.getNComprasNormal();
        this.n_compras_promocao = c.getNComprasPromocao();
        this.total_gasto_normal = c.getTotalGastoNormal();
        this.total_gasto_promocao = c.getTotalGastoPromocao();
    }

    @Override
    public Float[] getTotalGastoNormal() {
        return this.total_gasto_normal.clone();
    }

    @Override
    public Float[] getTotalGastoPromocao() {
        return this.total_gasto_promocao.clone();
    }

    @Override
    public Integer[] getNComprasNormal() {
        return this.n_compras_normal.clone();
    }

    @Override
    public Integer[] getNComprasPromocao() {
        return this.n_compras_promocao.clone();
    }

    @Override
    public IProduct getProduct() {
        return product.clone();
    }

    @Override
    public ICustomerBought clone() {
        return new CustomerBought(this);
    }

    @Override
    public int getNCompras(boolean promocao){
        int total;

        if(promocao) total = Arrays.stream(n_compras_promocao).mapToInt(i -> i).sum();
        else total = Arrays.stream(n_compras_normal).mapToInt(i -> i).sum();

        return total;
    }

    @Override
    public int getNCompras(){
        return Arrays.stream(n_compras_normal).mapToInt(i -> i).sum() + Arrays.stream(n_compras_promocao).mapToInt(i -> i).sum();
    }

    @Override
    public int getNComprasInMonth(int month){
        return n_compras_normal[month-1] + n_compras_promocao[month-1];
    }

    @Override
    public float getTotalSpentInMonth(int month){
        return total_gasto_normal[month-1] + total_gasto_promocao[month-1];
    }

    @Override
    public boolean wasSoldInMonth(int month){
        return n_compras_normal[month - 1] + n_compras_promocao[month - 1] > 0;
    }

    @Override
    public float getTotalSpent(){
        return (float) (Arrays.stream(total_gasto_normal).mapToDouble(i -> i).sum() + Arrays.stream(total_gasto_promocao).mapToDouble(i -> i).sum());
    }

    @Override
    public void accumulateSaleOnCustomerBought(ISale sale){
        int amount = sale.getAmount();
        float profit = sale.getPrice() * amount;
        int month = sale.getMonth();

        if(sale.getType() == 'P'){
            this.n_compras_promocao[month-1] += amount;
            this.total_gasto_promocao[month-1] += profit;
        } else {
            this.n_compras_normal[month-1] += amount;
            this.total_gasto_normal[month-1] += profit;
        }
    }

    @Override
    public void accumulateCbOnCustomerBought(ICustomerBought cb){
        for(int i = 0; i < number_of_months; i++){
            n_compras_promocao[i] += cb.getNComprasPromocao()[i];
            n_compras_normal[i] += cb.getNComprasNormal()[i];
            total_gasto_promocao[i] += cb.getTotalGastoPromocao()[i];
            total_gasto_normal[i] += cb.getTotalGastoNormal()[i];
        }
    }
}

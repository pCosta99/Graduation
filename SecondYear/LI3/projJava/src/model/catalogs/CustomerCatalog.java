package model.catalogs;

import model.base.ICustomer;

import java.io.Serializable;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;
import java.util.stream.Collectors;

import static java.util.stream.Collectors.toMap;

public class CustomerCatalog implements ICustomerCatalog, Serializable {
    private Set<ICustomer> catalog;

    public CustomerCatalog(){
        this.catalog = new HashSet<>();
    }

    public CustomerCatalog(CustomerCatalog cc){
        this.catalog = cc.getCatalog();
    }

    private Set<ICustomer> getCatalog() {
        return this.catalog.stream().map(ICustomer::clone).collect(Collectors.toSet());
    }

    @Override
    public void addToCatalog(ICustomer customer) {
        this.catalog.add(customer.clone());
    }

    @Override
    public boolean existsInCatalog(ICustomer customer) {
        return this.catalog.contains(customer);
    }

    @Override
    public int number_of_customers() {
        return catalog.size();
    }

    @Override
    public ICustomerCatalog clone(){
        return new CustomerCatalog(this);
    }
}

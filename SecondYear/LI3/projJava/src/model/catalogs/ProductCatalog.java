package model.catalogs;

import model.base.IProduct;

import java.io.Serializable;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;
import java.util.stream.Collectors;

import static java.util.stream.Collectors.toMap;

public class ProductCatalog implements IProductCatalog, Serializable {
    private Set<IProduct> catalog;

    public ProductCatalog(){
        this.catalog = new HashSet<>();
    }

    public ProductCatalog(ProductCatalog pc){
        this.catalog = pc.getCatalog();
    }

    private Set<IProduct> getCatalog() {
        return this.catalog.stream().map(IProduct::clone).collect(Collectors.toSet());
    }

    @Override
    public void addToCatalog(IProduct code) {
        this.catalog.add(code.clone());
    }

    @Override
    public boolean existsInCatalog(IProduct code) {
        return this.catalog.contains(code);
    }

    @Override
    public IProductCatalog clone(){
        return new ProductCatalog(this);
    }

    @Override
    public int number_of_products() {
        return catalog.size();
    }
}

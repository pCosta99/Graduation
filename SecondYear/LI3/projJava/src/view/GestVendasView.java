package view;

import com.jakewharton.fliptables.FlipTable;
import com.jakewharton.fliptables.FlipTableConverters;
import model.base.ICustomer;
import model.base.IProduct;
import org.apache.commons.lang3.tuple.ImmutablePair;
import org.javatuples.Pair;
import org.javatuples.Triplet;

import java.util.*;
import java.util.List;

public class GestVendasView implements IGestVendasView {
    private final String[] months = {"Jan", "Feb", "Mar", "Apr", "May", "June", "July", "Aug", "Sept", "Oct", "Nov", "Dec"};
    private final String[] months_with_blank = {"", "Jan", "Feb", "Mar", "Apr", "May", "June", "July", "Aug", "Sept", "Oct", "Nov", "Dec"};


    @Override
    public void firstMenuForm() throws NumberFormatException {
        System.out.println("Welcome to GestVendasApp!\n");
        System.out.println("How can we help you today?");
        menuForm();
    }

    @Override
    public void menuForm() {
        System.out.println("Options:");
        System.out.println("0 -> Leave");
        System.out.println("1 -> Query 1");
        System.out.println("2 -> Query 2");
        System.out.println("3 -> Query 3");
        System.out.println("4 -> Query 4");
        System.out.println("5 -> Query 5");
        System.out.println("6 -> Query 6");
        System.out.println("7 -> Query 7");
        System.out.println("8 -> Query 8");
        System.out.println("9 -> Query 9");
        System.out.println("10 -> Query 10");
        System.out.println("11 -> Query 11");
        System.out.println("12 -> Query 12");
        System.out.println("13 -> Serialize");
        System.out.println("14 -> Deserialize");
        System.out.println("15 -> Load custom files");
        System.out.println("16 -> Load default files");
    }

    @Override
    public void clearScreen(){
        for(int i = 0; i < 60; i++) System.out.println();
    }

    @Override
    public void screenInfo(String s){
        System.out.print(s);
    }

    @Override
    public void printProductCollection(Collection<IProduct> products){
        products.forEach(p -> System.out.println(p.getCode()));
        System.out.println("Total printed: " + products.size());
    }

    private String[][] matrixListToMatrixArray(List<List<String>> matrix){
        int rows = matrix.size();
        int columns = matrix.get(0).size();
        String[][] ret = new String[rows][columns];
        for(int i = 0; i < rows; i++){
            List<String> list = matrix.get(i);
            for(int j = 0; j < columns; j++){
                ret[i][j] = list.get(j);
            }
        }
        return ret;
    }

    @Override
    public void displayPerBranchInfo(List<ImmutablePair<Integer, Integer>> list) {
        int branch = 1;
        String[] headers = {"Branch", "Customers total", "Purchases total"};
        List<List<String>> data = new ArrayList<>();
        for(ImmutablePair<Integer, Integer> p : list){
            List<String> row = new ArrayList<>();
            if(branch <= 3) row.add(String.valueOf(branch));
            else row.add("All");
            row.add(String.valueOf(p.left));
            row.add(String.valueOf(p.right));
            branch++;
            data.add(row);
        }
        System.out.println(FlipTable.of(headers, matrixListToMatrixArray(data)));
    }

    @Override
    public void displayPerMonthInfo(List<Object> list, Object identifier){
        List<String> header = Arrays.asList(this.months_with_blank);
        Object[][] data = new Object[1][list.size()+1];
        data[0][0] = identifier;
        for(int i = 1; i <= list.size(); i++) data[0][i] = list.get(i-1);
        System.out.println(FlipTableConverters.fromObjects(header.toArray(new String[0]), data));
    }

    @Override
    public void displayPerBranchAndPerMonthInfo(Object[][] data, Object[] identifiers){
        List<String> header = Arrays.asList(months_with_blank);
        Object[][] allData = new Object[data.length][data[0].length+1];
        for(int i = 0; i < identifiers.length; i++) allData[i][0] = identifiers[i];
        for(int i = 0; i < data.length; i++){
            for(int j = 1; j <= data[i].length; j++){
                allData[i][j] = data[i][j-1];
            }
        }
        System.out.println(FlipTableConverters.fromObjects(header.toArray(new String[0]), allData));
    }

    @Override
    public void displayColumns(String[] identifiers, Object[] data){
        String[] empty_headers = {"", ""};
        Object[][] allData = new Object[identifiers.length][2];
        for(int i = 0; i < identifiers.length; i++){
            allData[i][0] = identifiers[i];
            allData[i][1] = data[i];
        }
        System.out.println(FlipTableConverters.fromObjects(empty_headers, allData));
    }

    @Override
    public void monthly_triplet_list_view_with_header(List<Triplet<Integer, Integer, Float>> list, String[] headers){
        List<List<String>> data = new ArrayList<>();
        for(int i = 0; i < list.size(); i++){
            List<String> row = new ArrayList<>();
            row.add(months[i]);
            row.add(String.valueOf(list.get(i).getValue0()));
            row.add(String.valueOf(list.get(i).getValue1()));
            row.add(String.valueOf(list.get(i).getValue2()));
            data.add(row);
        }
        System.out.println(FlipTable.of(headers, matrixListToMatrixArray(data)));
    }

    @Override
    public void triplet_list_view_with_header(List<Triplet<Object, Object, Object>> list, String[] headers){
        List<List<String>> data = new ArrayList<>();
        for(int i = 0; i < list.size(); i++){
            List<String> row = new ArrayList<>();
            row.add(String.valueOf(list.get(i).getValue0()));
            row.add(String.valueOf(list.get(i).getValue1()));
            row.add(String.valueOf(list.get(i).getValue2()));
            data.add(row);
        }
        System.out.println(FlipTable.of(headers, matrixListToMatrixArray(data)));
    }

    @Override
    public void pair_list_view_with_header(List<Pair<Object, Object>> list, String[] headers){
        List<List<String>> data = new ArrayList<>();
        for(int i = 0; i < list.size(); i++){
            List<String> row = new ArrayList<>();
            row.add(String.valueOf(list.get(i).getValue0()));
            row.add(String.valueOf(list.get(i).getValue1()));
            data.add(row);
        }
        System.out.println(FlipTable.of(headers, matrixListToMatrixArray(data)));
    }

    @Override
    public void perBranchTable(List<List<Pair<ICustomer, Float>>> lists){
        String[] headers = {"Customer", "Spent"};
        for(int i = 0; i < lists.size(); i++) {
            List<List<String>> data = new ArrayList<>();
            lists.get(i).forEach(p -> {
                List<String> row = new ArrayList<>();
                row.add(p.getValue0().getCode());
                row.add(String.valueOf(p.getValue1()));
                data.add(row);
            });
            System.out.println("Branch " + (i+1));
            System.out.println(FlipTable.of(headers, matrixListToMatrixArray(data)));
        }
    }

    @Override
    public void displayError(String error) {
        System.out.println(error);
    }
}
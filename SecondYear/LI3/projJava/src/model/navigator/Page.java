package model.navigator;

import com.jakewharton.fliptables.FlipTable;
import org.javatuples.Pair;
import utilities.MutablePair;

import java.util.ArrayList;
import java.util.List;
import java.util.regex.PatternSyntaxException;
import java.util.stream.Collectors;

public class Page implements IPage {
    public static final String ANSI_RESET = "\u001B[0m";
    public static final String ANSI_BLACK = "\u001B[30m";
    public static final String ANSI_RED = "\u001B[31m";
    public static final String ANSI_GREEN = "\u001B[32m";
    public static final String ANSI_YELLOW = "\u001B[33m";
    public static final String ANSI_BLUE = "\u001B[34m";
    public static final String ANSI_PURPLE = "\u001B[35m";
    public static final String ANSI_CYAN = "\u001B[36m";
    public static final String ANSI_WHITE = "\u001B[37m";

    private List<MutablePair<String, Boolean>> header;
    private List<MutablePair<String, Boolean>> content;
    private List<MutablePair<String, Boolean>> defaultHeader;
    private int print_cols;
    private int print_lines;

    public Page(List<String> content, int print_lines, int print_cols, List<String> header){
        this.content = content.stream().map(s -> new MutablePair<>(s, false)).collect(Collectors.toList());
        this.header = header.stream().map(s -> new MutablePair<>(s, false)).collect(Collectors.toList());
        this.defaultHeader = new ArrayList<>();
        this.print_cols = print_cols;
        this.print_lines = print_lines;
        for(int i = 0; i < print_cols; i++){
            defaultHeader.add(new MutablePair<>("", false));
        }
    }

    public Page(List<String> content, int print_lines, int print_cols){
        this.content = content.stream().map(s -> new MutablePair<>(s, false)).collect(Collectors.toList());
        this.header = new ArrayList<>();
        this.defaultHeader = new ArrayList<>();
        this.print_cols = print_cols;
        this.print_lines = print_lines;
        for(int i = 0; i < print_cols; i++){
            defaultHeader.add(new MutablePair<>("", false));
        }
    }

    @Override
    public String toString(){
        /* Get the header or use the default one */
        String[] headers = new String[print_cols];
        boolean useDefault = header.size() == 0;
        for(int i = 0; i < print_cols; i++){
            MutablePair<String, Boolean> pair;
            /* Check which header to use */
            if(useDefault) pair = defaultHeader.get(i);
            else pair = header.get(i);
            /* Check if we color it or not */
            if(pair.getSnd()) headers[i] = ANSI_YELLOW + pair.getFst() + ANSI_RESET;
            else headers[i] = pair.getFst();
        }

        /* Parse the data from the content we want to print */
        String[][] data =  new String[print_lines][print_cols];
        for(int i = 0; i < print_lines; i++){
            for(int j = 0; j < print_cols; j++){
                if((i*print_cols + j) >= content.size()) data[i][j] = "";
                else{
                    MutablePair<String, Boolean> pair = content.get(i * print_cols + j);
                    if(pair.getSnd()) data[i][j] = ANSI_YELLOW + pair.getFst() + ANSI_RESET;
                    else data[i][j] = pair.getFst();
                }
            }
        }

        /* Print it together */
        return FlipTable.of(headers, data);
    }

    @Override
    public int contains(String regex) throws PatternSyntaxException {
        int amount = 0;
        for (MutablePair<String, Boolean> pair : header) {
            if (pair.getFst().matches(regex)) {
                amount++;
                pair.setSnd(true);
            }
        }
        for (MutablePair<String, Boolean> pair : content) {
            if (pair.getFst().matches(regex)) {
                amount++;
                pair.setSnd(true);
            }
        }
        return amount;
    }

    @Override
    public void clearHits(){
        content.forEach(p -> p.setSnd(false));
        header.forEach(p -> p.setSnd(false));
        defaultHeader.forEach(p -> p.setSnd(false));
    }
}

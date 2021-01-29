package model.navigator;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class StatusBar implements IStatusBar {
    private int currentPage;
    private int totalPages;
    private Map<String, String> messages;
    private List<String> history;
    private String type;

    public StatusBar(int totalPages){
        this.totalPages = totalPages;
        this.messages = new HashMap<>();
        this.history = new ArrayList<>();
        this.type = "default";
        this.messages.put(type, "You need to give me other messages!");
    }

    @Override
    public void setCurrentPage(int currentPage){
        this.currentPage = currentPage;
    }

    @Override
    public void addMessage(String type, String message){
        messages.put(type, message);
    }

    @Override
    public void setMessageType(String type){
        this.type = type;
    }

    @Override
    public String toString(){
        String print = "(Page " + currentPage + " of " + totalPages + ") --> " + messages.get(type);
        history.add(print);
        return print;
    }
}

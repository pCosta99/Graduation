package model.navigator;

import java.util.ArrayList;
import java.util.List;
import java.util.regex.PatternSyntaxException;

public class Navigator implements INavigator {
    private boolean showingHits;
    private int total_print_size;
    private int total_pages;
    private int current_page;
    private List<Page> pages;
    private IStatusBar statusBar;
    private boolean statusBarOn;
    /* Lookup helpers */
    private List<Integer> hits;
    private int current_hit;

    public Navigator(int print_lines, List<String> content, List<String> header){
        int i;
        this.showingHits = false;
        this.hits = new ArrayList<>();
        this.current_hit = 0;
        this.total_print_size = print_lines * header.size();
        this.total_pages = content.size() / total_print_size;
        this.pages = new ArrayList<>();
        current_page = 0;
        for(i = 0; i < total_pages; i++) {
            pages.add(new Page(content.subList(i * total_print_size, (i+1) * total_print_size), print_lines, header.size(), header));
        }
        pages.add(new Page(content.subList(i * total_print_size, content.size()), print_lines, header.size(), header));
        statusBar = new StatusBar(total_pages);
        statusBarInit();
    }

    public Navigator(int print_lines, int print_cols, List<String> content){
        int i;
        this.hits = new ArrayList<>();
        this.showingHits = false;
        this.current_hit = 0;
        this.total_print_size = print_lines * print_cols;
        this.total_pages = content.size() / total_print_size;
        this.pages = new ArrayList<>();
        current_page = 0;
        for(i = 0; i < total_pages; i++) {
            pages.add(new Page(content.subList(i * total_print_size, (i+1) * total_print_size), print_lines, print_cols));
        }
        pages.add(new Page(content.subList(i * total_print_size, content.size()), print_lines, print_cols));
        statusBar = new StatusBar(total_pages);
        statusBarInit();
    }
    public Navigator(int print_lines, List<String> content, ArrayList<List<String>> headers){
        int i = 0;
        this.hits = new ArrayList<>();
        this.showingHits = false;
        this.current_hit = 0;
        this.total_print_size = print_lines * headers.get(i).size();
        this.total_pages = content.size() / total_print_size;
        this.pages = new ArrayList<>();
        current_page = 0;
        for(i = 0; i < (total_pages-1); i++) {
            pages.add(new Page(content.subList(i * total_print_size, (i+1) * total_print_size), print_lines, headers.get(i).size(), headers.get(i)));
        }
        pages.add(new Page(content.subList(i * total_print_size, content.size()), print_lines, headers.get(i).size(), headers.get(i)));
        statusBar = new StatusBar(total_pages);
        statusBarInit();
    }

    private void statusBarInit(){
        statusBarOn = false;
        statusBar.addMessage("first page", "Already at first page!");
        statusBar.addMessage("last page", "Already at last page!");
        statusBar.addMessage("not enough pages", "There's only " + total_pages + " pages to go over!");
        statusBar.addMessage("no match", "Couldn't find what you where looking for!");
        statusBar.addMessage("sucess on lookup", "Got what you were looking for!");
        statusBar.addMessage("first hit", "Already on first match!");
        statusBar.addMessage("last hit", "Already on last match!");
        statusBar.addMessage("not showing hits", "Do a lookup before trying to navigate them!");
        statusBar.addMessage("invalid regex", "The regex provided is invalid!");
        /* Used whenever it's needed */
        statusBar.addMessage("temp", "");
    }

    @Override
    public void setHelpMessage(String message){
        if(statusBarOn) statusBar.addMessage("help", message);
    }

    @Override
    public void setDefaultMessage(String message){
        if(statusBarOn) statusBar.addMessage("default", message);
    }

    @Override
    public void triggerStatusBar(){
        statusBarOn = true;
    }

    @Override
    public void callHelpMessage(){
        statusBar.setMessageType("help");
    }

    @Override
    public void nextPage(){
        showingHits = false;
        if(current_page < total_pages) {
            current_page++;
            if(statusBarOn) statusBar.setCurrentPage(current_page);
        } else if(statusBarOn) {
            statusBar.setMessageType("last page");
        }
    }

    @Override
    public void previousPage(){
        showingHits = false;
        if(current_page > 0) {
            current_page--;
            if(statusBarOn) statusBar.setCurrentPage(current_page);
        } else if(statusBarOn) {
            statusBar.setMessageType("first page");
        }
    }

    @Override
    public String toString(){
        String page;
        if(!showingHits) pages.forEach(Page::clearHits);
        page = pages.get(current_page).toString();
        if(statusBarOn) {
            page = page + statusBar.toString();
            statusBar.setMessageType("default");
        }
        return page;
    }

    @Override
    public void getPage(int page) {
        showingHits = false;
        if(page > total_pages && statusBarOn) statusBar.setMessageType("not enough pages");
        else if(page <= total_pages) {
            current_page = page;
            if(statusBarOn) statusBar.setCurrentPage(page);
        }
    }

    @Override
    public void find(String regex) {
        try {
            if (showingHits) pages.forEach(Page::clearHits);
            showingHits = true;
            boolean gotIt = false;
            hits.clear();
            /* Go through each page and store the ones where we found the string */
            for (int i = 0; i < pages.size(); i++) {
                int amount = pages.get(i).contains(regex);
                if (amount > 0) {
                    if (!gotIt) {
                        current_page = i;
                        current_hit = 0;
                        hits.add(i);
                        if (statusBarOn) {
                            statusBar.setMessageType("sucess on lookup");
                            statusBar.setCurrentPage(current_page);
                        }
                    } else hits.add(i);
                    gotIt = true;
                }
            }
            if (!gotIt && statusBarOn) statusBar.setMessageType("no match");
        } catch(PatternSyntaxException e){
            if(statusBarOn) statusBar.setMessageType("invalid regex");
        }
    }

    @Override
    public void nextOcurrence() {
        if(showingHits) {
            if (current_hit < hits.size() - 1) {
                current_hit++;
                current_page = hits.get(current_hit);
                if (statusBarOn) {
                    statusBar.setCurrentPage(current_page);
                    statusBar.addMessage("temp", "Match " + (current_hit + 1) + " of " + hits.size() + ".");
                    statusBar.setMessageType("temp");
                }
            } else if (statusBarOn) statusBar.setMessageType("last hit");
        } else if (statusBarOn) statusBar.setMessageType("not showing hits");
    }

    @Override
    public void previousOcurrence() {
        if(showingHits) {
            if (current_hit > 0) {
                current_hit--;
                current_page = hits.get(current_hit);
                if (statusBarOn) {
                    statusBar.setCurrentPage(current_page);
                    statusBar.addMessage("temp", "Match " + (current_hit + 1) + " of " + hits.size() + ".");
                    statusBar.setMessageType("temp");
                }
            } else if (statusBarOn) statusBar.setMessageType("first hit");
    } else if (statusBarOn) statusBar.setMessageType("not showing hits");
}
}

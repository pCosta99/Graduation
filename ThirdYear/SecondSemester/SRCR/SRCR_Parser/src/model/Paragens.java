package model;

import java.util.HashMap;
import java.util.Map;

public class Paragens {
    private Map<Integer, Paragem> paragens;

    public Paragens(){
        paragens = new HashMap<>();
    }

    public void addParagem(Paragem p){
        paragens.put(p.getGid(), p);
    }

    public Paragem getParagem(int gid){
        return this.paragens.get(gid);
    }

    public void printParagens(){
        paragens.values().forEach(System.out::println);
    }
}

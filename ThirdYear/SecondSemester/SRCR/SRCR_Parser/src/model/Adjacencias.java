package model;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class Adjacencias {
    Map<String, List<Adjacencia>> adjacencias;

    public Adjacencias(){
        this.adjacencias = new HashMap<>();
    }

    public void addAdjacencia(String carreira, Adjacencia adj){
        if(adjacencias.containsKey(carreira)){
            adjacencias.get(carreira).add(adj);
        } else {
            List<Adjacencia> adjList = new ArrayList<>();
            adjList.add(adj);
            adjacencias.put(carreira, adjList);
        }
    }

    public void printAdjacencias(){
        adjacencias.values().forEach(l -> l.forEach(System.out::println));
    }
}

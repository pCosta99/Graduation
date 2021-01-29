import javax.print.DocFlavor;
import java.io.*;
import java.util.*;
import java.util.stream.Collectors;
import java.util.stream.Stream;

public class gestorCarros implements Serializable {
    private Map<String,Carro> mapa;
    public gestorCarros() {
        this.mapa = new HashMap<>();
    }
    public gestorCarros(gestorCarros carros) {
        this.mapa = carros.getMapa();
    }
    public gestorCarros(Map<String,Carro> mapa) {
        this.mapa = mapa;
    }
    public gestorCarros clone(){
        return new gestorCarros(this);
    }
    public boolean equals(Object obj) {
        if(obj==this) return true;
        if(obj==null || obj.getClass()!=this.getClass()) return false;
        gestorCarros h = (gestorCarros) obj;
        return this.mapa.equals(h.getMapa());
    }

    public Map<String, Carro> getMapa() {
        Map<String,Carro> p = this.mapa.values().stream().collect(Collectors.toMap((e)->e.getMatricula(),(e)->e.clone()));
        return p;
    }
    public void setMapa(Map<String, Carro> h) {
        this.mapa = h.entrySet().stream().collect(Collectors.toMap((e)->e.getKey(),(e)->e.getValue()));
    }
    public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append("Mapa ").append(mapa.toString());
        return sb.toString();
    }
    public boolean existeCarro(String cod){
        return this.mapa.containsKey(cod);
    }
    public int quantos(){
        return this.mapa.size();
    }
    public Carro getCarro(String cod){
        return this.mapa.get(cod);
    }
    public List<Carro> quantosDoTipo(TipoCarro tipoCarro){
        return this.mapa.values().stream().filter(e->e.getTipo().equals(tipoCarro)).collect(Collectors.toList());
    }
    public void adiciona(Carro carro){
        this.mapa.put(carro.getMatricula(),carro.clone());
    }
    public void adiciona(Set<Carro> hs){
        for(Carro ob: hs){
            this.mapa.put(ob.getMatricula(),ob.clone());
        }
    }

    public int quantosT(String tipo) {

        return (int) this.mapa.values().stream().filter(h -> h.getTipo().equals(tipo)).count();
    }

    public List<Carro> getCarrosAsList(){

        return this.mapa.values().stream().map(h->h.clone()).collect(Collectors.toList());
    }
    public void toFile(String nomeDoficheiro) throws FileNotFoundException, IOException {
        FileOutputStream fos = new FileOutputStream(nomeDoficheiro);
        ObjectOutputStream oos = new ObjectOutputStream(fos);
        oos.writeObject(this);
        oos.flush();
        oos.close();
    }
    public static gestorCarros carregaEstado(String nomedoficheiro) throws FileNotFoundException, IOException, ClassNotFoundException{
        FileInputStream fis = new FileInputStream(nomedoficheiro);
        ObjectInputStream ois = new ObjectInputStream(fis);
        gestorCarros h = (gestorCarros) ois.readObject();
        ois.close();
        return h;
    }

    public Carro carroMaisBarato(Localizacao destino){
        List<Carro> carros = this.mapa.values().stream().map(carro->carro.clone()).collect(Collectors.toList());
        double custoMaisBaixo = 1000000000000.0;
        Carro carroMaisBarato = null;
        for(Carro carro : carros){
            double distancia = carro.getPos().distancia(destino);
            double custo = carro.getPrecoPorKM()*distancia;
            if(custo< custoMaisBaixo &&carro.autonomiaAcimaDe10porcento() && carro.autonomiaSuficiente(distancia)&&carro.getDisponivel()){
                custoMaisBaixo = custo;
                carroMaisBarato = carro;
            }
        }
        return carroMaisBarato;
    }
    public Carro carroMaisPerto(Localizacao cliente){
        List<Carro> carros = this.mapa.values().stream().map(carro->carro.clone()).collect(Collectors.toList());
        double distanciaMaisPequena = 10000000000.0;
        Carro carroMaisPerto = null;
        for (Carro carro : carros){
            double distancia = carro.getPos().distancia(cliente);
            if(distancia<distanciaMaisPequena && carro.autonomiaAcimaDe10porcento() &&carro.autonomiaSuficiente(distancia)&&carro.getDisponivel()){
                distanciaMaisPequena = distancia;
                carroMaisPerto = carro;
            }

        }
        ;
        return carroMaisPerto;
    }
    public Carro carroMaisBaratoMaxDist(Localizacao cliente, Localizacao destino, double distanciaMax){
        List<Carro> carros = this.mapa.values().stream().map(carro->carro.clone()).collect(Collectors.toList());
        double custoMaisBaixo = 100000000000.0;
        Carro carro = null;
        for(Carro c : carros){
            double distanciaDest = c.getPos().distancia(destino);
            double custo = c.getPrecoPorKM()*distanciaDest;
            double distanciaCarro = c.getPos().distancia(cliente);
            if(distanciaCarro< distanciaMax && custo<custoMaisBaixo && c.autonomiaAcimaDe10porcento() &&c.autonomiaSuficiente(distanciaDest)&& c.getDisponivel()){
                carro = c.clone();
                custoMaisBaixo = custo;
            }
        }
        return carro;
    }
}

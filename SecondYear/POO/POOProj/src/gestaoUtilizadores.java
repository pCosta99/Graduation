import javax.print.DocFlavor;
import java.io.*;
import java.util.*;
import java.util.stream.*;
import java.util.stream.Collectors;

public class gestaoUtilizadores implements Serializable {
    private Map<String,Proprietario> proprietarioMap;
    private Map<String,Cliente> clienteMap;
    public gestaoUtilizadores() {
        this.proprietarioMap = new HashMap<>();
        this.clienteMap = new HashMap<>();
    }
    public gestaoUtilizadores(gestaoUtilizadores gestao) {
        this.proprietarioMap = gestao.getProprietarios();
        this.clienteMap = gestao.getClientes();
    }
    public gestaoUtilizadores(Map<String,Cliente> clientes, Map<String,Proprietario> proprietarios) {
        this.setClientes(clientes);
        this.setProprietarios(proprietarios);
    }
    public gestaoUtilizadores clone(){
        return new gestaoUtilizadores(this);
    }
    public boolean equals(Object obj) {
        if(obj==this) return true;
        if(obj==null || obj.getClass()!=this.getClass()) return false;
        gestaoUtilizadores g = (gestaoUtilizadores) obj;
        return this.clienteMap.equals(g.getClientes())&&this.proprietarioMap.equals(g.getProprietarios());
    }

    public Map<String, Cliente> getClientes() {
        Map<String,Cliente> p = this.clienteMap.values().stream().collect(Collectors.toMap((e)->e.getEmail(),(e)->e.clone()));
        return p;
    }
    public Map<String,Proprietario> getProprietarios(){
        Map<String, Proprietario> p = this.proprietarioMap.values().stream().collect(Collectors.toMap((e)->e.getEmail(),(e)->e.clone()));
        return p;
    }
    public void setClientes(Map<String, Cliente> h) {
        this.clienteMap = h.entrySet().stream().collect(Collectors.toMap((e)->e.getKey(),(e)->e.getValue()));
    }
    public void setProprietarios(Map<String, Proprietario> h) {
        this.proprietarioMap = h.entrySet().stream().collect(Collectors.toMap((e)->e.getKey(),(e)->e.getValue()));
    }
    public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append("Proprietarios ").append(proprietarioMap.toString()).append("Clientes").append(clienteMap.toString());
        return sb.toString();
    }
    public boolean existeUtilizador(String email){
        return (this.clienteMap.containsKey(email) ||this.proprietarioMap.containsKey(email));
    }
    public boolean existeCliente(String email){
        return (this.clienteMap.containsKey(email));
    }
    public boolean existeProprietario(String email){
        return (this.proprietarioMap.containsKey(email));
    }
    public int quantos(){
        return this.proprietarioMap.size()+this.clienteMap.size();
    }
    public Cliente getCliente(String email){
        return this.clienteMap.get(email).clone();
    }
    public Proprietario getProprietario(String email){return this.proprietarioMap.get(email).clone();}
    public void toFile(String nomeDoficheiro) throws FileNotFoundException, IOException {
        FileOutputStream fos = new FileOutputStream(nomeDoficheiro);
        ObjectOutputStream oos = new ObjectOutputStream(fos);
        oos.writeObject(this);
        oos.flush();
        oos.close();
    }
    public static gestaoUtilizadores carregaEstado(String nomedoficheiro) throws FileNotFoundException, IOException, ClassNotFoundException{
        FileInputStream fis = new FileInputStream(nomedoficheiro);
        ObjectInputStream ois = new ObjectInputStream(fis);
        gestaoUtilizadores h = (gestaoUtilizadores) ois.readObject();
        ois.close();
        return h;
    }

    public Cliente loginCliente(String email, String password) throws Exception_ClienteInexistente,Exception_PasswordErrada{
        if(!existeCliente(email)) throw new Exception_ClienteInexistente("Não existe um utilizador com esse email");
        else if(!getCliente(email).getPassword().equals(password)){
            throw new Exception_PasswordErrada("Password incorreta");
        }
        else return getCliente(email);
    }
    public Proprietario loginProprietario(String email, String password) throws Exception_ClienteInexistente,Exception_PasswordErrada{
        if(!existeProprietario(email)) throw new Exception_ClienteInexistente("Não existe um utilizador com esse email");
        else if(!getProprietario(email).getPassword().equals(password)){
            throw new Exception_PasswordErrada("Password incorreta");
        }
        else return getProprietario(email);
    }

    public void stringToProprietario(String[] campos){
        Proprietario prop = new Proprietario();
        prop.setNome(campos[0]);
        prop.setNif(campos[1]);
        prop.setEmail(campos[2]);
        prop.setMorada(campos[3]);
        this.proprietarioMap.put(prop.getEmail(),prop.clone());

    }
    public void stringToCliente(String[] campos){
        Cliente cliente = new Cliente();
        cliente.setNome(campos[0]);
        cliente.setNif(campos[1]);
        cliente.setEmail(campos[2]);
        cliente.setMorada(campos[3]);
        cliente.setPos(new Localizacao(Double.parseDouble(campos[4]),Double.parseDouble(campos[5])));
        this.clienteMap.put(cliente.getEmail(),cliente.clone());
    }

    // Com base no total gasto
    public String top10gastos(){
        int i = 0;
        StringBuilder sb = new StringBuilder();
        TreeSet<Cliente> top10 = this.clienteMap
                                     .values()
                                     .stream()
                                     .map(cliente -> cliente.clone())
                                     .collect(Collectors.toCollection(()->new TreeSet<>(new ComparatorClienteGasto())));
        List<Cliente> ret = new ArrayList<Cliente>(10);
        for (Cliente c : top10){
            ret.add(c.clone());
            sb.append((i+1)+ "-" + c.toString() + "\n");

            if (i < 9) i++;
            else break;
        }

        return sb.toString();
    }

    // Com base na utilização do sistema
    public String top10utilizacao(){
        int i = 0;
        StringBuilder sb = new StringBuilder();
        TreeSet<Cliente> top10 = this.clienteMap
                                     .values()
                                     .stream()
                                     .map(cliente -> cliente.clone())
                                     .collect(Collectors.toCollection(()->new TreeSet<>(new ComparatorClienteUtilizacao())));
        List<Cliente> ret = new ArrayList<Cliente>(10);
        for (Cliente c : top10){
            ret.add(c.clone());
            sb.append((i+1)+ "-" + c.toString() + "\n");
            if (i < 9) i++;
            else break;
        }
        return sb.toString();
    }
}

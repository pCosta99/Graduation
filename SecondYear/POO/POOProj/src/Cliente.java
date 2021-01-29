import java.io.Serializable;
import java.time.LocalDate;
import java.util.Comparator;
import java.util.List;
import java.util.ArrayList;
import java.util.Date;
import java.util.stream.Collectors;

/**
 * Cliente do sistema.
 */
public class Cliente extends Utilizador implements Serializable {
    private Localizacao pos;
    private List<Aluguer> alugueres;
    private float classificacao;
    private int numClassificacoes;
    private List<String> matriculasPorClassificar;
    private static final int kmPorHora = 4;

    /**
     * Construtor vazio
     */
    public Cliente(){
        super();
        this.pos = new Localizacao();
        this.alugueres = new ArrayList<>();
        this.matriculasPorClassificar = new ArrayList<>();
        this.classificacao = 0;
        this.numClassificacoes = 0;
    }

    /**
     * Construtor parametrizado
     */
    public Cliente(String email, String password,
                   String morada, String nome, LocalDate dataNascimento, Localizacao pos, ArrayList<Aluguer> alugueres,
                   String nif){
        super(email, nif, password, morada, nome, dataNascimento);
        this.pos = pos.clone();
        this.alugueres = alugueres.stream().map(Aluguer::clone).collect(Collectors.toList());
        this.classificacao = 0;
        this.numClassificacoes = 0;
    }
    /**
     * Devolve a lista de alugueres entre a data1 e a data2.
     * @param data1 data de começo do periodo
     * @param data2 data de fim do periodo
     * @return Lista de Alugueres entre as datas.
     */
    public List<Aluguer>aluguerEntreDatas(LocalDate data1, LocalDate data2){

        return this.alugueres.stream().filter(aluguer -> aluguer.getData().isAfter(data1))
                .filter(aluguer -> aluguer.getData().isBefore(data2)).map(h->h.clone()).collect(Collectors.toList());
    }
    /**
     * Construtor de cópia
     */
    public Cliente(Cliente c){
        super(c);
        this.pos = c.getPos().clone();
        this.alugueres = c.getAlugueres();
        this.classificacao = c.getClassificacao();
        this.numClassificacoes = c.getNumClassificacoes();
        this.matriculasPorClassificar = c.getMatriculasPorClassificar();
    }

    /**
     * Devolve a localizacao do cliente.
     *
     * @return localizacao do cliente.
     */
    public Localizacao getPos(){
        return this.pos;
    }

    /**
     * Devolve a lista de alugueres efetuados pelo cliente.
     *
     * @return lista de alugueres efetuados pelo cliente.
     */
    public List<Aluguer> getAlugueres(){
        return this.alugueres.stream().map(Aluguer::clone).collect(Collectors.toList());
    }

    public List<String> getMatriculasPorClassificar(){
        return this.matriculasPorClassificar.stream().collect(Collectors.toList());
    }
    /**
     * Devolve a classificacao do cliente.
     *
     * @return classificação do cliente.
     */
    public float getClassificacao(){
        return this.classificacao;
    }

    /**
     * Devolve o número de classificações do cliente.
     *
     * @return número de classificações do cliente.
     */
    public int getNumClassificacoes(){
        return this.numClassificacoes;
    }


    /**
     * Calcula o total gasto pelo cliente.
     *
     * @return Total gasto pelo cliente.
     */
    public double getTotalGasto(){ return this.alugueres.stream().mapToDouble(a -> a.getCustoViagem()).reduce(0, Double::sum); }

    /**
     * Atualiza a localizacao do cliente.
     *
     * @param pos nova localizacao
     */
    public void setPos(Localizacao pos){
        this.pos = pos.clone();
    }

    /**
     * Adiciona um aluguer a lista do cliente.
     *
     * @param a novo aluguer.
     */
    public void addAluguer(Aluguer a){
        this.alugueres.add(a.clone());
    }
    public void addPorClassificar(String a){
        this.matriculasPorClassificar.add(a);
    }
    public void removePorClassificar(String a){this.matriculasPorClassificar.remove(a);}


    /**
     * Implementação do método de clonagem de um Cliente
     *
     * @return objeto do tipo Cliente
     */
    public Cliente clone(){
        return new Cliente(this);
    }

    /**
     * Implementação do método de igualdade entre dois clientes
     * Redefinição do método equals de Object.
     *
     //* @param  umCliente cliente a ser comparado com o recetor
     * @return true ou false
     */
    public boolean equals(Object o){
        if (this == o) return true;
        if ((o == null) || (this.getClass() != o.getClass())) return false;
        if (!super.equals(o)) return false;
        Cliente c = (Cliente) o;
        return(this.pos.equals(c.getPos()) &&
                this.alugueres.equals(c.getAlugueres()) &&
                this.classificacao == c.getClassificacao());
    }
    public String toString(){
        StringBuilder sb = new StringBuilder();
        sb.append("Cliente : ");
        sb.append(this.getEmail());
        sb.append(" Nome: ");
        sb.append(this.getNome());
        sb.append(" Classificacao ");
        sb.append(this.getClassificacao());
        sb.append("\n");
        return  sb.toString();
    }
    /**
     * Métodos
     */

    /**
     * Atualiza a classificacão do cliente.
     *
     * @param classificacao classificacão a adicionar.
     */
    public void insertClassificacao(float classificacao){
        if(this.numClassificacoes == 0) this.classificacao = classificacao;
        else {
            this.classificacao = ((this.classificacao * this.numClassificacoes)
                    + classificacao) / (this.numClassificacoes + 1);
        }this.numClassificacoes++;
    }





}

class ComparatorClienteGasto implements Comparator<Cliente>, Serializable {
    /**
     * Implementação do método compare de clientes.
     *
     * Compara com base no gasto de cada cliente.
     */
    public int compare(Cliente c1, Cliente c2) {
        if (c1.getTotalGasto() < c2.getTotalGasto()) return 1;
        else return -1;
    }
}

class ComparatorClienteUtilizacao implements Comparator<Cliente>, Serializable {
    /**
     * Implementação do método compare de clientes.
     *
     * Compara com base na utilização do sistema de cada cliente.
     */
    public int compare(Cliente c1, Cliente c2){
        if (c1.getAlugueres().size() < c2.getAlugueres().size()) return 1;
        else return -1;
    }
}
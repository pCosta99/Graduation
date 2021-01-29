import java.io.Serializable;
import java.time.LocalDate;
import java.util.*;
import java.util.stream.Collectors;

/**
 * Proprietário de um (ou mais) automóveis no sistema.
 */
public class Proprietario extends Utilizador implements Serializable
{
    // variáveis de instância
    private float classificacao;
    private int numClassificacoes;
    private List<Aluguer> alugueres;
    private List<Aluguer> alugueresEmEspera;
    private Map<String,Carro> listaVeiculos;

    /**
     *
     * Construtor vazio
     */
    public Proprietario()
    {
        super();
        this.classificacao = 0;
        this.numClassificacoes = 0;
        this.alugueres = new ArrayList<Aluguer>();
        this.alugueresEmEspera = new ArrayList<Aluguer>();
        this.listaVeiculos = new HashMap<>();
    }

    /**
     * Construtor parametrizado
     */
    public Proprietario(String email, String username, String password,
                        String morada, String nome, LocalDate dataNascimento, ArrayList<Aluguer> alugueres,
                        ArrayList<Aluguer> alugueresEmEspera, HashMap<String,Carro> listaVeiculos){
        super(email, username, password, morada, nome, dataNascimento);
        this.classificacao = 0;
        this.alugueres = alugueres.stream().map(Aluguer::clone).collect(Collectors.toList());
        this.alugueresEmEspera = alugueresEmEspera.stream().map(Aluguer::clone).collect(Collectors.toList());
        this.listaVeiculos = listaVeiculos.entrySet().stream().collect(Collectors.toMap(e -> e.getKey(), e ->e.getValue().clone()));
    }

    /**
     * Construtor de cópia
     */
    public Proprietario(Proprietario p) {
        super(p);
        this.classificacao = p.getClassificacao();
        this.alugueres = p.getAlugueres();
        this.alugueresEmEspera = p.getAlugueresEmEspera();
        this.listaVeiculos = p.getListaVeiculos();
    }

    /**
     * Devolve a classificacão do proprietário.
     *
     * @return classificacão do proprietário.
     */
    public float getClassificacao() {
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
     * Devolve a lista de alugueres do proprietário.
     *
     * @return lista de alugueres do proprietário.
     */
    public List<Aluguer> getAlugueres() {
        return this.alugueres.stream().map(Aluguer::clone).collect(Collectors.toList());
    }

    /**
     * Devolve a lista de alugueres em espera do proprietário.
     *
     * @return lista de alugueres do proprietário.
     */
    public List<Aluguer> getAlugueresEmEspera() {
        return this.alugueresEmEspera.stream().map(Aluguer::clone).collect(Collectors.toList());
    }

    /**
     * Devolve a lista de alugueres entre a data1 e a data2.
     * @param data1 data de começo do periodo
     * @param data2 data de fim do periodo
     * @return Lista de Alugueres entre as datas.
     */
    public List<Aluguer>aluguerEntreDatas(LocalDate data1, LocalDate data2){
        return this.alugueres.stream().filter(aluguer -> aluguer.getData().isAfter(data1))
                .filter(aluguer -> aluguer.getData().isBefore(data2)).map(a->a.clone()).collect(Collectors.toList());
    }
    /**
     * Devolve a lista de veículos do proprietário.
     *
     * @return lista de veículos do proprietário.
     */
    public Map<String, Carro> getListaVeiculos() {
        Map<String,Carro> p = this.listaVeiculos.values().stream().collect(Collectors.toMap((e)->e.getMatricula(),(e)->e.clone()));
        return p;
    }


    /**
     * Adiciona um aluguer à lista do proprietário.
     *
     * @param a novo aluguer.
     */
    public void addAluguer(Aluguer a) {
        this.alugueres.add(a.clone());
    }

    /**
     * Adiciona um veículo à lista do proprietário.
     *
     * @param c novo veículo.
     */
    public void addVeiculo(Carro c) {
        this.listaVeiculos.put(c.getMatricula(),c.clone());
    }

    /**
     * Adiciona um aluguer à lista de espera do proprietário.
     *
     * @param a novo aluguer.
     */
    public void addAluguerEmEspera(Aluguer a) {
        this.alugueresEmEspera.add(a.clone());
    }
    /**
     * Implementação do método de clonagem de um Proprietario
     *
     * @return objeto do tipo Proprietario
     */
    public Proprietario clone() {
        return new Proprietario(this);
    }

    public void removeAluguerEmEspera(Aluguer a){
        this.alugueresEmEspera.remove(a);
    }
    /**
     * Implementação do método de igualdade entre dois proprietários
     * Redefinição do método equals de Object.
     *
     //* @param  umProprietario proprietário a ser comparada com o recetor
     * @return true ou false
     */
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        if (!super.equals(o)) return false;
        Proprietario that = (Proprietario) o;
        return classificacao == that.classificacao &&
                this.alugueres.equals(alugueres) &&
                this.listaVeiculos.equals(listaVeiculos);
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
        this.classificacao = ((this.classificacao * this.numClassificacoes)
                + classificacao) / (this.numClassificacoes + 1);
        this.numClassificacoes++;
    }

    /**
     * Abastece um carro com uma certa autonomia que tambem atualiza o combustivel
     * @param matricula
     * @param autonomia
     */
    public void abastecimento(String matricula,int autonomia) {
        if (!this.listaVeiculos.containsKey(matricula)) return;
        int autonomiamax = this.listaVeiculos.get(matricula).getAutonomiaMax();
        int autonomiaatual = this.listaVeiculos.get(matricula).getAutonomiaAtual();
        Carro novo = this.listaVeiculos.get(matricula).clone();
        if (autonomiaatual + autonomia > autonomiamax) {
            novo.setAutonomiaAtual(autonomiamax);
            this.listaVeiculos.put(matricula, novo);
        } else {
            novo.setAutonomiaAtual(autonomiaatual + autonomia);
            this.listaVeiculos.put(matricula,novo );
        }
    }


    private void setPrecoKM(Carro carro, float preco){
        Carro novo = this.listaVeiculos.get(carro.getMatricula()).clone();
        novo.setPrecoPorKM(preco);
        this.listaVeiculos.put(carro.getMatricula(),novo);
    }



    public String toString(){
        StringBuilder sb = new StringBuilder();
        sb.append("Nome : ");
        sb.append(this.getNome());
        sb.append(" Email: ");
        sb.append(this.getEmail());
        sb.append(" Classificacao : ");
        sb.append(this.getClassificacao());
        sb.append("\n");

        return  sb.toString();
    }


}

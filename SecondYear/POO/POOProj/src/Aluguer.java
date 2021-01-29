import java.io.Serializable;
import java.time.LocalDate;
import java.util.Date;
import java.util.Objects;
import java.util.Random;

enum Preferencia{
    MaisPerto,
    MaisBarato,
    Matricula,
    MaisBarataoMaxDist
}
/**
 * Informação relativa ao aluguer de um veículo.
 */
public class Aluguer implements Serializable {
    private String veiculoMatricula;
    private String emailCliente;
    private LocalDate data;
    private double custoViagem;
    private double distPercorrida;
    private double tempoDeViagem; //Em horas
    private Localizacao destino;
    private TipoCarro tipo;
    private Preferencia preferencia;

    /**
     * Construtor vazio
     */
    public Aluguer(){
        this.veiculoMatricula = null;
        this.emailCliente = null;
        this.data = null;
        this.custoViagem = 0;
        this.distPercorrida = 0;
        this.tempoDeViagem = 0;
        this.destino = new Localizacao();
        this.tipo = null;
        this.preferencia = Preferencia.Matricula;
    }

    /**
     * Construtor parametrizado
     */
    public Aluguer(String v, String  c, LocalDate data, double custoViagem,
                   double distPercorrida,double tempoDeViagem, Localizacao destino, TipoCarro tipo, Preferencia preferencia){
        this.veiculoMatricula = v;
        this.emailCliente = c;
        this.data = data;
        this.custoViagem = custoViagem;
        this.distPercorrida = distPercorrida;
        this.tempoDeViagem = tempoDeViagem;
        this.destino = destino;
        this.tipo = tipo;
        this.preferencia = preferencia;
    }

    /**
     * Construtor de cópia
     */
    public Aluguer(Aluguer a){
        this.veiculoMatricula = a.getVeiculo();
        this.emailCliente = a.getCliente();
        this.data = a.getData();
        this.custoViagem = a.getCustoViagem();
        this.distPercorrida = a.getDistPercorrida();
        this.tempoDeViagem = a.getTempoDeViagem();
        this.destino = a.getDestino();
        this.tipo = a.getTipo();
        this.preferencia = a.getPreferencia();
    }

    /**
     * Devolve o veículo que foi alugado.
     *
     * @return veículo que foi alugado.
     */
    public String getVeiculo() {
        return this.veiculoMatricula;
    }

    /**
     * Devolve o cliente que realizou o aluger.
     *
     * @return cliente que realizou o aluger.
     */
    public String getCliente() {
        return this.emailCliente;
    }



    /**
     * Devolve a data em que ocorreu o aluguer.
     *
     * @return data em que ocorreu o aluguer.
     */
    public LocalDate getData() {
        return this.data;
    }

    /**
     * Devolve o custo da viagem.
     *
     * @return custo da viagem.
     */
    public double getCustoViagem() {
        return this.custoViagem;
    }

    /**
     * Devolve a distância percorrida.
     *
     * @return distância percorrida.
     */
    public double getDistPercorrida() {
        return this.distPercorrida;
    }
    /**
     * Devolve o tempo de viagem.
     *
     * @return Tempo de viagem.
     */
    public double getTempoDeViagem() {
        return this.tempoDeViagem;
    }

    public Localizacao getDestino(){
        return this.destino.clone();
    }
    public TipoCarro getTipo(){
        return  this.tipo;
    }
    /**
     * Devolve a Preferencia.
     *
     * @return Preferencia.
     */
    public Preferencia getPreferencia() {
        return this.preferencia;
    }

    /**
     * Atualiza o veículo do aluguer.
     *
     * @param veiculo novo veículo.
     */
    public void setVeiculo(String veiculo) {
        this.veiculoMatricula = veiculo;
    }

    /**
     * Atualiza o cliente do aluguer.
     *
     * @param cliente novo cliente.
     */
    public void setCliente(String cliente) {
        this.emailCliente = cliente;
    }



    /**
     * Atualiza a data do aluguer.
     *
     * @param data nova data.
     */
    public void setData(LocalDate data) {
        this.data = data;
    }

    public void randomizer(){
        Random random = new Random();
        int minDay = (int) LocalDate.of(2018,1,1).toEpochDay();
        int maxDay = (int) LocalDate.of(2019,12,31).toEpochDay();
        long randomDay = minDay + random.nextInt(maxDay - minDay);
        LocalDate randomDate = LocalDate.ofEpochDay(randomDay);
        this.setData(randomDate);
    }

    /**
     * Atualiza o custo da viagem do aluguer.
     *
     * @param custoViagem novo custo da viagem.
     */
    public void setCustoViagem(double custoViagem) {
        this.custoViagem = custoViagem;
    }

    /**
     * Atualiza a distância percorrida da viagem.
     *
     * @param distPercorrida nova distância percorrida.
     */
    public void setDistPercorrida(double distPercorrida) {
        this.distPercorrida = distPercorrida;
    }

    /**
     * Atualiza o tempo de viagem.
     *
     * @param tempoDeViagem novo tempo de viagem.
     */
    public void setTempoDeViagem(double tempoDeViagem) {
        this.tempoDeViagem = tempoDeViagem;
    }
    public void setDestino(Localizacao destino){this.destino = destino.clone();}
    public void setTipo(TipoCarro tipo){this.tipo = tipo;}
    /**
     * Atualiza a preferencia.
     *
     * @param preferencia nova preferencia.
     */
    public void setPreferencia(Preferencia preferencia){ this.preferencia = preferencia;}

    /**
     * Implementação do método de clonagem de um Aluguer
     *
     * @return objeto do tipo Aluguer
     */
    public Aluguer clone(){
        return new Aluguer(this);
    }
    public String toString(){
        StringBuilder sb = new StringBuilder();
        sb.append("Cliente ").append(this.getCliente());
        sb.append(" Veiculo ");
        sb.append(this.getVeiculo());

        sb.append(" Data ");
        sb.append(this.getData().toString()).append(" \n");
        return  sb.toString();
    }
    /**
     * Implementação do método de igualdade entre dois alugueres
     * Redefinição do método equals de Object.
     *
     //* @param  umAluguer aluguer a ser comparada com o recetor
     * @return true ou false
     */
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Aluguer a = (Aluguer) o;
        return this.custoViagem == a.getCustoViagem() &&
                this.distPercorrida == a.getDistPercorrida() &&
                this.veiculoMatricula.equals(a.getVeiculo()) &&
                this.emailCliente.equals(a.getCliente()) &&
                this.data.equals(a.getData())&&
                this.preferencia.equals(a.getPreferencia())&&
                this.tempoDeViagem == a.getTempoDeViagem();
    }
}
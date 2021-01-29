import java.io.Serializable;
import java.time.LocalDate;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.stream.Collectors;
import java.util.stream.Stream;

/**
 * Veículo do sistema.
 */
public class Veiculo  implements Serializable {
    private String emailProprietario;
    private String matricula;
    private double kmPorHora;
    private double precoPorKM;
    private double scoreVeiculo;
    private int numClassificacoes;
    private List<Aluguer> alugueres;
    boolean disponivel;

    /**
     * Construtor vazio
     */
    public Veiculo(){
        this.emailProprietario = null;
        this.matricula = null;
        this.kmPorHora = 0;
        this.precoPorKM = 0;
        this.scoreVeiculo = 0;
        this.numClassificacoes = 0;
        this.alugueres = new ArrayList<Aluguer>();
        this.disponivel = false;
    }
    /**
     * Construtor parametrizado
     */
    public Veiculo(String p,String matricula, float kmPorHora, float precoPorKM, float scoreVeiculo,int numClassificacoes,ArrayList<Aluguer> alugueres, Boolean disponivel){
        this.emailProprietario = p;
        this.matricula = matricula;
        this.kmPorHora = kmPorHora;
        this.precoPorKM = precoPorKM;
        this.scoreVeiculo = scoreVeiculo;
        this.numClassificacoes = numClassificacoes;
        this.alugueres = alugueres.stream().map(Aluguer::clone).collect(Collectors.toList());
        this.disponivel = disponivel;
    }

    /**
     * Construtor de cópia
     */
    public Veiculo(Veiculo v) {
        this.emailProprietario = v.getProprietario();
        this.matricula = v.getMatricula();
        this.kmPorHora = v.getKmPorHora();
        this.precoPorKM = v.getPrecoPorKM();
        this.scoreVeiculo = v.getScoreVeiculo();
        this.numClassificacoes = v.getNumClassificacoes();
        this.alugueres = v.getAlugueres();
        this.disponivel = v.isDisponivel();
    }
    /**
     * Devolve a faturacao total entre a data1 e a data2.
     * @param data1 data de começo do periodo
     * @param data2 data de fim do periodo
     * @return Faturacao total entre as duas datas.
     */
    public float totalFaturado(LocalDate data1, LocalDate data2){
        List<Aluguer> s= this.alugueres.stream().filter(aluguer -> aluguer.getData().isAfter(data1)).filter(aluguer -> aluguer.getData().isBefore(data2)).collect(Collectors.toList());
        float fact_final = 0;
        for (Aluguer a : s){
            fact_final += a.getCustoViagem();
        }
        return fact_final;
    }


    /**
     * Devolve o proprietário do veículo.
     *
     * @return proprietário do veículo.
     */
    public String getProprietario() {
        return this.emailProprietario;
    }
    /**
     * Devolve a matrícula do carro.
     *
     * @return matricula.
     */
    public String getMatricula() {
        return this.matricula;
    }

    /**
     * Devolve a velocidade (em km/h) do veículo.
     *
     * @return velocidade (em km/h) do veículo.
     */
    public double getKmPorHora() {
        return this.kmPorHora;
    }

    /**
     * Devolve o preço (em euros/km) do veículo.
     *
     * @return preço em (euros/km) do veículo.
     */
    public double getPrecoPorKM() {
        return this.precoPorKM;
    }

    /**
     * Devolve a classificação do veículo.
     *
     * @return classificação do veículo.
     */
    public double getScoreVeiculo() {
        return this.scoreVeiculo;
    }
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
     * Indica se o veículo se encontra disponível para aluguer.
     *
     * @return disponibilidade do veículo.
     */
    public boolean isDisponivel() {
        return this.disponivel;
    }

    /**
     * Atualiza o proprietário do veículo.
     *
     * @param p novo proprietário.
     */
    public void setP(String p) {
        this.emailProprietario = p;
    }
    /**
     * Atualiza a matrícula.
     *
     * @param matricula nova matrícula.
     */
    public void setMatricula(String matricula) {
        this.matricula = matricula;
    }
    /**
     * Atualiza a velocidade (em km/h) do veículo.
     *
     * @param kmPorHora nova velocidade (em km/h).
     */
    public void setKmPorHora(double kmPorHora) {
        this.kmPorHora = kmPorHora;
    }

    /**
     * Atualiza o preço (em euros/h) do veículo.
     *
     * @param precoPorKM novo preço (em euros/h).
     */
    protected void setPrecoPorKM(double precoPorKM) {
        this.precoPorKM = precoPorKM;
    }
    protected void setDisponivel(boolean bool){
        this.disponivel = bool;
    }


    public void setNumClassificacoes(int numClassificacoes){
        this.numClassificacoes = numClassificacoes;
    }
    /**
     * Altera o estatuto de disponibilidade de um veículo.
     */
    public void switchDisponivel() {
        if(this.disponivel) this.disponivel = false;
        else this.disponivel = true;
    }
    /**
     * Atualiza a classificacão do Veiculo.
     *
     * @param classificacao classificacão a adicionar.
     */
    public void insertClassificacao(float classificacao){
       if(this.numClassificacoes == 0) this.scoreVeiculo = classificacao;
        else {
           this.scoreVeiculo = ((this.scoreVeiculo * this.numClassificacoes)
                   + classificacao) / (this.numClassificacoes + 1);
       }this.numClassificacoes++;
    }
    /**
     * Implementação do método de clonagem de um Veiculo
     *
     * @return objeto do tipo Veiculo
     */
    public Veiculo clone(){
        return new Veiculo(this);
    }

    /**
     * Adiciona um aluguer à lista do veiculo.
     *
     * @param a novo aluguer.
     */
    public void addAluguer(Aluguer a) {
        this.alugueres.add(a.clone());
    }
        /**
     * Implementação do método de igualdade entre dois veículos
     * Redefinição do método equals de Object.
     *
     //* @param  umVeiculo veículo a ser comparada com o recetor
     * @return true ou false
     */


    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Veiculo v = (Veiculo) o;
        return this.emailProprietario.equals(v.getProprietario()) &&
        this.matricula.equals(v.getMatricula())&&
                this.kmPorHora == v.kmPorHora &&
                this.scoreVeiculo == v.scoreVeiculo &&
                this.precoPorKM == v.precoPorKM &&
                (this.disponivel == (v.disponivel));
    }
}
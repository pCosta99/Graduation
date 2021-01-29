import java.io.Serializable;
import java.util.ArrayList;
import java.util.List;
import java.util.Objects;

enum TipoCarro{
    Gasolina,
    Electrico,
    Hibrido
}
/**
 * Extensão de um veículo para representar um carro.
 */
public class Carro extends Veiculo implements Serializable {
    private TipoCarro tipo;
    private float consumoPorKm;
    private String marca;
    private String modelo;
    private double qtdCombustivelAtual;
    private double qtdCombustivelMaxima;
    private int autonomiaAtual;
    private int autonomiaMax;
    private Localizacao pos;
    /**
     * Construtor vazio
     */
    public Carro() {
        this.tipo = null;
        this.consumoPorKm = 0;
        this.marca = null;
        this.modelo = null;
        this.qtdCombustivelAtual = 0;
        this.qtdCombustivelMaxima = 0;
        this.pos = new Localizacao();
        this.autonomiaAtual = 0;
        this.autonomiaMax = 0;
        this.disponivel = true;
    }

    /**
     * Construtor parametrizado
     */
    public Carro(String p, float kmPorHora, float precoPorKM, int scoreVeiculo, ArrayList<Aluguer> alugueres, Boolean disponivel,
                 TipoCarro tipo, int consumoPorKm, String matricula, String marca, String modelo, double qtdCombustivelAtual, double qtdCombustivelMaxima,
                 int autonomiaAtual,int autonomiaMax,Localizacao posicao,int numClassificacoes) {
        super(p, matricula, kmPorHora, precoPorKM, scoreVeiculo,numClassificacoes, alugueres, disponivel);
        this.tipo = tipo;
        this.consumoPorKm = consumoPorKm;
        this.marca = marca;
        this.modelo = modelo;
        this.qtdCombustivelAtual = qtdCombustivelAtual;
        this.qtdCombustivelMaxima = qtdCombustivelMaxima;
        this.autonomiaAtual = autonomiaAtual;
        this.autonomiaMax = autonomiaMax;
        this.pos = posicao.clone();
        this.disponivel = disponivel;
    }

    /**
     * Construtor de cópia
     */
    public Carro(Carro c) {
        super(c);
        this.tipo = c.getTipo();
        this.consumoPorKm = c.getConsumoPorKm();
        this.marca = c.getMarca();
        this.modelo = c.getModelo();
        this.qtdCombustivelAtual = c.getQtdCombustivelAtual();
        this.qtdCombustivelMaxima = c.getQtdCombustivelMaxima();
        this.autonomiaAtual = c.getAutonomiaAtual();
        this.autonomiaMax = c.getAutonomiaMax();
        this.pos = c.getPos().clone();
        this.disponivel = c.getDisponivel();
    }
    public void addAluguerCarro(Aluguer a) {
        this.addAluguer(a);
        this.setAutonomiaAtual(this.getAutonomiaAtual() -(int)a.getDistPercorrida());
    }
    /**
     * Devolve o tipo do carro.
     *
     * @return tipo do carro.
     */
    public TipoCarro getTipo() {
        return this.tipo;
    }

    /**
     * Devolve o consumo por quilómetro.
     *
     * @return consumo por quilómetro.
     */
    public float getConsumoPorKm() {
        return this.consumoPorKm;
    }


    /**
     * Devolve a marca do carro.
     *
     * @return marca.
     */
    public String getMarca() {
        return this.marca;
    }

    /**
     * Devolve o modelo do carro.
     *
     * @return modelo.
     */
    public String getModelo() {
        return this.modelo;
    }

    /**
     * Devolve a quantidade de combustível atual.
     *
     * @return quantidade de combustível atual.
     */
    public double getQtdCombustivelAtual() {
        return this.qtdCombustivelAtual;
    }

    /**
     * Devolve a quantidade de combustível máxima.
     *
     * @return quantidade de combustível máxima.
     */
    public double getQtdCombustivelMaxima() {
        return this.qtdCombustivelMaxima;
    }

    public int getAutonomiaAtual() {
        return autonomiaAtual;
    }

    public int getAutonomiaMax() {
        return autonomiaMax;
    }

    public boolean getDisponivel(){
        return this.disponivel;
    }
    /**
     * Devolve a localizacao do Carro.
     *
     * @return localizacao do Carro.
     */
    public Localizacao getPos() {
        return this.pos;
    }

    /**
     * Atualiza o tipo.
     *
     * @param tipo novo tipo.
     */
    public void setTipo(TipoCarro tipo) {
        this.tipo = tipo;
    }

    /**
     * Atualiza o consumo por quilómetro.
     *
     * @param consumoPorKm novo consumo por quilómetro.
     */
    public void setConsumoPorKm(float consumoPorKm) {
        this.consumoPorKm = consumoPorKm;
    }

    /**
     * Atualiza a marca.
     *
     * @param marca nova marca.
     */
    public void setMarca(String marca) {
        this.marca = marca;
    }

    /**
     * Atualiza o modelo.
     *
     * @param modelo novo modelo.
     */
    public void setModelo(String modelo) {
        this.modelo = modelo;
    }



    public void setAutonomiaAtual(int autonomiaAtual){
        this.autonomiaAtual = autonomiaAtual;
        this.qtdCombustivelAtual = autonomiaAtual* this.getConsumoPorKm();
    }
    public  void setAutonomiaMax(int autonomiaMax){
        this.autonomiaMax = autonomiaMax;
        this.qtdCombustivelMaxima = autonomiaMax*this.getConsumoPorKm();
    }
    /**
     * Atualiza a localizacao do carro.
     *
     * @param pos nova localizacao
     */
    public void setPos(Localizacao pos) {
        this.pos = pos.clone();
    }

    /**
     * Implementação do método de clonagem de um Carro
     *
     * @return objeto do tipo Carro
     */

    public Carro clone() {
        return new Carro(this);
    }

    /**
     * Implementação do método de igualdade entre dois carros
     * Redefinição do método equals de Object.
     * <p>
     * //* @param  umCarro carro a ser comparada com o recetor
     *
     * @return true ou false
     */
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        if (!super.equals(o)) return false;
        Carro c = (Carro) o;
        return this.consumoPorKm == c.consumoPorKm &&
                this.qtdCombustivelAtual == c.qtdCombustivelAtual &&
                this.qtdCombustivelMaxima == c.qtdCombustivelMaxima &&
                this.tipo == c.tipo &&
                this.marca.equals(c.marca) &&
                this.modelo.equals(c.modelo);
    }

    public boolean autonomiaAcimaDe10porcento() {
        try {
            if (this.getQtdCombustivelAtual() / this.getQtdCombustivelMaxima() < 0.1) {
                return false;
            } else return(true);
        } catch (Exception e) {
            return (false);
        }
    }
    public boolean autonomiaSuficiente(double distancia){
        if(distancia * this.consumoPorKm > this.getQtdCombustivelAtual()) return false;
        else return true;
    }
    public String toString(){
        StringBuilder sb = new StringBuilder();
        sb.append("Proprietario: "+this.getProprietario());
        sb.append(" Carro : ");
        sb.append(this.getMarca());
        sb.append(" PrecoPorKm: ");
        sb.append(this.getPrecoPorKM());
        sb.append(" Classificacao ");
        sb.append(this.getScoreVeiculo());
        sb.append(" Consumo por KM ");
        sb.append(this.getConsumoPorKm());
        sb.append(" Autonomia Atual ").append(this.getAutonomiaAtual()).append(" Autonomia max ").append(this.getAutonomiaMax()).append("\n");
        return  sb.toString();
    }
}
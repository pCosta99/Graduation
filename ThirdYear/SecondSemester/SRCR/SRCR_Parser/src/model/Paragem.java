package model;

import java.util.Arrays;

public class Paragem {
    private int gid;
    private float latitude;
    private float longitude;
    private String estadoDeConservacao;
    private String tipoDeAbrigo;
    private boolean comPublicidade;
    private String operadora;
    private String[] carreira;
    private int codigoDeRua;
    private String nomeDaRua;
    private String freguesia;

    public Paragem() {
    }

    public Paragem(int gid, float latitude, float longitude, String estadoDeConservacao, String tipoDeAbrigo, boolean comPublicidade, String operadora, String[] carreira, int codigoDeRua, String nomeDaRua, String freguesia) {
        this.gid = gid;
        this.latitude = latitude;
        this.longitude = longitude;
        this.estadoDeConservacao = estadoDeConservacao;
        this.tipoDeAbrigo = tipoDeAbrigo;
        this.comPublicidade = comPublicidade;
        this.operadora = operadora;
        this.carreira = carreira;
        this.codigoDeRua = codigoDeRua;
        this.nomeDaRua = nomeDaRua;
        this.freguesia = freguesia;
    }

    public int getGid() {
        return gid;
    }

    public float getLatitude() {
        return latitude;
    }

    public float getLongitude() {
        return longitude;
    }

    public String getEstadoDeConservacao() {
        return estadoDeConservacao;
    }

    public String getTipoDeAbrigo() {
        return tipoDeAbrigo;
    }

    public boolean isComPublicidade() {
        return comPublicidade;
    }

    public String getOperadora() {
        return operadora;
    }

    public String[] getCarreira() {
        return carreira;
    }

    public int getCodigoDeRua() {
        return codigoDeRua;
    }

    public String getNomeDaRua() {
        return nomeDaRua;
    }

    public String getFreguesia() {
        return freguesia;
    }

    public void setGid(int gid) {
        this.gid = gid;
    }

    public void setLatitude(float latitude) {
        this.latitude = latitude;
    }

    public void setLongitude(float longitude) {
        this.longitude = longitude;
    }

    public void setEstadoDeConservacao(String estadoDeConservacao) {
        this.estadoDeConservacao = estadoDeConservacao;
    }

    public void setTipoDeAbrigo(String tipoDeAbrigo) {
        this.tipoDeAbrigo = tipoDeAbrigo;
    }

    public void setComPublicidade(boolean comPublicidade) {
        this.comPublicidade = comPublicidade;
    }

    public void setOperadora(String operadora) {
        this.operadora = operadora;
    }

    public void setCarreira(String[] carreira) {
        this.carreira = carreira;
    }

    public void setCodigoDeRua(int codigoDeRua) {
        this.codigoDeRua = codigoDeRua;
    }

    public void setNomeDaRua(String nomeDaRua) {
        this.nomeDaRua = nomeDaRua;
    }

    public void setFreguesia(String freguesia) {
        this.freguesia = freguesia;
    }

    @Override
    public String toString(){
        StringBuilder sb = new StringBuilder();
        sb.append("stop(").append(gid).append(",");
        sb.append(latitude).append(",");
        sb.append(longitude).append(",");
        sb.append("'").append(estadoDeConservacao).append("',");
        sb.append("'").append(tipoDeAbrigo).append("',");
        sb.append(comPublicidade).append(",");
        sb.append("'").append(operadora).append("',").append("[");
        for (String s : carreira) sb.append(s).append(",");
        sb.deleteCharAt(sb.length()-1);
        sb.append("],").append(codigoDeRua).append(",");
        sb.append("'").append(nomeDaRua).append("',");
        sb.append("'").append(freguesia).append("')").append(".");
        return sb.toString();
    }
}

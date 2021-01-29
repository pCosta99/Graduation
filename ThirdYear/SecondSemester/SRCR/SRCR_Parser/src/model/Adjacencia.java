package model;

public class Adjacencia {
    /* Dados combinados */
    private int gid_origem;
    private int gid_destino;
    private float distancia;

    /* Dados relativos ao primeiro nodo */
    private String estadoDeConservacao;
    private String tipoDeAbrigo;
    private boolean abrigoComPublicidade;
    private String operadora;
    private String carreira;
    private int codigoDeRua;
    private String nomeDaRua;
    private String freguesia;

    /* Cria um arco entre dois nodos */
    public Adjacencia(Paragem p1, Paragem p2, String carreira){
        /* Dados agregados */
        this.gid_origem = p1.getGid();
        this.gid_destino = p2.getGid();
        this.distancia = (float) Math.sqrt(Math.pow((p1.getLatitude()-p2.getLatitude()),2) + Math.pow((p1.getLongitude()-p2.getLongitude()),2));
        this.carreira = carreira;

        /* Dados possivelmente opcionais */
        this.estadoDeConservacao = p1.getEstadoDeConservacao();
        this.tipoDeAbrigo = p1.getTipoDeAbrigo();
        this.abrigoComPublicidade = p1.isComPublicidade();
        this.operadora = p1.getOperadora();
        this.codigoDeRua = p1.getCodigoDeRua();
        this.nomeDaRua = p1.getNomeDaRua();
        this.freguesia = p1.getFreguesia();
    }

    @Override
    public String toString(){
        StringBuilder sb = new StringBuilder();
        sb.append("connected(").append(gid_origem).append(",");
        sb.append(gid_destino).append(",");
        sb.append(carreira).append(",");
        sb.append(distancia).append(").");
        return sb.toString();
    }
}

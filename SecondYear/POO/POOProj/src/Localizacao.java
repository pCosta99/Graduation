import java.io.Serializable;

/**
 * Localização 2D.
 */
public class Localizacao implements Serializable {
    private double posX;
    private double posY;

    /**
     * Construtor vazio
     */
    public Localizacao(){
        this.posX = 0;
        this.posY = 0;
    }

    /**
     * Construtor parametrizado
     */
    public Localizacao(double posX, double posY){
        this.posX = posX;
        this.posY = posY;
    }

    /**
     * Construtor de cópia
     */
    public Localizacao(Localizacao l){
        this.posX = l.getX();
        this.posY = l.getY();
    }

    /**
     * Devolve a coordenada X.
     *
     * @return coordenada X.
     */
    public double getX(){
        return this.posX;
    }

    /**
     * Devolve a coordenada Y.
     *
     * @return coordenada Y.
     */
    public double getY(){
        return this.posY;
    }

    /**
     * Atualiza a coordenada X da localizacao.
     *
     * @param posX nova coordenada X
     */
    public void setX(double posX){
        this.posX = posX;
    }

    /**
     * Atualiza a coordenada Y da localizacao.
     *
     * @param posY nova coordenada Y
     */
    public void setY(double posY){
        this.posY = posY;
    }

    /**
     * Implementação do método de clonagem de uma Localizacao
     *
     * @return objeto do tipo Localizacao
     */
    public Localizacao clone(){
        return new Localizacao(this);
    }

    /**
     * Implementação do método de igualdade entre duas localizações
     * Redefinição do método equals de Object.
     *
     //* @param  umaLocalizacao localizacao a ser comparada com o recetor
     * @return true ou false
     */
    public boolean equals(Object o){
        if (this == o)
            return true;

        if ((o == null) || (this.getClass() != o.getClass()))
            return false;

        Localizacao loc = (Localizacao) o;
        return(this.posX == loc.getX() && this.posY == loc.getY());
    }


    public double distancia(Localizacao l){
        double x1 = this.getX();
        double x2 = l.getX();
        double y1 = this.getY();
        double y2 = l.getY();

        double distancia = Math.sqrt(Math.pow((y2-y1),2)+Math.pow((x2-x1),2));
        return distancia;
    }
}

package model;

public class Media {
    private int id;
    private Metadata metadata;
    private String path;
    private boolean isMusic;
    private String uploaderID;

    /* Construtor com identificador */
    public Media(int id, Metadata metadata, String path, boolean isMusic, String uploaderID){
        this.id = id;
        this.metadata = metadata;
        this.path = path;
        this.isMusic = isMusic;
        this.uploaderID = uploaderID;
    }


    /* Construtor inicial, sem identificador */
    public Media(Metadata metadata, String path, boolean isMusic, String uploaderID){
        this.id = -1; // Não inicializado
        this.metadata = metadata;
        this.path = path;
        this.isMusic = isMusic;
        this.uploaderID = uploaderID;
    }

    public int getId(){
        return this.id;
    }

    public void setId(int id){
        this.id = id;
    }

    public Metadata getMetadata(){
        return this.metadata;
    }

    public String getPath(){
        return this.path;
    }

    public String getUploaderID(){
        return this.uploaderID;
    }

    public boolean isMusic(){
        return this.isMusic;
    }

    public void setMetadata(Metadata metadata) {
        this.metadata = metadata;
    }

    public String toString(){
        StringBuilder sb = new StringBuilder();
        sb.append("Id: ");
        sb.append(this.id + "\n");
        sb.append(this.metadata.toString());
        sb.append("Path: ");
        sb.append(this.path + "\n");
        sb.append("Tipo: ");
        if(this.isMusic) sb.append("Música\n");
        else sb.append("Vídeo\n");
        return sb.toString();
    }
}

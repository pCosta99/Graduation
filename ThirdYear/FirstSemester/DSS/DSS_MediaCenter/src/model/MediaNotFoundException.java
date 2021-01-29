package model;

/* Usado quando não se descobre o conteúdo procurado */
public class MediaNotFoundException extends Exception{
    public MediaNotFoundException(){super();}
    public MediaNotFoundException(String s){super(s);}
}

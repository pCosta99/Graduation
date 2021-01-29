package model;

public class Pair<T1, T2> {
    private T1 fst;
    private T2 snd;

    public Pair(){

    }

    public Pair(T1 fst, T2 snd){
        this.fst = fst;
        this.snd = snd;
    }

    public T1 getFst() {
        return fst;
    }

    public T2 getSnd() {
        return snd;
    }

    public void setFst(T1 fst) {
        this.fst = fst;
    }

    public void setSnd(T2 snd) {
        this.snd = snd;
    }
}

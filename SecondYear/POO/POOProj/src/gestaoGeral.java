import java.io.*;
import java.time.LocalDate;
import java.util.Date;
import java.util.List;
import java.util.Map;
import java.util.Random;
import java.util.stream.Collectors;

public class gestaoGeral implements Serializable {
    private gestaoUtilizadores utilizadores;
    private gestorCarros carros;

    //método de ler ficheiro de logs construtores etc
    public gestaoGeral() {
        this.utilizadores = new gestaoUtilizadores();
        this.carros = new gestorCarros();
    }
    public gestaoGeral(gestaoGeral gestao) {
        this.utilizadores = gestao.getUtilizadores();
        this.carros = gestao.getCarros();
    }
    public gestaoGeral(Map<String, Cliente> clienteMap, Map<String,Proprietario> proprietarioMap, Map<String ,Carro> carros) {
        this.utilizadores.setClientes(clienteMap);
        this.utilizadores.setProprietarios(proprietarioMap);
        this.carros.setMapa(carros);
    }

    public gestaoUtilizadores getUtilizadores() {
        return this.utilizadores.clone();
    }

    public gestorCarros getCarros() {
        return this.carros.clone();
    }

    public gestaoGeral clone(){
        return new gestaoGeral(this);
    }
    public boolean equals(Object obj) {
        if(obj==this) return true;
        if(obj==null || obj.getClass()!=this.getClass()) return false;
        gestaoGeral g = (gestaoGeral) obj;
        return this.utilizadores.equals(g.getUtilizadores())&&this.carros.equals(g.getCarros());
    }
    public void stringToClassificacao(String[] campos){
        if (campos[0].length() == 8){
            Carro carro = new Carro(carros.getMapa().get(campos[0]));
            carro.insertClassificacao(Float.parseFloat(campos[1]));

            String email = this.utilizadores.getProprietarios().get(carro.getProprietario()).getEmail();
                Proprietario prop = this.utilizadores.getProprietarios().get(email);
                if (prop == null) System.out.println(carro.getProprietario());
                else {
                    prop.insertClassificacao(Float.parseFloat(campos[1]));
                    prop.addVeiculo(carro);
                    Map<String,Carro> mapa = carros.getMapa();
                    mapa.put(carro.getMatricula(),carro.clone());
                    carros.setMapa(mapa);
                    Map<String, Proprietario> mapa2 = this.utilizadores.getProprietarios();
                    mapa2.put(prop.getEmail(), prop.clone());
                    utilizadores.setProprietarios(mapa2);
                }
        }else
        {
            Cliente cliente =this.utilizadores.getClientes().get(campos[0] + "@gmail.com");
            if (cliente == null) ;
            else {
                cliente.insertClassificacao(Float.parseFloat(campos[1]));
                Map<String, Cliente> mapa2 = this.utilizadores.getClientes();
                mapa2.put(cliente.getEmail(), cliente.clone());
                this.utilizadores.setClientes(mapa2);
            }
        }
    }
    public void stringToCarro(String[] campos) {
        Carro carro = new Carro();
        carro.setTipo(TipoCarro.valueOf(campos[0]));
        carro.setMarca(campos[1]);
        carro.setMatricula(campos[2]);
        carro.setKmPorHora(Float.parseFloat(campos[4]));
        carro.setPrecoPorKM(Float.parseFloat(campos[5]));
        carro.setConsumoPorKm(Float.parseFloat(campos[6]));
        carro.setAutonomiaAtual(Integer.parseInt(campos[7]));
        carro.setAutonomiaMax(Integer.parseInt(campos[7]));
        carro.setPos(new Localizacao(Float.parseFloat(campos[8]), Float.parseFloat(campos[9])));
        Proprietario proprietario = utilizadores.getProprietarios().get(campos[3] + "@gmail.com");
        carro.setP(proprietario.getEmail());
        Map<String, Carro> mapacarros = carros.getMapa();
        mapacarros.put(carro.getMatricula(), carro);
        carros.setMapa(mapacarros);
        if (proprietario == null) System.out.println(campos[3]+"@gmail.com");
        else {
            proprietario.addVeiculo(carro);
            Map<String, Proprietario> mapaUtilizadores = utilizadores.getProprietarios();
            mapaUtilizadores.put(proprietario.getEmail(), proprietario.clone());
            utilizadores.setProprietarios(mapaUtilizadores);
        }
    }
    public void classificarCliente(String email, float classificacao) throws Exception_ClienteInexistente {
        Cliente cliente =this.utilizadores.getClientes().get(email);
        if (cliente == null) throw new Exception_ClienteInexistente("Nao existe cliente");
        else {
            cliente.insertClassificacao(classificacao);
            Map<String, Cliente> mapa2 = this.utilizadores.getClientes();
            mapa2.put(email, cliente.clone());
            this.utilizadores.setClientes(mapa2);
        }
    }

    public void classificarProprietarioECarro(Cliente cliente, String Matricula, float classificacao) {
        cliente.removePorClassificar(Matricula);
        Map<String, Cliente> mapa2 = this.utilizadores.getClientes();
        mapa2.put(cliente.getEmail(), cliente.clone());
        this.utilizadores.setClientes(mapa2);
        Carro carro = new Carro(carros.getMapa().get(Matricula));
        carro.insertClassificacao(classificacao);

        String email = this.utilizadores.getProprietarios().get(carro.getProprietario()).getEmail();
        Proprietario prop = this.utilizadores.getProprietarios().get(email);
        if (prop == null) System.out.println(carro.getProprietario());
        else {
            prop.insertClassificacao(classificacao);
            prop.addVeiculo(carro);
            Map<String, Carro> mapa = carros.getMapa();
            mapa.put(carro.getMatricula(), carro.clone());
            carros.setMapa(mapa);
            Map<String, Proprietario> mapa3 = this.utilizadores.getProprietarios();
            mapa3.put(prop.getEmail(), prop.clone());
            utilizadores.setProprietarios(mapa3);
        }
    }


    public static gestaoGeral fromLogFile(BufferedReader br) throws IOException{

        String campos[] = null;
        String line = null;
        gestaoGeral h = new gestaoGeral();
        while((line = br.readLine()) != null){
            campos = line.split(":");
            if (campos.length != 2) ;
           else {
                String campos2[] = campos[1].split(",");
                if (campos[0].equals("NovoProp")) h.utilizadores.stringToProprietario(campos2);
                else if (campos[0].equals("NovoCliente")) h.utilizadores.stringToCliente(campos2);
                else if (campos[0].equals("NovoCarro")) h.stringToCarro(campos2);
                else if (campos[0].equals("Aluguer")) h.stringToAluguer(campos2);
                else if (campos[0].equals("Classificar")) h.stringToClassificacao(campos2);
                else ;
            }
        }
        return h;
    }


    public void removeAluguerEmEspera(Proprietario p, Aluguer aluguer){
        Proprietario prop = this.utilizadores.getProprietarios().get(p.getEmail());
        prop.removeAluguerEmEspera(aluguer);
        Map<String,Proprietario> map = this.utilizadores.getProprietarios();
        map.put(prop.getEmail(),prop.clone());
        this.utilizadores.setProprietarios(map);
    }
    public void stringToAluguer(String[] campos) {
        String email = campos[0] + "@gmail.com";
        Cliente cliente = new Cliente(this.utilizadores.getClientes().get(email));
        Aluguer aluguer = new Aluguer();
        Localizacao destino = new Localizacao(Double.parseDouble(campos[1]),Double.parseDouble(campos[2]));
        aluguer.setCliente(cliente.getEmail());
        aluguer.setDestino(destino.clone());
        aluguer.setTipo(TipoCarro.valueOf(campos[3]));
        aluguer.setPreferencia(Preferencia.valueOf(campos[4]));
        aluguer.randomizer();
        Carro carro = null;
        if (aluguer.getPreferencia().equals(Preferencia.MaisBarato)) {
                carro = this.carros.carroMaisBarato(destino.clone());
            }else if (aluguer.getPreferencia().equals(Preferencia.MaisPerto)) {
                carro = this.carros.carroMaisPerto(cliente.getPos());

            }

        aluguer.setCustoViagem(this.custoViagem(carro,destino));
        aluguer.setDistPercorrida(this.distanciaPercorrida(carro,destino));
        aluguer.setTempoDeViagem(this.tempoViagem(carro,destino));
        aluguer.setVeiculo(carro.getMatricula());
            cliente.addAluguer(aluguer);
            cliente.setPos(aluguer.getDestino());
            carro.addAluguerCarro(aluguer);
            carro.setPos(aluguer.getDestino());
            Proprietario proprietario = this.getUtilizadores().getProprietarios().get(carro.getProprietario());

             proprietario.addVeiculo(carro);
            proprietario.addAluguer(aluguer);


            Map<String, Proprietario> mapaProp = this.utilizadores.getProprietarios();
            mapaProp.put(proprietario.getEmail(),proprietario.clone());
            this.utilizadores.setProprietarios(mapaProp);
            Map<String, Cliente> mapaCliente = this.utilizadores.getClientes();
            mapaCliente.put(cliente.getEmail(),cliente.clone());
            this.utilizadores.setClientes(mapaCliente);
            Map<String,Carro> mapaCarros = this.carros.getMapa();
            mapaCarros.put(carro.getMatricula(),carro.clone());
            this.carros.setMapa(mapaCarros);
    }

        public Carro pedidoAluguer(Cliente cliente,Localizacao destino,Preferencia preferencia, TipoCarro tipo){
            Aluguer aluguer = new Aluguer();
            aluguer.setCliente(cliente.getEmail());
            aluguer.setDestino(destino.clone());
            aluguer.setTipo(tipo);
            aluguer.setPreferencia(preferencia);
            aluguer.randomizer();
        Carro carro = null;
            if (aluguer.getPreferencia().equals(Preferencia.MaisBarato)) {
                carro = this.carros.carroMaisBarato(destino.clone());
            }else if (aluguer.getPreferencia().equals(Preferencia.MaisPerto)) {
                carro = this.carros.carroMaisPerto(cliente.getPos());
            }
            aluguer.setVeiculo(carro.getMatricula());
            aluguer.setCustoViagem(this.custoViagem(carro,destino));
            aluguer.setDistPercorrida(this.distanciaPercorrida(carro,destino));
            aluguer.setTempoDeViagem(this.tempoViagem(carro,destino));
            Proprietario proprietario = this.utilizadores.getProprietarios().get(carro.getProprietario());
            proprietario.addAluguerEmEspera(aluguer);
            proprietario.addVeiculo(carro);

            Map<String, Carro> mapaCarros = this.carros.getMapa();
            mapaCarros.put(carro.getMatricula(), carro.clone());
            this.carros.setMapa(mapaCarros);
            Map<String, Proprietario> mapaProp = this.utilizadores.getProprietarios();
            mapaProp.put(proprietario.getEmail(),proprietario.clone());
            this.utilizadores.setProprietarios(mapaProp);
                return carro.clone();
        }

        public void pedidoMatricula(Cliente cliente, Localizacao destino, Carro carro){
            Aluguer aluguer = new Aluguer();
            aluguer.setCliente(cliente.getEmail());
            aluguer.setDestino(destino);
            aluguer.setTipo(carro.getTipo());
            aluguer.randomizer();
            aluguer.setVeiculo(carro.getMatricula());
            aluguer.setCustoViagem(this.custoViagem(carro,destino));
            aluguer.setDistPercorrida(this.distanciaPercorrida(carro,destino));
            aluguer.setTempoDeViagem(this.tempoViagem(carro,destino));
            Proprietario proprietario = this.utilizadores.getProprietarios().get(carro.getProprietario());
            proprietario.addAluguerEmEspera(aluguer);

            Map<String, Proprietario> mapaProp = this.utilizadores.getProprietarios();
            mapaProp.put(proprietario.getEmail(),proprietario.clone());
            this.utilizadores.setProprietarios(mapaProp);
        }

    public Carro pedidoAluguerMaxDist(Cliente cliente,Localizacao destino, double maxDist){
        Aluguer aluguer = new Aluguer();
        aluguer.setCliente(cliente.getEmail());
        aluguer.setDestino(destino.clone());
        aluguer.setPreferencia(Preferencia.MaisBarataoMaxDist);
        aluguer.randomizer();
        Carro carro = null;
        carro = this.carros.carroMaisBaratoMaxDist(cliente.getPos(),destino.clone(), maxDist);
        aluguer.setTipo(carro.getTipo());
        aluguer.setVeiculo(carro.getMatricula());
        aluguer.setCustoViagem(this.custoViagem(carro,destino));
        aluguer.setDistPercorrida(this.distanciaPercorrida(carro,destino));
        aluguer.setTempoDeViagem(this.tempoViagem(carro,destino));
        Proprietario proprietario = this.utilizadores.getProprietarios().get(carro.getProprietario());
        proprietario.addAluguerEmEspera(aluguer);
        proprietario.addVeiculo(carro);

        Map<String, Carro> mapaCarros = this.carros.getMapa();
        mapaCarros.put(carro.getMatricula(), carro.clone());
        this.carros.setMapa(mapaCarros);
        Map<String, Proprietario> mapaProp = this.utilizadores.getProprietarios();
        mapaProp.put(proprietario.getEmail(),proprietario.clone());
        this.utilizadores.setProprietarios(mapaProp);
        return carro.clone();
    }


public void processaAluguer(Aluguer aluguer, boolean choice){
        Carro carro = this.carros.getMapa().get(aluguer.getVeiculo());
        Proprietario proprietario = this.getUtilizadores().getProprietarios().get(carro.getProprietario());
        if (choice) {
            carro.addAluguerCarro(aluguer);
            carro.setPos(aluguer.getDestino());
            proprietario.addVeiculo(carro);
            Cliente cliente = this.utilizadores.getCliente(aluguer.getCliente());
            cliente.addAluguer(aluguer);
            cliente.setPos(aluguer.getDestino());
            cliente.addPorClassificar(carro.getMatricula());
            proprietario.addAluguer(aluguer);
            Map<String, Cliente> mapaCliente = this.utilizadores.getClientes();
            mapaCliente.put(cliente.getEmail(), cliente.clone());
            this.utilizadores.setClientes(mapaCliente);
            Map<String, Carro> mapaCarros = this.carros.getMapa();
            mapaCarros.put(carro.getMatricula(), carro.clone());
            this.carros.setMapa(mapaCarros);
        }
    Map<String, Proprietario> mapaProp = this.utilizadores.getProprietarios();
    mapaProp.put(proprietario.getEmail(), proprietario.clone());
    this.utilizadores.setProprietarios(mapaProp);

}


public double custoViagem (Carro carro, Localizacao destino){
        Localizacao carroPos = carro.getPos();
        double distancia = carroPos.distancia(destino);
        double precoPorKM = carro.getPrecoPorKM();
        if(distancia>carro.getAutonomiaAtual()) return 0;
        else return (distancia * precoPorKM);

    }
    public double tempoViagem(Carro carro,Localizacao destino){
        Localizacao carroPos = carro.getPos();
        double distancia = carroPos.distancia(destino);
        double kmPorHora =carro.getKmPorHora();
        if(distancia> carro.getAutonomiaAtual()) return 0;
        else return distancia/kmPorHora;

    }
    public double distanciaPercorrida(Carro carro, Localizacao destino){
        Localizacao carroPos = carro.getPos();
        double distancia = carroPos.distancia(destino);
        if (distancia>carro.getAutonomiaAtual()) return 0;
        else return distancia;
    }

    public void adicionarCarro(String p,float kmPorHora, float precoPorKM,TipoCarro tipo, String matricula, String marca, String modelo, int autonomiaMax,Localizacao posicao,float consumoPorKM){
        Carro carro = new Carro();
        carro.setP(p);
        carro.setDisponivel(true);
        carro.setConsumoPorKm(consumoPorKM);
        carro.setAutonomiaAtual(autonomiaMax);
        carro.setKmPorHora(kmPorHora);
        carro.setPrecoPorKM(precoPorKM);
        carro.setTipo(tipo);
        carro.setMatricula(matricula);
        carro.setMarca(marca);
        carro.setModelo(modelo);
        carro.setAutonomiaMax(autonomiaMax);
        Proprietario prop = this.utilizadores.getProprietarios().get(p);
                prop.addVeiculo(carro);
        Map<String,Proprietario> map = this.utilizadores.getProprietarios();
        map.put(prop.getEmail(),prop.clone());
        this.utilizadores.setProprietarios(map);
        Map<String,Carro> mapaCarros = this.carros.getMapa();
        mapaCarros.put(carro.getMatricula(),carro.clone());
        this.carros.setMapa(mapaCarros);

    }

    public void adicionarCliente(String nome, String nif, String email, String password, String morada,double coordX, double coordY){
        Cliente cliente = new Cliente();
        cliente.setNome(nome);
        cliente.setNif(nif);
        cliente.setEmail(email);
        cliente.setPassword(password);
        cliente.setMorada(morada);
        cliente.setPos(new Localizacao(coordX,coordY));
        Map<String,Cliente> mapa = this.utilizadores.getClientes();
        mapa.put(cliente.getEmail(),cliente.clone());
        this.utilizadores.setClientes(mapa);
    }

    public void adicionarProprietario(String nome, String nif,String email, String password, String morada){
        Proprietario proprietario = new Proprietario();
        proprietario.setNome(nome);
        proprietario.setNif(nif);
        proprietario.setEmail(email);
        proprietario.setPassword(password);
        proprietario.setMorada(morada);
        Map<String,Proprietario> mapa = this.utilizadores.getProprietarios();
        mapa.put(proprietario.getEmail(),proprietario.clone());
        this.utilizadores.setProprietarios(mapa);
    }
    public String toString(){
        StringBuilder sb = new StringBuilder();
        sb.append("Utilizadores : ");
        sb.append(this.utilizadores.toString());
        sb.append("Carros: ");
        sb.append(this.carros.toString());
        return  sb.toString();
    }

    public void toFile(String nomeDoficheiro) throws FileNotFoundException, IOException {
        FileOutputStream fos = new FileOutputStream(nomeDoficheiro);
        ObjectOutputStream oos = new ObjectOutputStream(fos);
        oos.writeObject(this);
        oos.flush();
        oos.close();
    }
    public static gestaoGeral carregaEstado(String nomedoficheiro) throws FileNotFoundException, IOException, ClassNotFoundException{
        FileInputStream fis = new FileInputStream(nomedoficheiro);
        ObjectInputStream ois = new ObjectInputStream(fis);
        gestaoGeral h = (gestaoGeral) ois.readObject();
        ois.close();
        return h;
    }

    public Carro abastecimento(Proprietario p, String Matricula, int autonomia){
        Proprietario prop = p.clone();
        if(prop.getListaVeiculos().containsKey(Matricula)) {
            prop.abastecimento(Matricula, autonomia);
            return prop.getListaVeiculos().get(Matricula).clone();
        }else return null;

    }

    public Carro disponibilizarCarro(String matricula,Proprietario proprietario, boolean value) {
        if (proprietario.getListaVeiculos().containsKey(matricula)) {
            Carro novo = proprietario.getListaVeiculos().get(matricula).clone();
            novo.setDisponivel(value);
            Map<String, Proprietario> map = this.utilizadores.getProprietarios();
            map.put(proprietario.getEmail(), proprietario.clone());
            this.utilizadores.setProprietarios(map);
            Map<String, Carro> map2 = this.carros.getMapa();
            map2.put(novo.getMatricula(), novo.clone());
            this.carros.setMapa(map2);
            return novo;
        } else return null;
    }

    public Carro alteraPreco(String matricula,Proprietario proprietario, double value) {
        if (proprietario.getListaVeiculos().containsKey(matricula)) {
            Carro novo = proprietario.getListaVeiculos().get(matricula).clone();
            novo.setPrecoPorKM(value);
            proprietario.addVeiculo(novo);
            Map<String, Proprietario> map = this.utilizadores.getProprietarios();
            map.put(proprietario.getEmail(), proprietario.clone());
            this.utilizadores.setProprietarios(map);
            Map<String, Carro> map2 = this.carros.getMapa();
            map2.put(novo.getMatricula(), novo.clone());
            this.carros.setMapa(map2);
            return novo;
        } else return null;
    }
    public double totalFaturado(String matricula,Proprietario proprietario, LocalDate data1, LocalDate data2){
        double res = 0;
        if (proprietario.getListaVeiculos().containsKey(matricula)) {
            Carro novo = proprietario.getListaVeiculos().get(matricula).clone();
            List<Aluguer> aluguers = novo.getAlugueres().stream().filter(aluguer -> aluguer.getData().isAfter(data1)).filter(aluguer -> aluguer.getData().isBefore(data2)).collect(Collectors.toList());
            for(Aluguer a : aluguers){
                res+= a.getCustoViagem();
            }
            return res;
        } else return (-1);
    }
}

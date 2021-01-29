import java.time.LocalDate;
import java.time.format.DateTimeFormatter;
import java.util.Scanner;

public class Menu {

    public static void menu(gestaoGeral g) {
        Scanner keyboard = new Scanner(System.in);
        String optionLogNew = "20";
        try {
            while (true) {
                System.out.println(" 1- Login ou 2- Criar utilizador ou 3- top 10 clientes");
                optionLogNew = keyboard.nextLine();
                if (optionLogNew.equals("1")) {
                    System.out.println("Login de cliente ou proprietario?");
                    String user = keyboard.next();
                    System.out.println("Insira email e password");
                    String email = keyboard.next();
                    String password = keyboard.next();
                    if (user.equals("cliente")) {
                        Cliente cliente = g.getUtilizadores().loginCliente(email, password);
                        System.out.println("Logado como cliente: " + cliente.toString());
                        while (true) {
                            cliente = g.getUtilizadores().getClientes().get(cliente.getEmail());

                            System.out.println("O que gostaria de fazer?");
                            System.out.println("1 printar lista de carros de um determinado tipo");
                            System.out.println("2 Aluguer sem preferencia de carro");
                            System.out.println("3 Aluguer com preferencia de carro por Matricula");
                            System.out.println("4 Aluguer com distancia max a caminhar com carro mais barato");
                            System.out.println("5 Printar Lista de alugueres realizados");
                            System.out.println("6 Alugueres entre 2 datas");
                            System.out.println("7 Classificacoes em espera");
                            System.out.println("8 Guardar estado");
                            System.out.println("0 Sair");
                            int option = Integer.parseInt(keyboard.next());
                            if (option == 1) {
                                System.out.println("Insira o tipo que deseja");
                                String tipo = keyboard.next();
                                System.out.println(g.getCarros().quantosDoTipo(TipoCarro.valueOf(tipo)).toString());
                            }
                            if (option == 2) {
                                System.out.println("Insira o destino no seguinte formato: CoordenadaX CoordenadaY");
                                String X = keyboard.next();
                                String Y = keyboard.next();
                                double coordX = Double.parseDouble(X);
                                double coordY = Double.parseDouble(Y);
                                Localizacao destino = new Localizacao(coordX, coordY);
                                System.out.println("Insira a preferencia ( MaisBarato ou MaisPerto)");
                                String pref = keyboard.next();
                                Preferencia preferencia = Preferencia.valueOf(pref);
                                System.out.println("Insira um tipo (Gasolina ou Electrico ou Hibrido");
                                TipoCarro tipo = TipoCarro.valueOf(keyboard.next());
                                Carro carro = g.pedidoAluguer(cliente, destino, preferencia, tipo);
                                System.out.println(carro.toString());
                            }
                            if (option == 3) {
                                System.out.println("Insira o destino no seguinte formato: CoordenadaX CoordenadaY");
                                String X = keyboard.next();
                                String Y = keyboard.next();
                                double coordX = Double.parseDouble(X);
                                double coordY = Double.parseDouble(Y);
                                Localizacao destino = new Localizacao(coordX, coordY);
                                System.out.println("Insira a matricula");
                                String matricula = keyboard.next();
                                Carro carro = g.getCarros().getCarro(matricula);
                                if (carro == null) {
                                    System.out.println("Matricula inválida");

                                }
                               else  if(carro.getDisponivel() == false){
                                    System.out.println("Carro indisponivel");

                                }
                                else if (carro.autonomiaSuficiente(carro.getPos().distancia(destino)) && (carro.autonomiaAcimaDe10porcento())&& carro.getDisponivel()) {
                                    g.pedidoMatricula(cliente, destino, carro);
                                }
                                else System.out.println("Carro com autonomia insuficiente, escolha outro");
                            }
                            if (option == 4) {
                                System.out.println("Insira o destino no seguinte formato: CoordenadaX CoordenadaY");
                                String X = keyboard.next();
                                String Y = keyboard.next();
                                double coordX = Double.parseDouble(X);
                                double coordY = Double.parseDouble(Y);
                                Localizacao destino = new Localizacao(coordX, coordY);
                                System.out.println("Insira a distancia maxima disposto a percorrer");
                                double distMax = Double.parseDouble(keyboard.next());
                                Carro carro = g.pedidoAluguerMaxDist(cliente, destino, distMax);
                                if (carro == null) System.out.println("Nao há nenhum carro nessa distancia disponivel");
                                else System.out.println("Carro pedido: " + carro.toString());
                            }
                            if (option == 5) {
                                System.out.println(cliente.getAlugueres().toString());
                            }
                            if (option == 6) {
                                System.out.println("Insira 2 datas no formato d/MM/YYYY d/MM/YYYY");
                                String date1 = keyboard.next();
                                String date2 = keyboard.next();
                                DateTimeFormatter formatter = DateTimeFormatter.ofPattern("d/MM/yyyy");
                                LocalDate data1 = LocalDate.parse(date1, formatter);
                                LocalDate data2 = LocalDate.parse(date2, formatter);
                                System.out.println(cliente.aluguerEntreDatas(data1, data2).toString());
                            }
                            if (option == 7) {
                                if (cliente.getMatriculasPorClassificar().isEmpty())
                                    System.out.println("Nao ha nenhuma classificacao em espera");
                                else {
                                    String matricula = cliente.getMatriculasPorClassificar().get(0);
                                    System.out.println(matricula);
                                    System.out.println("Que classificacao pretende dar ao carro/proprietario?");
                                    float classificacao = Float.parseFloat(keyboard.next());
                                    g.classificarProprietarioECarro(cliente, matricula, classificacao);

                                }
                            }
                            if (option == 8) {
                                g.toFile("src/estado.bak");
                                System.out.println("Ficheiro guardado!");
                            }
                            if (option == 0) break;
                            if (option != 0 && option != 1 && option != 2 && option != 3 && option != 4 && option != 5 && option != 6 && option != 7 && option != 8)
                                System.out.println(" Opcao Invalida");
                        }
                    } else if (user.equals("proprietario")) {
                        Proprietario proprietario = g.getUtilizadores().loginProprietario(email, password);
                        System.out.println("Logado como proprietario " + proprietario.toString());
                        while (true) {
                            proprietario = g.getUtilizadores().getProprietarios().get(proprietario.getEmail());
                            System.out.println("O que gostaria de fazer?");
                            System.out.println("1 printar lista de carros");
                            System.out.println("2 Abastecer carro dando matricula e qtd de kilometros");
                            System.out.println("3 Adicionar um carro");
                            System.out.println("4 guardar estado");
                            System.out.println("5 Printar alugueres");
                            System.out.println("6 Aceitar/Rejeitar aluguer");
                            System.out.println("7 Alugueres entre duas datas");
                            System.out.println("8 Trocar a disponibilidade de um carro");
                            System.out.println("9 Total faturado por um veiculo entre 2 datas");
                            System.out.println("10 Alterar o preço por km de um carro");
                            System.out.println("0 sair ");
                            int option = keyboard.nextInt();
                            if (option == 1) System.out.println(proprietario.getListaVeiculos().toString());
                            else if (option == 2) {
                                System.out.println("Insira matricula e qtd separadas por espaço");
                                String matricula = keyboard.next();
                                String qtd2 = keyboard.next();
                                int qtd = Integer.parseInt(qtd2);
                                Carro carro = g.abastecimento(proprietario, matricula, qtd);
                                if (carro == null) System.out.println("Esse carro não é seu, impossivel abastecer.");
                            } else if (option == 3) {
                                System.out.println("Insira os dados separados por espaços no formato: \n");
                                System.out.println("kmPorHora PrecoPorKM TipoCarro Matricula Marca Modelo AutonomiaMax CoordX CoordY ConsumoPorKM");
                                float kmPorHora = Float.parseFloat(keyboard.next());
                                float precoPorKM = Float.parseFloat(keyboard.next());
                                TipoCarro tipo = TipoCarro.valueOf(keyboard.next());
                                String matricula = keyboard.next();
                                String marca = keyboard.next();
                                String modelo = keyboard.next();
                                int autonomiaMax = Integer.parseInt(keyboard.next());
                                double coordX = Double.parseDouble(keyboard.next());
                                double coordY = Double.parseDouble(keyboard.next());
                                float consumoPorKM = Float.parseFloat(keyboard.next());
                                g.adicionarCarro(proprietario.getEmail(), kmPorHora, precoPorKM, tipo, matricula, marca, modelo, autonomiaMax, new Localizacao(coordX, coordY),consumoPorKM);
                            } else if (option == 4) {
                                g.toFile("src/estado.bak");
                                System.out.println("Ficheiro guardado!");
                            } else if (option == 5) {
                                System.out.println(proprietario.getAlugueres().toString());
                            } else if (option == 6) {
                                if (proprietario.getAlugueresEmEspera().isEmpty())
                                    System.out.println("Nao ha alugueres em espera");
                                else {
                                    Aluguer aluguer = proprietario.getAlugueresEmEspera().get(0);
                                    g.removeAluguerEmEspera(proprietario, aluguer);
                                    boolean choice = true;
                                    System.out.println(aluguer.toString());
                                    System.out.println("Aceitar ou rejeitar? (a ou r)");
                                    String opcao = keyboard.next();
                                    if (opcao.equals("a")) g.processaAluguer(aluguer, true);
                                    else g.processaAluguer(aluguer, false);
                                    System.out.println("Que classificacao pretende dar ao cliente?");
                                    float classificacao = Float.parseFloat(keyboard.next());
                                    g.classificarCliente(aluguer.getCliente(), classificacao);

                                }
                            } else if (option == 7) {
                                System.out.println("Insira 2 datas no formato d/MM/YYYY d/MM/YYYY");
                                String date1 = keyboard.next();
                                String date2 = keyboard.next();
                                DateTimeFormatter formatter = DateTimeFormatter.ofPattern("d/MM/yyyy");
                                LocalDate data1 = LocalDate.parse(date1, formatter);
                                LocalDate data2 = LocalDate.parse(date2, formatter);
                                System.out.println(proprietario.aluguerEntreDatas(data1, data2).toString());
                            }else if(option == 8){
                                System.out.println("Insira Matricula e true/false para a disponibilidade");
                                Carro carro = g.disponibilizarCarro(keyboard.next(),proprietario,keyboard.nextBoolean());
                                if(carro == null){
                                    System.out.println("Matricula inválida ou carro não lhe pertence");
                                }
                            }else if(option == 9){
                                System.out.println("Insira Matricula e 2 datas no formato d/MM/YYYY d/MM/YYYY");
                                String matricula = keyboard.next();
                                String date1 = keyboard.next();
                                String date2 = keyboard.next();
                                DateTimeFormatter formatter = DateTimeFormatter.ofPattern("d/MM/yyyy");
                                LocalDate data1 = LocalDate.parse(date1, formatter);
                                LocalDate data2 = LocalDate.parse(date2, formatter);
                                double res = g.totalFaturado(matricula,proprietario,data1,data2);
                                if(res < 0) System.out.println("Você não possui esse carro");
                                else System.out.println("Total faturado por carro de matricula " + matricula + " é " + res);
                            } else if(option == 10){
                                System.out.println("Insira Matricula e novo preço por km");
                                Carro carro = g.alteraPreco(keyboard.next(),proprietario,Double.parseDouble(keyboard.next()));
                                if(carro == null){
                                    System.out.println("Matricula inválida ou carro não lhe pertence");
                                }else System.out.println(carro.getPrecoPorKM());
                            }
                             else if (option == 0) break;
                            if (option != 0 && option != 1 && option != 2 && option != 3 && option != 4 && option != 5 && option != 6
                                    && option != 7 && option != 8 && option != 9 && option !=10)
                                System.out.println(" Opcao Invalida");
                        }
                    } else System.out.println("Login inválido");
                } else if (optionLogNew.equals("2")) {
                    System.out.println("Cliente ou proprietário?");
                    String option = keyboard.nextLine();
                    if (option.equals("cliente")) {
                        System.out.println("Insira separado por espacos: Nome Nif email password coordX coordY");
                        String nome = keyboard.next();
                        String nif = keyboard.next();
                        String email = keyboard.next();
                        String password = keyboard.next();
                        double x = Double.parseDouble(keyboard.next());
                        double y = Double.parseDouble(keyboard.next());

                        System.out.println("insira morada ");
                        keyboard.nextLine();
                        String morada = keyboard.nextLine();
                        if (g.getUtilizadores().getClientes().containsKey(email))
                            System.out.println("Cliente ja existe com esse email");
                        else g.adicionarCliente(nome, nif, email, password, morada, x, y);
                    } else if (option.equals("proprietario")) {
                        System.out.println("Insira separado por espacos: Nome Nif Email password ");
                        String nome = keyboard.next();
                        String nif = keyboard.next();
                        String email = keyboard.next();
                        String password = keyboard.next();
                        System.out.println("Insira morada");
                        keyboard.nextLine();
                        String morada = keyboard.nextLine();
                        if (g.getUtilizadores().getProprietarios().containsKey(email))
                            System.out.println("Proprietario já existe com esse email");
                        else g.adicionarProprietario(nome, nif, email, password, morada);
                    }
                } else if (optionLogNew.equals("quit")) return;
                else if (optionLogNew.equals("3")) {
                    System.out.println("top 10 gastos \n");
                    String res = g.getUtilizadores().top10gastos();
                    System.out.println(res);
                    System.out.println("top 10 utilizacao\n");
                    String util = g.getUtilizadores().top10utilizacao();
                    System.out.println(util);
                }

                if (!optionLogNew.equals("quit") && !optionLogNew.equals("2") && !optionLogNew.equals("1") && !optionLogNew.equals("3"))
                    System.out.println("Opcao invalida" + optionLogNew);

            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
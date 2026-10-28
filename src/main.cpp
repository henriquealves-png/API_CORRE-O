#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <HTTPClient.h>

String pedirDadosNinjaAPI_porId(unsigned int idNinja);
void printNinja(unsigned int idNinja);

const char *nomeRedeWiFi = "SENAI EDUCACIONAL";
const char *senhaRedeWiFi = "info@134";
String enderecoAPI = "https://dattebayo-api.onrender.com/characters/";

void setup()
{
  Serial.begin(115200);
  WiFi.begin(nomeRedeWiFi, senhaRedeWiFi);

  Serial.println();

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print("#");
    delay(500);
  }

  Serial.printf("\n\rConectado.\n\r");

  randomSeed(10);
}

void loop()
{

  unsigned int idNinjaAleatorio = random(1, 2000);

  printNinja(idNinjaAleatorio);

  delay(5000);
}

String pedirDadosNinjaAPI_porId(unsigned int idNinja)
{
  // usamos a funcao strcat() para concatenar textos, apenas variaveis char, string usamos o +
  // strcat(enderecoAPI, idNinja);

  String resposta = "";
  String enderecoPesquisado = enderecoAPI + idNinja + "/";

  enderecoAPI = enderecoAPI + idNinja + "/";
  Serial.printf("\n\rEndereço a ser pesquisado %s\n\r", enderecoAPI.c_str());
  Serial.printf("\n\rEndereço a ser pesquisado %s\n\r", enderecoPesquisado.c_str());

  HTTPClient textoJsonHTTP;

  textoJsonHTTP.begin(enderecoAPI); // inicia o objeto instanciado do tipo HTTP para pegar textos HTTP na internet
  textoJsonHTTP.begin(enderecoPesquisado); // inicia o objeto instanciado do tipo HTTP para pegar textos HTTP na internet

  int codigoHTTP = textoJsonHTTP.GET(); //* o metodo GET acessa o endereco passado no begin e baixa as informacoes, e retorna
  //* o codigoHTTP de status como 200 para Ok e 404 para pagina não encontrada

  if (codigoHTTP != 200)
  {
    resposta = "Erro ao acessar o endereço: " + codigoHTTP;
    return resposta;
  }

  resposta = textoJsonHTTP.getString();

  textoJsonHTTP.end();

  return resposta;
}

void printNinja(unsigned int idNinja)
{

  JsonDocument docNinja;
  JsonDocument docFiltro;

  docFiltro["name"] = true;
  docFiltro["personal"]["sex"] = true;
  docFiltro["tools"][0] = true;
  docFiltro["affiliation"][0] = true;
  docFiltro["rank"]["ninjaRank"]["Part II"] = true;

  // O [0] indica que é um vetor na API, e usando ele pedimos para comecar na posicao 0

  String jsonVindoDaApiNaruto = pedirDadosNinjaAPI_porId(idNinja);
  DeserializationError codigoStatusJson = deserializeJson(docNinja, jsonVindoDaApiNaruto, DeserializationOption::Filter(docFiltro));

  String nomeNinja = docNinja["name"];
  String sexoNinja = docNinja["personal"]["sex"];

  JsonArray ferramentaNinja = docNinja["tools"]; // usado para exibir vetores da API
  JsonArray aldeiaNinja = docNinja["affiliation"];

  String patenteNinja = docNinja["rank"]["ninjaRank"]["Part II"];

  Serial.printf("\n\rNome do ninja pesquisado: %s\n\r", nomeNinja.c_str());
  Serial.printf("\n\rSexo do ninja pesquisado: %s\n\r", sexoNinja.c_str());

  Serial.print("Ferramentas: ");
  // ferramentaNinja.size() diz que o for vai até o tamanho do vetor
  for (int i = 0; i < ferramentaNinja.size(); i++)
  {
    Serial.print(ferramentaNinja[i].as<const char *>());
    Serial.print(i == (ferramentaNinja.size() - 1) ? "." : ",");
  }

  for (int i = 0; i < aldeiaNinja.size(); i++)
  {
    Serial.print(aldeiaNinja[i].as<const char *>());
    Serial.print(i == (aldeiaNinja.size() - 1) ? "." : ",");
    //* ? "." : "," pede para colocar virgula ou ponto final
  }

  Serial.printf("\n\rPatente do ninja pesquisado: %s\n\r", patenteNinja.c_str());

  jsonVindoDaApiNaruto.clear(); //! excluindo objeto String para desalocar memoria pq nao usaremos mais
#include <iostream>
#include <fstream>
#include <vector>
#include <bits/stdc++.h>

using namespace std;
typedef struct dupla{
	string key;
	string value;
} dupla;

void escreverArquivo(char* arquivo, string entrada);
void Init(char* arquivo, char* saida);
void analiseLexica(string arquivo, int numerolinha);
void concatenaToken(char simbolo);
void adicionaToken(string key);
bool isNumber(char simbolo);
bool isAlpha(char simbolo);
string palavrasReservadas[] = {"A", "Acenda", "Acesa", "Aguarde", "Apagada", "Apague", "Ate", "Bloqueada", "como", "definainstrucao",
"direita", "esquerda", "entao", "enquanto", "execucaoinicio", "faca", "fim", "fimexecucao", "fimpara", "fimprograma", "fimrepita", "fimse",
"fimsenao", "Finalize", "Frente", "inicio", "Lampada", "mova", "Movimentado", "Ocupado", "Para", "Parado", "Pare", "passo", "passos", "programainicio", "Pronto",
"repita", "Robo", "se", "senao", "vezes", "Vire"};
vector<dupla> tokens;
string token;

bool isReserved(string palavra){
    string entrada = palavra;
    transform(entrada.begin(), entrada.end(), entrada.begin(), ::toupper);
	for (int i = 0; i<9; i++){
	    string palavrareservada = palavrasReservadas[i];
	    transform(palavrareservada.begin(), palavrareservada.end(), palavrareservada.begin(), ::toupper);
		if (palavrareservada == entrada)
			return true;
	}
	return false;
}

bool isNumber(char simbolo){
	if (simbolo > 47 && simbolo < 58)
		return true;
	return false;
}

bool isAlpha(char simbolo){
	if ((simbolo > 64 && simbolo < 91) || (simbolo > 96 && simbolo < 123))
		return true;
	return false;
}

void concatenaToken(char simbolo){
	
		token += simbolo;
	
	
}

void adicionaToken(string key){
	dupla auxiliar;
	if (key == "?"){
		if (isReserved(token))
			key = "RESERVADA";
		else
			key = "IDENTIFICADOR";
	}
	auxiliar.key = key;
	auxiliar.value = token;
	tokens.push_back(auxiliar);
	token.clear();
}

void Init(char* arquivo, char* saida){
	string line;
	string texto;
	ifstream myfile (arquivo);
	int nlinha = 1;
	if (myfile.is_open())
	{
		while ( getline (myfile,line) )
		{
			line += " ";
			analiseLexica(line, nlinha);
			nlinha++;
		}
		myfile.close();
		escreverArquivo(saida, texto);
	}

	else cout << "Erro ao encontrar arquivo." << endl; 
}

void escreverArquivo(char* arquivo, string entrada){
	ofstream myfile (arquivo);
	
	if (myfile.is_open())
	{
		for (int i = 0; i < tokens.size(); i++)
			myfile << tokens[i].value << " \"" << tokens[i].key << "\"" << endl;
		myfile.close();
	}

	else cout << "Erro ao encontrar arquivo." << endl; 
}

void analiseLexica(string linha, int numerolinha){
	int state = 0;
	for (int i = 0; i<linha.length(); i++){
		switch(state){
			case 0: //caracter especial
				if (isNumber(linha[i])){
					concatenaToken(linha[i]);
					state = 1;
					continue;
				}
				else if (isAlpha(linha[i])){
					concatenaToken(linha[i]);
					state = 2;
					continue;
				}
				else if (linha[i] == '#'){
					return;
				}
				else if (linha[i] == 9 || linha[i] == 32 || linha[i] == 11 || linha[i] == 13 || linha[i] == 10){ 
					continue;
				}
				else{
					concatenaToken(linha[i]);
					state = 3;
					continue;
				}
			break;
			case 1: //numero
				if (isNumber(linha[i])){
					concatenaToken(linha[i]);
					continue;
				}
				else if(linha[i] == 32){
					adicionaToken("NUMERO");
					state = 0;
				}
				else{
					concatenaToken(linha[i]);
					state = 3;
				}
			break;
			case 2: //alpha
				if (isAlpha(linha[i]) || isNumber(linha[i])){
					concatenaToken(linha[i]);
					continue;
				}
				else if(linha[i] == 32 || linha[i] == 13 ){
					adicionaToken("?");
					state = 0;
				}
				else{
					concatenaToken(linha[i]);
					state = 3;
				}
			break;
			case 3: // Erro
				if (linha[i] == 32){
				    //string erro = "Entrada invalida na linha " + to_string(numerolinha) + ", coluna " + to_string(i);
					cout << "Entrada invalida na linha " + to_string(numerolinha) + ", coluna " + to_string(i) << endl;
					adicionaToken("ERRO");
					state = 0;
					continue;
				}
				else{
					concatenaToken(linha[i]);
					continue;
				}
			break;
		}
	}
	 
	int final = linha.length() -1;
	
	if (linha[final] != 9 && linha[final] != 10 && linha[final] != 11 && linha[final] != 32 && linha[final] != 13) {
		if (state == 1){
		    adicionaToken("NUMERO");
		}
		else if (state == 2){
		    adicionaToken("?");
		}
		else if (state == 3){
		    string erro = "Entrada invalida na linha " + to_string(numerolinha) + ", coluna " + to_string(linha.length());
			adicionaToken(erro);
		}
	}
}

int main(int argc, char* argv[]){
	Init(argv[1], argv[2]);
 //    string ent = "(AbacaTe DeF DEF deF Louco 1 x1 12! 1x1asdfas 5 ) def 1x1asdfas";
	// analiseLexica(ent, 1);
	// for (int i = 0; i < tokens.size(); i++)
	//     cout << tokens[i].value << " \"" << tokens[i].key << "\"" << endl;
}
#include <iostream>
using namespace std;

class Arvore;
class Node
{
	friend Arvore;
	Node *esquerda, *direita;
	int dado;
	bool dadoPreenchido;

public:
	Node()
	{
		dadoPreenchido = false;
		esquerda = NULL;
		direita = NULL;
	}
	Node(int _dado)
	{
		dado = _dado;
		dadoPreenchido = true;
		esquerda = NULL;
		direita = NULL;
	}
	///
	~Node() {}
};

class Arvore
{
	Node *raiz;
public:
	Arvore()
	{
		raiz = new Node();
	}
	Arvore(int dado)
	{
		raiz = new Node(dado);
	}
	~Arvore()
	{
		if (raiz)
			delete raiz;
	}

	void adiciona(int novoDado)
	{
		if (!(raiz->dadoPreenchido))
		{
			raiz->dado = novoDado;
			raiz->dadoPreenchido = true;
			return;
		}

		//Procura o nó adequado para colocar o novo dado
		Node* tmp = raiz;
		Node* pai;
		while (tmp != NULL)
		{
			pai = tmp;
			if (novoDado > tmp->dado)
				tmp = tmp->direita;
			else if (novoDado < tmp->dado)
				tmp = tmp->esquerda;
			else
				return;
		}

		tmp = new Node(novoDado);
		if (pai->dado > novoDado)
			pai->esquerda = tmp;
		else
			pai->direita = tmp;
	}


	void imprime()
	{
		cout << "Raiz: ";
		if (raiz->dadoPreenchido)
			cout << raiz->dado;
		else
			cout << "NULL";
		cout << "  FE: ";
		if (raiz->esquerda)
			cout << raiz->esquerda->dado;
		else
			cout << "NULL";
		cout << "  FD: ";
		if (raiz->direita)
			cout << raiz->direita->dado;
		else
			cout << "NULL";
		cout << endl;

		Node* nos[100];
		int nosFaltantes = 0;
		if (raiz->esquerda)
		{
			nos[nosFaltantes] = raiz->esquerda;
			nosFaltantes++;
		}
		if (raiz->direita)
		{
			nos[nosFaltantes] = raiz->direita;
			nosFaltantes++;
		}
		int noAtual = 0;
		while (nosFaltantes > noAtual)
		{
			cout << "No: " << nos[noAtual]->dado;

			cout << "  FE: ";
			if (nos[noAtual]->esquerda)
			{
				cout << nos[noAtual]->esquerda->dado;
				nos[nosFaltantes++] = nos[noAtual]->esquerda;
			}
			else
				cout << "NULL";

			cout << "  FD: ";
			if (nos[noAtual]->direita)
			{
				cout << nos[noAtual]->direita->dado;
				nos[nosFaltantes++] = nos[noAtual]->direita;
			}
			else
				cout << "NULL";
			cout << endl;
			noAtual++;
		}
	}

	void remove(int dado)
	{
		Node* pai = NULL;
		Node* noParaRemover = procuraNoRemover(dado, &pai);
		cout << "No para remover: " << noParaRemover << endl;
		if (!noParaRemover)
			return;
		int tipo = determinaTipo(noParaRemover);
		cout << "Tipo: " << tipo << endl;
		switch (tipo)
		{
			case 0:
				//Nó sem filhos
				removeNoSemFilhos(noParaRemover, pai);
				break;
			case 1:
				removeNoFilhoEsquerda(noParaRemover, pai);
				break;
			case 2:
				removeNoFilhoDireita(noParaRemover, pai);
				break;
			case 3:
				removeNoDoisFilhos(noParaRemover, pai);
				break;
		}
	}

private:
	Node* procuraNoRemover(int dado, Node** pai)
	{
		Node* tmp = raiz;
		while (tmp->dado != dado)
		{
			*pai = tmp;
			if (dado > tmp->dado)
				tmp = tmp->direita;
			else if (dado < tmp->dado)
				tmp = tmp->esquerda;

			//Se chegar ao fim da árvore e não encontrou, então a árvore não possui este dado
			if (tmp == NULL)
				return NULL;
		}

		return tmp;
	}

	//Determina se o nó tem filhos e onde estão
	int determinaTipo(Node* noParaRemover)
	{
		if (noParaRemover->esquerda)
		{
			if (noParaRemover->direita)
				return 3;
			else
				return 1;
		}
		else
			if (noParaRemover->direita)
				return 2;
		return 0;
	}

	void removeNoSemFilhos(Node* noParaRemover, Node* pai)
	{
		//Nó a ser removido não é a raiz
		if (pai)
		{
			//O nó a ser removido é um filho da esquerda
			if (pai->esquerda == noParaRemover)
				pai->esquerda = NULL;

			//O nó a ser removido é um filho da direita
			else
				pai->direita = NULL;
			
			delete noParaRemover;
			return;
		}

		else
			raiz->dadoPreenchido = false;
	}

	//O nó a ser removido tem um filho só e está na esquerda
	void removeNoFilhoEsquerda(Node* noParaRemover, Node* pai)
	{
		//Nó não é raiz
		if (pai)
		{
			if (pai->esquerda == noParaRemover)
				pai->esquerda = noParaRemover->esquerda;
			else
				pai->direita = noParaRemover->esquerda;

			delete noParaRemover;
		}
		else
		{
			Node* aux = noParaRemover->esquerda;
			delete noParaRemover;
			raiz = aux;
		}
	}

	//O nó a ser removido tem um filho só e está na direita
	void removeNoFilhoDireita(Node* noParaRemover, Node* pai)
	{
		if (pai)
		{
			//O nó a ser removido é um filho da esquerda
			if (pai->esquerda == noParaRemover)
				pai->esquerda = noParaRemover->direita;

			//O nó a ser removido é um filho da direita
			else
				pai->direita = noParaRemover->direita;

			delete noParaRemover;		
		}
		else
		{
			Node* aux = noParaRemover->direita;
			delete noParaRemover;
			raiz = aux;
		}
	}


	//O nó a ser removido tem 2 filhos
	void removeNoDoisFilhos(Node* noParaRemover, Node* pai)
	{
		Node* tmp = noParaRemover->esquerda;
		//Procura o último nó a direita do filho à esquerda do nó a ser removido para remoção por fusão
		while (tmp->direita != NULL)
			tmp = tmp->direita;
		tmp->direita = noParaRemover->direita;
		
		if (pai)
		{

			if (pai->esquerda == noParaRemover)
				pai->esquerda = noParaRemover->esquerda;
			else
				pai->direita = noParaRemover->esquerda;

			delete noParaRemover;
		}
		else
		{
			Node* aux = noParaRemover->esquerda;
			delete noParaRemover;
			raiz = aux;
		}
	}
};

void menu()
{
	cout << "1.Adiciona" << endl;
	cout << "2.Remove" << endl;
	cout << "3.Imprime" << endl;
	cout << "4.Termina" << endl;
}


int main()
{
	Arvore arvore;
	int opcao, dado;
	
	do
	{
		menu();
		cin >> opcao;
		opcao--;

		if (!opcao)
		{		
			cout << "Novo dado: " << endl;
			cin >> dado;
			arvore.adiciona(dado);
		}
		else if (opcao == 1)
		{
			cout << "Dado a remover" << endl;
			cin >> dado;
			arvore.remove(dado);
		}
		else if (opcao == 2)
			arvore.imprime();
		
	} while (opcao != 3);


	return 0;
}

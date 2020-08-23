//Grupo:
//Alvaro Barbosa de Paula - 201611006
//Lucas Floriano Bernardes de Castro - 201611486

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

struct sVert
{
    int id;
    int tipo;
    int risco;
    int adjacentes;
    int* lista;
    bool percorre;
};

struct sTipos
{
    int* vertId;
    int quantia;
};

class cGrafo
{
private:
    sVert* listAdj;
    sTipos* lisTipos;// agrupa os pontos de interesse
    int tamanho;
public:
    cGrafo(int x);
    ~cGrafo();
    void print();
    void getVert(int x, int y);
    void getAdj(int x, int y);
    void geTipo(int x, int y);
    void propagar();
    void propagar(int id, int valor);
    void ciclo(int restricao);
};

cGrafo::cGrafo(int x)
{
    tamanho=x;
    listAdj = new sVert [tamanho];
    lisTipos = new sTipos [4];
    lisTipos[0].quantia=0;
    lisTipos[1].quantia=0;
    lisTipos[2].quantia=0;
    lisTipos[3].quantia=0;
}

cGrafo::~cGrafo()
{
    for (int i=0; i<tamanho; i++)
    {
        listAdj[i].lista = NULL;
        delete listAdj[i].lista;
    }

    lisTipos[0].vertId = NULL;
    delete lisTipos[0].vertId;
    lisTipos[1].vertId = NULL;
    delete lisTipos[1].vertId;
    lisTipos[2].vertId = NULL;
    delete lisTipos[2].vertId;
    lisTipos[3].vertId = NULL;
    delete lisTipos[3].vertId;

    listAdj = NULL;
    delete listAdj;
    lisTipos = NULL;
    delete lisTipos;
}

void cGrafo::getVert(int id,int tipo)
{
    listAdj[id].id=id;
    listAdj[id].tipo=tipo;
    listAdj[id].adjacentes=0;
    listAdj[id].percorre=false;
    listAdj[id].risco=1;// risco base de cada vertice
    if (tipo!= 0) geTipo(id, tipo);// se for um ponto de interece adiciona a lisTipos
}


void cGrafo::getAdj(int id,int adj)//adiciona os vertices adjacentes
{
    int tam=listAdj[id].adjacentes;
    if (tam==0)
    {
        int* vet = new int [1];
        vet[0]=adj;
        listAdj[id].adjacentes=1;
        listAdj[id].lista=vet;
    }
    else
    {
        int* vet = new int [tam+1];
        vet[tam]=adj;
        for (int i=0; i<tam; i++)
        {
            vet[i]=listAdj[id].lista[i];
        }
        listAdj[id].adjacentes=tam+1;
        listAdj[id].lista=vet;
    }
}

void cGrafo::geTipo(int id, int tipo)// adiciona o vertice a lisTipos
{
    int tam=lisTipos[tipo-1].quantia;
    if (tam==0)
    {
        int* vet = new int [1];
        vet[0]=id;
        lisTipos[tipo-1].quantia=1;
        lisTipos[tipo-1].vertId=vet;
    }
    else
    {
        int* vet = new int [tam+1];
        vet[tam]=id;
        for (int i=0; i<tam; i++)
        {
            vet[i]=lisTipos[tipo-1].vertId[i];
        }
        lisTipos[tipo-1].quantia=tam+1;
        lisTipos[tipo-1].vertId=vet;
    }
}

void cGrafo::print() ///  lista os vertices e seus vertices adjacentes
{
    for (int i=0; i<tamanho; i++)
    {
        cout << "- id = " << i+1 << " tipo = " << listAdj[i].tipo << " risco = "
             << listAdj[i].risco << endl<< "-- Adjacentes:";
        for (int j=0; j<listAdj[i].adjacentes; j++)
        {
            cout << listAdj[i].lista[j]+1 << " ";
        }
        cout << endl;
    }
}

void cGrafo::propagar(int id, int valor)
{
    if (listAdj[id].risco <= valor)// se o valor eh de rico do vertice ja eh maior ou igual n precisa alterar
    {
        listAdj[id].risco=valor;
        if(valor > 3)// se for menor que tres n eh necessario continuar
        {
            for (int j=0; j<listAdj[id].adjacentes; j++)
            {
                propagar(listAdj[id].lista[j], valor-3);//passa para os vertices adjacentes
            }
        }
    }
}

void cGrafo::propagar()//atribui os valores aos vertices
{
    for (int i=1; i<4; i++)
    {
        for (int j=0; j<lisTipos[i].quantia; j++)
        {
            if (i==1) propagar(lisTipos[i].vertId[j], 6);
            else if (i==2) propagar(lisTipos[i].vertId[j], 12);
            else propagar(lisTipos[i].vertId[j], 21);
        }
    }
}

void cGrafo::ciclo(int restricao)
{
    int ciclo[2][tamanho+1];//cria um vetor para armazenar o ciclo
    for (int i=0; i<tamanho+1; i++)
    {
        ciclo[0][i] = -1;// onde fica o vertice
        ciclo[1][i] = 0;// qual vertice adjacente ja foi usado para definir o proximo
    }
    // vertId pode ser trocado por outro numero caso quera usar outra casa como ponto inicial
    // 0->n-1 onde ene é o numero de possiveis casas, pntos roxos
    ciclo[0][0] = lisTipos[0].vertId[0];
    // baseado no algoritimo de backtracking
    // usa backtraking para encontrar um ciclo pelo grafo
    bool completou=false;
    int quantidade=1;
    //while para quando encontra um ciclo ou quando n eh possivel encontrar um ciclo
    while (!completou)
    {
        int atual=ciclo[0][quantidade-1];
        int adj = listAdj[atual].adjacentes;
        int proximo, tmp=-1;
        for (int i=0; i<adj; i++)
        {
            int abrev=listAdj[atual].lista[i];
            int hist=ciclo[1][quantidade-1];
            // detecta se tem um proximo vertice q ainda n foi testado e ja n esta no ciclo, e que esta dentro da restricao
            if (!(hist&(1<<i)) and !(listAdj[abrev].percorre) and (listAdj[abrev].tipo<=restricao)) tmp=i;
        }
        if (tmp!=-1)
        {
            proximo=listAdj[atual].lista[tmp];
            for (int i=0; i<adj; i++)//testa se tem um adjacente ainda n testado com valor de risco menor
            {
                int abrev=listAdj[atual].lista[i];
                int hist=ciclo[1][quantidade-1];
                if (!(hist&&(1<<i)) and (listAdj[proximo].risco<listAdj[abrev].risco) and !(listAdj[abrev].percorre) and (listAdj[abrev].tipo<=restricao))
                {
                    proximo=listAdj[atual].lista[i];
                    tmp=i;
                }
            }
            //atualiza a lista
            int marca = +(1<<tmp);
            ciclo[0][quantidade]=proximo;
            ciclo[1][quantidade-1]+=marca ;
            listAdj[proximo].percorre=true;
            quantidade++;
            if (ciclo[0][0]==ciclo[0][quantidade-1]) completou = true;
        }
        else// caso n tenha como seguir em frente remove o atual e retorna ao vertice anterior
        {
            ciclo[0][quantidade-1]=-1;
            ciclo[1][quantidade-1]=0;
            listAdj[atual].percorre=false;
            quantidade--;
            if (quantidade==0) completou = true;
        }
    }
    if (quantidade==0)
    {
        cout << " erro: n foi possivel encontrar um ciclo" << endl;
    }
    else if(quantidade==tamanho+1)//caso o backtracking encontro o ciclo hamiltoniano, podemos sonhar
    {
        int riscoFinal=0;
        for (int i=0; i<quantidade; i++)
        {
            riscoFinal+=listAdj[ciclo[0][i]].risco;
            cout << ciclo[0][i]+1 << " ";
        }
        cout << endl;
        cout << "--Risco final = " << riscoFinal/quantidade << endl;
        cout << "--Vertices percorridos = " << quantidade << endl;
    }
    else//baseado em vertice mais proximo, tenta adicionar os vertices ainda n inseridos no ciclo
    {
        int riscoFinal=0;
        for (int i=0; i<quantidade; i++)
        {
            riscoFinal+=listAdj[ciclo[0][i]].risco;
            cout << ciclo[0][i]+1 << " ";
        }
        cout << endl;
        cout << "Vertices percorridos = " << quantidade << endl;
        completou=false;
        int complemento[2][tamanho+1];
        int quantCompl;
        int pos=0, anterior=-1;
        bool encaixa=false, alterou=false;
        while (!completou)
        {
            for (int i=0; i<tamanho+1; i++)//cria um vetor para poder armazenar um possivel caminho entre os vertices
            {
                complemento[0][i] = -1;// onde fica o vertice
                complemento[1][i] = 0;// qual vertice adjacente ja foi usado para definir o proximo
            }
            quantCompl=0;
            bool encontrou=false;
            if (alterou)// se uma alteração foi feita no ciclo, vertices anteriormente n acessiveis podem ser acessiveis agora
            {
                pos=0;
                alterou=false;
            }
            bool ehAdj=false;
            int tmpId;
            while (!encontrou)// busca um vertice fora do ciclo adjacente ao ciclo
            {
                //testa se um dos vertices adjacentes esta no ciclo
                for (int i=0; i<listAdj[pos].adjacentes; i++)
                {
                    tmpId =listAdj[pos].lista[i];
                    if (listAdj[tmpId].percorre and (!listAdj[pos].percorre) and listAdj[pos].tipo<=restricao and anterior!=pos)
                    {
                        ehAdj=true;
                        encontrou = true;
                    }
                }
                if (!ehAdj) pos++;
                if (pos==tamanho)
                {
                    completou=true;// se pos igual a tamanho, n tem mais nada para fazer
                    encontrou=true;
                }
            }
            if (encontrou and ehAdj)
            {
                anterior= pos;
                complemento[0][0]=pos;
                encontrou=true;
                quantCompl++;
                listAdj[pos].percorre=true;
            }
            //testa se oq esta em complemento encaixa no ciclo, caso n complemento eh expandido
            encaixa=false;
            while(!encaixa and !completou)
            {
                int vert1;
                for (int i=0; i<quantidade-1; i++)
                {
                    for (int j=0; j<listAdj[ciclo[0][i]].adjacentes; j++)
                    {
                        for (int k=0; k<listAdj[ciclo[0][i+1]].adjacentes; k++)
                        {
                            //encotra um espaço entre dois vertices do ciclo que o coclo encontra, caso exista
                            if (listAdj[ciclo[0][i]].lista[j]==complemento[0][0] and listAdj[ciclo[0][i+1]].lista[k]==complemento[0][quantCompl-1])
                            {
                                encaixa=true;
                                vert1=i;
                            }
                        }
                    }
                }
                if (encaixa)
                {
                    for (int i=quantidade-1; i>vert1; i--)//move o vertices em ciclo para frente
                    {
                        ciclo[0][i+quantCompl]=ciclo[0][i];
                        ciclo[1][i+quantCompl]=ciclo[1][i];
                    }
                    cout << "-add = ";
                    for (int i=0; i<quantCompl; i++)//encaixa os vertices nas posicoes abertas
                    {
                        cout << complemento[0][i]+1 << " ";
                        ciclo[0][vert1+i+1]=complemento[0][i];
                        ciclo[1][vert1+i+1]=complemento[1][i];
                    }
                    cout << endl;
                    quantidade+=quantCompl;
                    alterou = true;
                }
                else // backtracking 2.0 electric boogaloo
                {
                    //busca um novo vertice adjacente ao vertice anterior para add ao complemento
                    int atual=complemento[0][quantCompl-1];
                    int adj = listAdj[atual].adjacentes;
                    int proximo, tmp=-1;
                    for (int i=0; i<adj; i++)
                    {
                        int abrev=listAdj[atual].lista[i];
                        int hist=complemento[1][quantCompl-1];
                        // detecta se tem um proximo vertice q ainda n foi testado e n esta no ciclo, e que esta dentro da restricao
                        if (!(hist&(1<<i)) and !(listAdj[abrev].percorre) and (listAdj[abrev].tipo<=restricao)) tmp=i;
                    }
                    if (tmp!=-1)
                    {
                        proximo=listAdj[atual].lista[tmp];
                        for (int i=0; i<adj; i++)//testa se tem um adjacente ainda n testado com valor de risco menor
                        {
                            int abrev=listAdj[atual].lista[i];
                            int hist=complemento[1][quantCompl-1];
                            if (!(hist&&(1<<i)) and (listAdj[proximo].risco<listAdj[abrev].risco) and !(listAdj[abrev].percorre) and (listAdj[abrev].tipo<=restricao))
                            {
                                proximo=listAdj[atual].lista[i];
                                tmp=i;
                            }
                        }
                        //atualiza o complemento
                        int marca = +(1<<tmp);
                        complemento[0][quantCompl]=proximo;
                        complemento[1][quantCompl-1]+=marca ;
                        listAdj[proximo].percorre=true;
                        quantCompl++;
                    }
                    else// caso n tenha como seguir em frente remove o atual e retorna ao vertice anterior
                    {
                        complemento[0][quantCompl-1]=-1;
                        complemento[1][quantCompl-1]=0;
                        listAdj[atual].percorre=false;
                        quantCompl--;
                        if (quantCompl==0) encaixa = true;
                    }
                }
            }
        }
         riscoFinal=0;
        for (int i=0; i<quantidade; i++)
        {
            riscoFinal+=listAdj[ciclo[0][i]].risco;
            cout << ciclo[0][i]+1 << " ";
        }
        cout << endl;
        cout << "Risco final = " << riscoFinal/quantidade << endl;
        cout << "Vertices percorridos = " << quantidade << endl;
    }
}

int main ()
{
    stringstream fileLine;
    string tmp, line;
    ifstream myFile ("mapa03.txt");//trocar o nome o arquivo para mudar o fonte do grafo
    int total, id, num, tipo;
    getline (myFile, line);
    fileLine << line;// passa a linha para o stringstream para poder ler palavra por palavra
    fileLine >> tmp;// passa o int para tmp
    stringstream(tmp) >> total;
    tmp = "";// garante q tmp esta vazio antes de passar algum valor
    cGrafo mapa(total);
    if (myFile.is_open())
    {
        for (int i=0; i<total; i++)
        {
            fileLine.clear();
            getline (myFile, line);
            fileLine << line;// passa a linha para o stringstream para poder ler palavra por palavra
            fileLine >> tmp;// passa o id para tmp
            stringstream(tmp) >> id;
            tmp = "";
            fileLine >> tmp;// passa o tipo para tmp
            stringstream(tmp) >> tipo;
            tmp = "";
            mapa.getVert(id-1,tipo);
            while (!fileLine.eof())// checa se ainda tem algo no stringstream
            {
                fileLine >> tmp;
                if (stringstream(tmp) >> num) //caso tenha passa para num
                {
                    mapa.getAdj(id-1, num-1);
                }
                tmp = "";
            }
            cout << endl;
       }
        myFile.close();
    }
    else cout << "Unable to open file";
    mapa.propagar();
    //mapa.print();
    mapa.ciclo(1);// 1->4 quanto maior menor a restricao
  return 0;
}

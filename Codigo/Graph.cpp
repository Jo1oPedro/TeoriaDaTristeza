#include "Graph.h"
#include "Node.h"
#include "Edge.h"
#include <iostream>
#include <fstream>
#include <stack>
#include <queue>
#include <list>
#include <vector>
#include <math.h>
#include <cstdlib>
#include <ctime>
#include <float.h>
#include <iomanip>
#include <algorithm> /// fun��o find
using namespace std;
#define INFINITO 1000000000;
#include <limits.h>
/**************************************************************************************************
 * Defining the Graph's methods
**************************************************************************************************/

// Constructor
Graph::Graph(int order, bool directed, bool weighted_edge, bool weighted_node)
{

    this->order = 0;                              // numbers of nodes
    this->number_edges = 0;                       // number of edges
    this->directed = directed;                    // if it's directed;
    this->weighted_edge = weighted_edge;          // if it has weight on its edges
    this->weighted_node = weighted_node;          // if it has weight on its nodes
    this->first_node = this->last_node = nullptr; // first and last node starts as null cause theres is nothing in the start
    this->negative_edge = false;
}

// Destructor
Graph::~Graph()
{

    Node *next_node = this->first_node;

    while (next_node != nullptr)
    {
        next_node->removeAllEdges();
        Node *aux_node = next_node->getNextNode();
        delete next_node;
        next_node = aux_node;
    }
}

// Getters
int Graph::getOrder() // return the Order of the graph!;
{
    return this->order;
}
int Graph::getNumberEdges() // return the number of edges in the graphic!;
{
    return this->number_edges;
}
//Function that verifies if the graph is directed!;
bool Graph::getDirected() // return if the graphic is directed!;
{
    return this->directed;
}
//Function that verifies if the graph is weighted at the edges!;
bool Graph::getWeightedEdge() // return if the graphic have weight at the edges!;
{
    return this->weighted_edge;
}

//Function that verifies if the graph is weighted at the nodes
bool Graph::getWeightedNode() // return if the graphic have weight at the nodes!;
{
    return this->weighted_node;
}

Node *Graph::getFirstNode() // return the first node of the graph!;
{
    return this->first_node;
}

Node *Graph::getLastNode() // return the last node of the graph!;
{
    return this->last_node;
}

///////////////////////////////////////////////////////////////////

void Graph::insertNode(int id, int weight)
{
    Node *node = new Node(id, order);
    node->setWeight(weight);
    if(!searchNode(id))
    {
         
        if (order == 0) // if there are no nodes in the graph
        {
            this->first_node = this->last_node = node; // both of them receive the new node;
        }
        else
        {                                 // if there are more than 0 nodes in the graph
            last_node->setNextNode(node); // set the next node to the new node;
            last_node = node;             // set the last node as the new node;
        }
        order++; // increase the order of the graph
    }

}

void Graph::insertEdge(int id, int target_id, float weight)
{
    if (searchNode(id) && searchNode(target_id)) // search if the two nodes are in the graph
    {
        if (weight < 0)
            this->negative_edge = true; // verifica se o peso da aresta � negativa

        if (this->directed)
        {
            Node *node = getNode(id);             // search the actual node that's being called;
            if (!node->hasEdgeBetween(target_id)) // return if theres is no edge between the node id and the node target_id
            {
                Node *target = getNode(target_id);
                node->insertEdge(target_id, weight, target->getIdNode()); // inserts the edge between the node we are to the node targeted 
                target->incrementInDegree();
                this->number_edges++;
            }
        }
        else
        {
            Node *node = getNode(id);             // search the actual node that's being called;
            if (!node->hasEdgeBetween(target_id)) // return if theres is no edge between the node id and the node target_id
            {
                Node *aux = getNode(target_id);
                node->insertEdge(target_id, weight, aux->getIdNode()); // inserts the edge between the two nodes
                aux->insertEdge(node->getId(), weight, node->getIdNode()); // inserts the edge between the two nodes;
                this->number_edges++; 
                node->incrementInDegree();
                aux->incrementInDegree();
            }
        }
    }
}

void Graph::removeNode(int id)
{
    if (this->first_node != NULL) // graph not empty
    {
        if (searchNode(id)) // node is in the graph
        {
            if (this->first_node == this->last_node) // theres only one node in the graph and the node we want do delete is in the graph
            {
                this->first_node = NULL;
                this->last_node = NULL;
                order--;
            }
            else // theres more than only one node and the node we want to delete is in the graph
            {
                Node *node = getNode(id);                                                                                 // new id receiving the target node;
                Node *previous = this->first_node;                                                                        // setting new node as first node;
                for (previous; (previous->getNextNode() != node || previous == node); previous = previous->getNextNode()) // just looking for the node before the targetedNode;
                {
                    cout << "";
                }
                if (node == previous) // if the node i want is equals previous so it is the first node;
                {
                    previous = previous->getNextNode();
                    first_node = previous;
                    delete previous;
                }
                else // the first node is not the node that we found
                {
                    previous->setNextNode(node->getNextNode());
                    if (last_node == node) // verifying if the node that we found is not the last node;
                    {
                        last_node = nullptr;
                    }
                    delete node;
                }
                previous = this->first_node; // passando o primeiro vertice para o previous
                while (previous != nullptr)  // enquanto previous n�o chegar no ultimo vertice
                {
                    previous->removeEdge(id, this->directed, previous); // vai chamando todos os vertices e verificando se eles tem aresta com o id(vertice que a gente quer excluir)
                    previous = previous->getNextNode();                 // passa o previous para o prox vertice
                }
                node->removeAllEdges(); // remove todas as arestas do vertice que a gente vai deletar, n�o sei se � necessario j� que a gente j� vai deletar ele mesmo, mas fica ai
                delete node;            // deleta o node que a gente quer
                delete previous;        // deleta o previous que usamos na fun��o
                order--;                // diminui a ordem do grafo ou seja o n�mero de v�rtices presente nele, j� que excluimos um v�rtice;
                if (order > 0)
                {
                    for (Node *aux = this->first_node; aux != NULL; aux = aux->getNextNode())
                    {
                        if (aux->getIdNode() == 0)
                        {
                        }
                        else
                        {
                            aux->setIdNode(aux->getIdNode()-1);
                        }
                    }
                }
            }
        }
    }
}

bool Graph::searchNode(int id)
{
    Node *node = this->first_node;                          // first node to the actual node
    for (node; node != nullptr; node = node->getNextNode()) // searching for the node if this id;
    {
        if (node->getId() == id) // found the node if this id
        {
            return true;
        }
    }
    return false; // didnt found the node if this id;
}

Node *Graph::getNode(int id)
{
    Node *node = this->first_node;                          // first node to the actual node
    for (node; node != nullptr; node = node->getNextNode()) // searching for the node
    {
        if (node->getId() == id) // if it finds it returns the node
        {
            return node;
        }
    }
    return nullptr; // else it returns nullptr;
}

Node *Graph::getNodeId(int id)
{
    Node *node = this->first_node;                          // first node to the actual node
    for (node; node != nullptr; node = node->getNextNode()) // searching for the node
    {
        if (node->getIdNode() == id) // if it finds it returns the node
        {
            return node;
        }
    }
    return nullptr; // else it returns nullptr;
}

// INICIO FECHO TRANSITIVO DIRETO ///////////////////////////////

void Graph::fechoTransitivoDireto(ofstream &output_file, int id)
{
    //cria um vetor que marca quais nodes ja foram analisados
    bool *visitados = new bool[this->order];
    //cria o vetor fecho transitivo direto
    bool *vet_ftd = new bool[this->order];
    //cria uma fila que diz quais vertices ainda precisam ser analisados
    queue<int> fila;
    //adiciona o vertice inicial na fila
    fila.push(id);
    for (int i = 0; i < this->order; i++)
    {
        visitados[i] = false;
        vet_ftd[i] = false;
    }

    //começa iteração (enquanto a fila não estiver vazia)
    while (!(fila.empty()))
    {
        //pega um node a ser analisado da fila
        int IdVet = getNode(fila.front())->getIdNode(); // já que os vetores começam da posição 0, isso equivale a passar a posição equivalente do id do vertice no vetor
        Node *V = getNode(fila.front());

        fila.pop();
        //verifica se o node a ser analisado ja foi analisado. (se ele ja foi acaba essa iteração)
        if (visitados[IdVet] == false)
        {
            //marca o node como visitado;
            visitados[IdVet] = true;
            //adiciona ele no vetor fecho transitivo direto
            vet_ftd[IdVet] = true;
            //adiciona todos os nodes adjacentes a ele na fila
            for (Edge *it = V->getFirstEdge(); it != NULL; it = it->getNextEdge())
            {
                int verticeAdj = it->getTargetId(); // aqui ele passa o id do node com o qual it(ou seja V) está ligado pela aresta e que tem como id o node alvo
                fila.push(verticeAdj);
            }
        }
    }
    
    //imprime o fecho transtivio direto do id
    output_file << "O fecho transitivo direto de " << id << " é: {"; 
    int cont = 0;
    for (int i = 0; i < this->order; i++)
    {
        if (vet_ftd[i] == true)
        {
            cont++;
        }
    }
    for (int i = 0; i < this->order; i++)
    {
        if (vet_ftd[i] == true)
        {
            if (cont - 1 > 0)
            {
                output_file << i + 1 << ", ";
                cont--;
            }
            else if (cont - 1 == 0)
            {
                output_file << i + 1;
            }
        }
    }
    output_file << "}" << endl;
}

// FIM FECHO TRANSITIVO DIRETO //////////////////////////////////


// INICIO FECHO TRANSITIVO INDIRETO

void Graph::fechoTransitivoIndireto(ofstream &output_file, int id)
{
    //cout << this->order;
    bool *fti = new bool[this->order];    // vetor para verificar o fecho transitivo indireto
    bool *node = new bool[this->order];   // vetor para verificar os vizinhos
    int *vetFti = new int[this->order];
    int *vetId = new int[this->order];
    int cont = 0;

    for (int i = 0; i < this->order; i++) // passando false para tudo antes de começaar
    {
        fti[i] = false;
        node[i] = false;
    }

    int conta = 0; // auxilia a descobrir qual o ultimo vertice para quando a gente for printar n�o colocar uma "," depois do ultimo

    for (Node *p = this->first_node; p != nullptr; p = p->getNextNode()) /// percorre todos os vertices
    {

        if (!node[p->getIdNode()]) // se a posição do vetor que equivale ao indice do vertice-1 já que a posição do vetor começa do 0, se ela for false o código ocorre, pois ainda não passamos por esse vertice
        {
            node[p->getIdNode()] = true;                            // passa true para a posição atual
            fti[p->getIdNode()] = deephFirstSearch(id, p->getId()); // chama a busca em profundidade passando o id que queremos e o id equivalente ao no que estamos no for
            if (fti[p->getIdNode()])                                  // se true, ou seja se é possivel desse node p chegar ao vertice "id" que é parametro da função então conta++ para auxiliar com a impressão, assim como está escrito ali em cima;
            {
                conta++;
                vetFti[cont] = p->getIdNode(); 
                vetId[cont] = p->getId();
                cont++;
            }
        }
    }
    output_file << "O fecho transitivo indireto de " << id << " é: ";
    output_file << "{ ";

    int aux = 0;
    for (int i = 0; i < cont; i++)
    {
        if (fti[vetFti[i]])
        {
            if (aux == conta - 1)
            {
                output_file << (getNode(vetId[i])->getId());
                aux++;
            }
            else
            {
                output_file << (getNode(vetId[i])->getId()) << ", ";
                aux++;
            }
        }
    }
    output_file << " }";
}

bool Graph::deephFirstSearch(int id, int start)
{
    
    //Criando vetor para verificar e tamb�m vetor predecessor de profundidade
    bool *node = new bool[this->order]; // vetor do tamanho do grafo
    int conta = 0;
    int idParametro; // equivale a posição do id do vertice no vetor;
    for (int i = 0; i < this->order; i++)
    {
        node[i] = false; // passa false para todas as posi��es
    }
    // cria vetor para auxiliar
    Node *p;

    //Para todo v em G;
    p = getNode(start);           // passa o primeiro v�rtice do grafo para o Node p;
    idParametro = p->getIdNode(); /// passa a posi��o equivalente do id de p em rela��o ao vetor
    //Se v n�o visitado ent�o

    if (id != p->getId()) // se o v�rtice que foi passado como parametro nessa fun��o que � chamada pela fecho transitivo indireta
    {                     // n�o for igual ao id, ele faz isso, caso contrario obviamente � pois j� estamos no v�rtice que queremos buscar
        auxDeepthFirstSearch(node, p); // passa o vetor e o node que estamos come�ando, pode ser 0,1,2 ... depende de onde o for
    }                                    // da fecho transitivo indireto est� chamando
    else
    {
        return true; // retorna true caso o v�rtice em que estamos � o vertice que queremos
    }

    //Se encontrou
    if (node[getNode(id)->getIdNode()]) // dps de passar pela aux ele verifica se foi mudado por parametro a posi��o equivalente
    {                                     // ao id que queremos no vetor, caso a gente queira o vertice 3 e passamos o vertice 8 que est� ligado
                                          // aos v�rtices 9 e 10, somente as posi��es 7,8,9 receberiam true, ou seja 8 n�o chega ao v�rtice 3, ou seja n�o entre nesse if
        delete[] node;
        return true;
    }
    delete[] node;
    return false;
}

void Graph::auxDeepthFirstSearch(bool node[], Node *v)
{

    int idParametro = v->getIdNode(); /// pega a posição equivalente do id desse node no vetor;

    Node *aux;

    //Marca v como visitado;
    node[idParametro] = true; /// marca o node que estamos como visitado;

    //Para todo node em Adj(v)
    for (Edge *p = v->getFirstEdge(); p != NULL; p = p->getNextEdge())
    {
        idParametro = p->getTargetIdNode(); /// pega a posição no vetor dos vizinhos que a gente quer verificar
        
        //Se o node !visitado então
        if (!node[idParametro])
        {
            
            aux = getNode(p->getTargetId());
            
            auxDeepthFirstSearch(node, aux);
        }
    }
    /*
        Explicando essa função acima: se por exemplo nós chamamos atraves da deepthFirstSearch o node 8
        que é ligado aos nodes 9 e 10, porém estamos buscando o node 3, ele retornara para a fecho
        transitivo indireto falso, pois na linha 355 nós passamos para todos os node falso, na chamada
        da função na qual o node 8 foi chamado, logo após nós chamamos essa função aq passando o vetor com
        tudo falso e o node 8, como ele está ligado ao 9 e 10, somente esses 3 irão receber true, oq acabara
        fazendo com que a posição 2 ou seja (3-1) que equivale a posição do vetor, continue false

    */
}

// FIM FECHO TRANSITIVO INDIRETO ////////////////////////////////

// INICIO DIJKSTRA /////////////////////////////////////////////

void Graph::caminhoMin_djkstra(ofstream &output_file, int orig, int dest) {

    if(!this->negative_edge)
    {
        if(this->weighted_edge)
        {
                if(this->directed)
                {
                    int *pa = new int[this->order];
                    int *dist = new int[this->order];
                    bool *mature = new bool[this->order];

                    for (int i=0; i<this->order; i++)
                        pa[i] = -1, mature[i] = false, dist[i] = INT_MAX;

                    int auxPa = getNode(orig)->getIdNode();
                    int auxDest = getNode(dest)->getIdNode();


                    pa[auxPa] = orig;
                    dist[auxPa] = 0;

                        while (true) 
                        {
                            int min = INT_MAX;
                            Node *y;

                            for (Node *z = this->first_node; z != nullptr; z = z->getNextNode()) 
                            {
                                if (mature[z->getIdNode()]) continue;
                                    if (dist[z->getIdNode()] < min) 
                                    {
                                        min = dist[z->getIdNode()];
                                        y = z;
                                    }                               
                            }

                            if (min == INT_MAX) break;
                            // atualização de dist[] e pa[]:
                            for (Edge *a = y->getFirstEdge(); a != nullptr; a = a->getNextEdge()) {
                                if (mature[a->getTargetIdNode()]) continue;
                                if (dist[y->getIdNode()] + a->getWeight() < dist[a->getTargetIdNode()]) {
                                    dist[a->getTargetIdNode()] = dist[y->getIdNode()] + a->getWeight();
                                    pa[a->getTargetIdNode()] = y->getId();
                                }
                            }
                            mature[y->getIdNode()] = true;
                        }

                        if(dist[auxDest] == INT_MAX )
                        output_file << " Nao existe caminho entre o vertice " << orig << " ao vertice " << dest << endl;
                        else  
                        output_file << " A distancia do vertice " << orig << " ao vertice " << dest << " sera: " << dist[auxDest] << endl;

                } else {
                    cout << "Chegou aqui";
                    int *pa = new int[this->order];
                    int *dist = new int[this->order];
                    bool *mature = new bool[this->order];

                    for (int i=0; i<this->order; i++)
                        pa[i] = -1, mature[i] = false, dist[i] = INT_MAX;

                    int auxPa = getNode(orig)->getIdNode();
                    int auxDest = getNode(dest)->getIdNode();


                    pa[auxPa] = orig;
                    dist[auxPa] = 0;

                        while (true) 
                        {
                            // escolha de y:
                            int min = INT_MAX;
                            Node *y;

                            for (Node *z = this->first_node; z != nullptr; z = z->getNextNode()) 
                            {
                                if (mature[z->getIdNode()]) continue;
                                    if (dist[z->getIdNode()] < min) 
                                    {
                                        min = dist[z->getIdNode()];
                                        y = z;
                                    }
                                    
                            }

                            if (min == INT_MAX) break;
                            // atualização de dist[] e pa[]:
                            for (Edge *a = y->getFirstEdge(); a != nullptr; a = a->getNextEdge()) {
                                if (mature[a->getTargetIdNode()]) continue;
                                if (dist[y->getIdNode()] + a->getWeight() < dist[a->getTargetIdNode()]) {
                                    dist[a->getTargetIdNode()] = dist[y->getIdNode()] + a->getWeight();
                                    pa[a->getTargetIdNode()] = y->getId();
                                }
                            }
                            mature[y->getIdNode()] = true;
                        }   
                        
                        int dist2 = -1;
                        dist2 = auxCaminhoMin_djkstra(dest,orig);
                        if(dist[auxDest] == INT_MAX )
                        {
                            output_file << " Nao existe caminho entre o vertice " << orig << " ao vertice " << dest << endl;
                        }
                        else if (dist2 == dist[auxDest] && dist2 != -1)
                        {  
                            if(dist2 == 0)
                            {
                                output_file << " A distancia do vertice " << orig << " ao vertice " << dest << " sera: " << dist[auxDest]<< endl;
                            }
                            else
                            {
                                output_file << " A distancia do vertice " << orig << " ao vertice " << dest << " sera: " << dist2 << endl;
                            }
                        } 
                        else if(dist2 == -1 )
                        {
                            output_file << " Nao existe caminho entre o vertice " << orig << " ao vertice " << dest << endl;
                        }                
                }
        }
        else
        {
            output_file << " A distancia do vertice " << orig << " ao vertice " << dest << " sera: " << 0 << endl;
        }
    } 
    else
    {
        output_file << " Não foi possível pois existem arestas com peso negativo" << endl;
    }

}


int Graph::auxCaminhoMin_djkstra(int orig, int dest)
{
    int *pa = new int[this->order];
    int *dist = new int[this->order];
    bool *mature = new bool[this->order];

        for (int i=0; i<this->order; i++)
            pa[i] = -1, mature[i] = false, dist[i] = INT_MAX;

        int auxPa = getNode(orig)->getIdNode();
        int auxDest = getNode(dest)->getIdNode();


        pa[auxPa] = orig;
        dist[auxPa] = 0;

            while (true) 
            {
                // escolha de y:
                int min = INT_MAX;
                Node *y;

                for (Node *z = this->first_node; z != nullptr; z = z->getNextNode()) 
                {
                    if (mature[z->getIdNode()]) continue;
                        if (dist[z->getIdNode()] < min) 
                        {
                            min = dist[z->getIdNode()];
                            y = z;
                        }
                        
                }

                if (min == INT_MAX) break;
                // atualização de dist[] e pa[]:
                for (Edge *a = y->getFirstEdge(); a != nullptr; a = a->getNextEdge()) {
                    if (mature[a->getTargetIdNode()]) continue;
                    if (dist[y->getIdNode()] + a->getWeight() < dist[a->getTargetIdNode()]) {
                        dist[a->getTargetIdNode()] = dist[y->getIdNode()] + a->getWeight();
                        pa[a->getTargetIdNode()] = y->getId();
                    }
                }
                mature[y->getIdNode()] = true;
            }

    if(!this->negative_edge)
    {
        if(dist[auxDest] != INT_MAX)
        {
          return dist[auxDest];
        } 
        else
        {
            return -1;
        }
    }
    else
    {
        if(dist[auxDest] != INT_MAX)
        {
          return dist[auxDest];
        } 
        else
        {
           exit(0);
        } 
    }
     
}

/////////// FIM DIJKSTRA /////////////////////////////////////////////


// INICIO FLOYD ////////////////////////////////////////////////////////////

void Graph::existeCaminho(bool *verifica,int idSource,int idTarget) {
    Node *node = getNode(idSource);
    for(Edge *aux = node->getFirstEdge(); aux != nullptr; aux = aux->getNextEdge()) {
        if(aux != nullptr) {
            if(aux->getTargetId() == idTarget) {
                *verifica = true;
            } else {
                existeCaminho(verifica, aux->getTargetId(), idTarget);
            }
        }
    }
}

void Graph::caminhoMin_floyd(ofstream &output_file, int idSource, int idTarget)
{
    if(this->directed)
    {
        Node *node = getNode(idSource);
        bool aux = false;
        bool *verifica;
        verifica = &aux;
        existeCaminho(verifica,idSource,idTarget);
        if(*verifica == true)
        {
            int ordem = this->order;               // recebe ordem do grafo
            int ** dist = new int *[ordem]; // inicializando matriz que recebe vetor
            dist = constroiMat_floyd(ordem, dist);             // dist recebe funcao floyd
            Node *node1 = getNode(idSource);
            Node *node2 = getNode(idTarget);
            output_file << "O menor caminho entre o No[" << idSource << "] e o No[" << idTarget << "] e: [" << dist[node1->getIdNode()][node2->getIdNode()] << "]" << endl;
        }
        else
        {
            output_file << " Nao existe caminho entre o No[" << idSource << "] e o No[" << idTarget << "]" << endl;
        }
    }
    else
    {
        output_file << " O grafo nao é direcionado " << endl;
    }
}

int **Graph::constroiMat_floyd(int ordem, int **dist)
{ // fucnao para utilizar lista de adj e para usar o alg de floyd

    dist = new int *[ordem];
    for (int i = 0; i < ordem; i++)
    {
        dist[i] = new int[this->order];
    }

    Node *auxA = this->first_node; // ponteiro do primeiro nó recebe primeiro no do grafo
    Node *auxB;                    // ponteiro auxiliar para um no
    int pesoEdge = 1;                  // peso da aresta
    // matriz com os valores de cada aresta entre os nos
    for (int i = 0; auxA != NULL; auxA = auxA->getNextNode(), i++)
    {
        auxB = this->first_node;

        for (int j = 0; auxB != NULL; auxB = auxB->getNextNode(), j++)
        {
            Edge *aux = auxA->hasEdgeBetween(auxB->getId());

            if (this->weighted_edge && aux != NULL)
                pesoEdge = aux->getWeight();

            if (auxA->getId() == auxB->getId())
                dist[i][j] = 0;

            else if (aux != NULL)

                dist[i][j] = pesoEdge;

            else
                dist[i][j] = INT_MAX / 2;
        }
    }
    for (int k = 0; k < ordem; k++)
    {
        // Escolhendo todos os vértices como fonte, um por um
        for (int i = 0; i < ordem; i++)
        {
            if (i != k)
            { // Escolhendo todos os vértices como destino
                for (int j = 0; j < ordem; j++)
                {
                    //Se o vértice c estiver no caminho mais curto de i para j, em seguida, atualize o valor de dist [i] [j]
                    if (dist[i][j] > dist[i][k] + dist[k][j] && dist[i][k] + dist[i][j] > 0)
                        dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }
    return dist;
}

// FIM FLOYD ////////////////////////////////////////////////////



Graph* Graph::getVertexInduced(int *listIdNodes, int tam)
{

    Graph *subGrafo = new Graph(this->order, this->directed, this->weighted_edge, this->weighted_node);

    Node *nodeAux;

    for (int i = 0; i < tam; i++)
    {
        if (this->searchNode(listIdNodes[i]))
        {
            subGrafo->insertNode(listIdNodes[i],0);
        }
    }

    Node *node;
    Node *inicio;
    Node *inicio2;
    Edge *aux;
    Edge *aux2;
    bool verifica = false;
    bool verifica2 = false;

    //para todo noh do subgrafo,
    

    for (node = subGrafo->getFirstNode(); node != nullptr; node = node->getNextNode())
    {

             inicio = this->getNode(node->getId()); 

           for (aux = inicio->getFirstEdge(); aux != nullptr; aux = aux->getNextEdge())
           {

            // se a aresta do vertice pra onde ela aponta existir

                verifica = subGrafo->searchNode(aux->getTargetId());
                verifica2 = inicio->searchEdge(aux->getTargetId());

                if (verifica && verifica2)
                {
                    
                    // incluir a aresta no noh do subgrafo;
                    if(node->searchEdge(aux->getTargetId()) == false)
                    {
                            node->insertEdge(aux->getTargetId(), aux->getWeight(), aux->getTargetIdNode()); 
                            getNode(aux->getTargetId())->insertEdge(node->getId(), aux->getWeight(), node->getIdNode()); 
                            subGrafo->number_edges++;
                    }
                } 

           }
        //verificar as arestas no grafo original. 
    }

    return subGrafo;
}

// INICIO PRIM ////////////////////////////////////////////////////////////

Graph *Graph::arvGMin_Prim(ofstream &output_file)
{

    if(this->directed && !weighted_edge ) {
        return nullptr;
    }  

    int num, vert;
 
    cout << "Digite o numero de vértices de 1 a " << this->order << " que serão adicionados no subgrafo vértice induzido" << endl;
    cin >> num;
    int *nodes = new int[num];

    for (int i = 0; i < num; i++)
    {
        cout << "Digite o vértice de numero " << i + 1 << ": " << endl;
        cin >> vert;
        nodes[i] = vert;
    }

    int *posicoes = new int[num];
    int *aux = new int[num];

    int cont = 0;

    Graph *grafoA;

    grafoA = this->getVertexInduced(nodes, num);
    // criando subGrafoVeti

   int *pa = new int[this->order];
   bool *tree = new bool[this->order];
   int *preco = new int[this->order];

   // inicialização:
   for (int i = 0; i < grafoA->order; i++)
   {
       pa[i] = -1;
       tree[i] = false;
       preco[i] = INT_MAX; 
   }   

    Node *auxN = grafoA->getFirstNode();
    tree[auxN->getIdNode()] = true;
    posicoes[auxN->getIdNode()] = auxN->getId();
    aux[cont] = auxN->getIdNode();
    pa[cont] = auxN->getId();

   for (Edge *auxE = auxN->getFirstEdge(); auxE != nullptr; auxE=auxE->getNextEdge())
   {
       cont++;
       pa[auxE->getTargetIdNode()] = auxN->getId();
       posicoes[auxE->getTargetIdNode()] = auxE->getTargetId();
       aux[cont] = auxE->getTargetIdNode(); 
       preco[auxE->getTargetIdNode()] = auxE->getWeight();
   }
 

   while (true)
    { 
      
      int min = INT_MAX;
      Node *y; 

       for (Node *w = grafoA->getFirstNode(); w != nullptr ; w = w->getNextNode())
       {

         if (!tree[w->getIdNode()] && preco[w->getIdNode()] < min)
         {
              min = preco[w->getIdNode()];
               y = w;
         } 
       }

      if (min == INT_MAX) break;
      // a aresta pa[y]-y é a mais barata da franja
      tree[y->getIdNode()] = true;
      // atualização dos preços e ganchos: 
      for (Edge *a = y->getFirstEdge(); a != nullptr; a = a->getNextEdge())
       {

         if (!tree[a->getTargetIdNode()] && a->getWeight() < preco[a->getTargetIdNode()]) 
         {
            cont++;
            preco[a->getTargetIdNode()] = a->getWeight();
            pa[a->getTargetIdNode()] = y->getId();
            posicoes[a->getTargetIdNode()] = a->getTargetId();
            aux[cont] = a->getTargetIdNode();
         }
      }
   }

    // montando subArv e printando

    Graph *arvPrim = new Graph(num, this->directed, this->weighted_edge, this->weighted_node);

     int aux2;
     for(int i=0;i<=cont;i++)
     {
        aux2 = aux[i];
        arvPrim->insertNode(posicoes[aux2],0); 
     }

     int auxP;
     int total = 0;

      for(int i=0;i<=cont;i++)
     {

         Node *e = getNode(pa[aux[i]]);
         
            for(Edge *x = e->getFirstEdge(); x != nullptr; x = x->getNextEdge())
            {
                if(x->getTargetId() == posicoes[aux[i]])
                {
                   auxP = x->getWeight();
                   total = total + auxP;
                }
            }

         if(pa[aux[i]] != posicoes[aux[i]])
         {
           arvPrim->insertEdge(pa[aux[i]],posicoes[aux[i]],auxP);
         }

     }

     total = total/2;

    output_file << " O peso total da Arvore Geradora Minina pelo algoritmo de Prim sera: " << endl;
    output_file << total << endl;
    output_file << "Arvore Geradora Minina pelo algoritmo de Prim: " << endl;

    return arvPrim;

}

// FIM PRIM ////////////////////////////////////////////////////

// INICIO KRUSKAL ////////////////////////////////////////////////////////////

Graph *Graph::arvGMin_Kruskal(ofstream &output_file)
{
    if(this->directed && !weighted_edge) {
        return nullptr;
    }

       
    int num, v;
    cout << "Digite o numero de vértices de '1' a " << this->order << " que serão adicionados no subgrafo vértice induzido" << endl;
    cin >> num;
    int *nodes = new int[num];
    for (int i = 0; i < num; i++)
    {
        nodes[i] = -1;
    }
    for (int i = 0; i < num; i++)
    {
        cout << "Digite o vértice numero " << i + 1 << ": " << endl;
        cin >> v;
        nodes[i] = v;
    }
    //pre-requisitos pra fazer a ordenacao

    Graph *grafoA;
    grafoA = this->getVertexInduced(nodes, num);

    Graph *grafoB = new Graph(this->order, this->directed, this->weighted_edge, this->weighted_node); //vai vira o grafoVI

    int *EdgeNode = new int[3];
    int totalEdge = 0;

     totalEdge = grafoA->getNumberEdges();

    list<pair<int, int>> listP;

    Node *sup;
    Node *p;
    Edge *aux;

    for (sup = grafoA->getFirstNode(); sup != nullptr; sup = sup->getNextNode())
    {
        grafoB->insertNode(sup->getId(),sup->getWeight());
    }
    
        
    //Criar uma lista L com as arestas ordenadas em
    //ordem crescente de pesos.
    for (int i = 0; i < totalEdge; i++)
    {
        // acha a aresta de menor peso
        grafoA->getWeithlessEdge(EdgeNode);
        //insere a aresta de menor peso
        listP.push_back(make_pair(EdgeNode[0], EdgeNode[1]));
        //retira a aresta do grafo pra evitar repetir a mesma aresta;
        sup = grafoA->getNode(EdgeNode[0]);
        p = grafoA->getNode(EdgeNode[1]);
        if (!this->directed)
        {
            sup->removeEdge(p->getId(), this->directed, p);
            p->removeEdge(sup->getId(), this->directed, sup);
            sup->removeEdge(p->getId(), this->directed, p);
            p->removeEdge(sup->getId(), this->directed, sup);
        }
        else
        {
            sup->removeEdge(p->getId(), this->directed, p);
        }
        //adiciona a a resta num grafo auxiliar.
        grafoB->insertEdge(EdgeNode[0], EdgeNode[1], EdgeNode[2]);
    }
   
      
    //Organizar a lista;

    //Criar |V| subárvores contendo cada uma um nó
    //isolado.
    Graph *agMin = new Graph(this->order, this->directed, this->weighted_edge, this->weighted_node);
    for (sup = grafoB->getFirstNode(); sup != nullptr; sup = sup->getNextNode())
    {
        agMin->insertNode(sup->getId(), sup->getWeight());
    }

    //Cria lista vazia
    list<pair<int, int>> listaAux;

    //contador ¬ 0
    int cont = 0;
    int numMaxAresta = agMin->getOrder() - 1;
    bool *verificado = new bool[this->order];

    for (int i = 0; i < this->order; i++)
    {
        verificado[i] = false;
    }
      
    while (cont < numMaxAresta && !listP.empty())
    {
        pair<int, int> dist_no = listP.front(); //copia par (id do vertice e distancia) do topo
        int v1 = dist_no.first;
        int v2 = dist_no.second;

        listP.pop_front();
        //Se v1 e v2 não estão na mesma subárvore então
        if (!verificaSubarvore(v1, v2, agMin))
        {
            //preenche a lista;
            listaAux.push_back(make_pair(v1, v2));
            //busca o peso da aresta
            int peso = getWeightFromEdgeNodeCombo(v1, v2, grafoB);

            //Unir as subárvores que contêm v1 e v2.
            agMin->insertEdge(v1, v2, peso);
            //contador ¬ contador + 1
            cont++;
        }
    }
   
    
    int pesoT = 0;
    while (!listaAux.empty())
    {
        pair<int, int> dist_no = listaAux.front(); //copia par (id do vertice e distancia) do topo
        int v1 = dist_no.first;
        int v2 = dist_no.second;
        pesoT = pesoT + getWeightFromEdgeNodeCombo(v1, v2, agMin);
        listaAux.pop_front();
    }
    output_file << "Peso da Arvore Geradora Minima: " << pesoT << endl;


    return agMin;

}

// FIM KRUSKAL ////////////////////////////////////////////////////


// INICIO BUSCA EM LARGURA ////////////////////////////////////////////////////////////

void Graph::arv_Buscalargura(ofstream &output_file, int id)
{

  int *num = new int[this->order];
  int *pa = new int[this->order];
  int *vetAd = new int[this->order];
  int *posicoes= new int[this->order];
  bool entrou = false;
  int cont = 0;
  int cont2 = 0;


    for (int i = 0; i < this->order; i++)
        num[i] = pa[i] = -1;

        
    list<Node*> listN;  
    Node *node1 = getNode(id);
    num[node1->getIdNode()] = cont++; 
    pa[node1->getIdNode()] = id;
    listN.push_back(getNode(id));
    vetAd[cont2] = id;
    posicoes[0] = id;

    while (!listN.empty()) 
    {
        entrou = false;
        Node *aux = listN.front();

        listN.pop_front(); 

        for (Edge *auxE = aux->getFirstEdge(); auxE!=NULL; auxE=auxE->getNextEdge())
        {
             if (num[auxE->getTargetIdNode()] == -1) {
                 posicoes[cont] = aux->getId();
                num[auxE->getTargetIdNode()] = cont; 
                cont++;
                pa[auxE->getTargetIdNode()] = aux->getId();
                listN.push_back(getNode(auxE->getTargetId()));
                cont2++;
                vetAd[cont2] = auxE->getTargetId();
            }

        }
     
    }

      Graph *arvBL = new Graph(this->order, this->directed, this->weighted_edge, this->weighted_node);

     for(int i=0;i<=cont2;i++)
     {
        arvBL->insertNode(vetAd[i],0); 
     }

    int auxP;
    int auxId;

     for(int i=1;i<=cont2;i++)
     {
            Node *e = getNode(posicoes[i]);
            for(Edge *x = e->getFirstEdge(); x != nullptr; x = x->getNextEdge())
            {
                if(x->getTargetId() == vetAd[i])
                {
                   auxP = x->getWeight();
                }
            }
            arvBL->insertEdge(posicoes[i],vetAd[i],auxP);

     }

    output_file << "Arvore dada pelo caminhamento em lagura: ";

    arvBL->printGraph(output_file);
 
}


// FIM BUSCA EM LARGURA ////////////////////////////////////////////////////

void Graph::getWeithlessEdge(int *nohAresta)
{

    Node *p = this->first_node;
    Edge *aux = p->getFirstEdge();
    int menor = 9999999;
    while (p != NULL)
    {

        aux = p->getFirstEdge();
        while (aux != NULL)
        {
            if (aux->getWeight() < menor)
            {
                nohAresta[0] = p->getId();
                nohAresta[1] = aux->getTargetId();
                nohAresta[2] = aux->getWeight();
                menor = aux->getWeight();
            }
            aux = aux->getNextEdge();
        }
        p = p->getNextNode();
    }
}

// usa os mecanismos da busca em profundidade para indicar a alcansabilidade de um vertice a outro
bool Graph::verificaSubarvore(int v1, int v2, Graph *subGrafo)
{
    //vetor de alcansabilidade -- se tem caminho ou nao
    bool *fti = new bool[this->order];

    for (int i = 0; i < this->order; i++)
    {
        fti[i] = false;
    }
    // verifica para todos os nohs se tem caminho ou nao
    for (Node *p = subGrafo->getFirstNode(); p != NULL; p = p->getNextNode())
    {

        fti[p->getId() - 1] = subGrafo->deephFirstSearch(v2, p->getId());
    }

    return fti[v1 - 1];
}

//pega o peso da aresta atravez do int idNoh, int idAresta, Graph *subGrafo
int Graph::getWeightFromEdgeNodeCombo(int idNoh, int idAresta, Graph *subGrafo)
{
    Node *p = subGrafo->getNode(idNoh);
    Edge *aux;
    for (aux = p->getFirstEdge(); aux != NULL; aux = aux->getNextEdge())
    {
        if (aux->getTargetId() == idAresta)
        {
            break;
        }
    }
    return aux->getWeight();
}


void Graph::printGraph(ofstream &output_file)
{
    Node *p = this->first_node;
    Edge *aux = p->getFirstEdge();
    if (!directed)
    {
        output_file << "strict graph{"<<endl;
        while (p != NULL)
        {

            aux = p->getFirstEdge();
            while (aux != NULL)
            {

                output_file << p->getId() << " -- " << aux->getTargetId() <<" PESO : " << aux->getWeight() << endl;
                aux = aux->getNextEdge();
            }
            p = p->getNextNode();
        }
        output_file <<"}"<<endl;
    }
    else
    {
        output_file << "digraph{"<<endl;
        while (p != NULL)
        {

            aux = p->getFirstEdge();
            while (aux != NULL)
            {

                output_file << p->getId() << " -> " << aux->getTargetId() <<" PESO : " << aux->getWeight() << endl;
                aux = aux->getNextEdge();
            }
            p = p->getNextNode();
        }
        output_file <<"}"<<endl;
    }
        output_file << endl;
    output_file << endl;
}

/////////// INICIO DA ORDENAÇÃO TOPOLOGICA///////////////////////////

void Graph::ord_Topologica(ofstream &output_file)
{
    list<Node*> listN; // lista de nodes
    list<int> listTop; // lista topologica
    if (this->graphTemCiclo())// verifica se o grafo é aciclico ou não
    {
        output_file <<" Se o Grafo possui ciclos, logo, nao possui ordenação topologica"<<endl;
    }
    else{ // adaptando algoritimo kahn's
            Node *auxN;
            Edge *auxE;
            //procurando nos com enttrada =0
            for (auxN=this->first_node;auxN!=NULL;auxN = auxN->getNextNode())
            {   if (auxN->getInDegree()==0)// se entrada  = 0
                {
                    listN.push_back(auxN); //coloca os nos corretos na fila
                }
            }
            while (!listN.empty())// enquanto lista e vazia
            {
                Node *aux = listN.front();
                listN.pop_front(); //remove da lista
                listTop.push_back(aux->getId()); //coloca na lista auxiliar
                for(auxE =aux->getFirstEdge(); auxE!=NULL;auxE=auxE->getNextEdge())
                {
                    auxN = this->getNode(auxE->getTargetId()); //pega o no vizinho
                    auxN->decrementInDegree(); //decrementa o grau de entrada
                    if (auxN->getInDegree()==0) //se a entrada = 0
                    {
                        listN.push_back(auxN);
                    }

                }
            }
            //imprimindo ordenaçao a classificação topologica
            output_file << "Ordenação Topologica :" << endl;
            for(list<int>::iterator k = listTop.begin(); k != listTop.end(); k++)
            {
                    if(listTop.size() == this->getOrder())
                    output_file << (*k) << endl;
            }

        }
}

bool Graph::graphTemCiclo()
{
    list<int> auxCiclo;
    // Alocando os ints em uma lista
    for (int i = 0; i < this->order; i++)
    {
        auxCiclo.push_back(i);
    }
    auxCiclo.sort();

    for (list<int>::iterator i = auxCiclo.begin(); i !=  auxCiclo.end();){
     int anterior = *i;
        i++;
        // Se houver componentes iguais, o gráfo é cíclico,
         // entao o grafo tem um circuito
        if (anterior == *i)
            return true;
    }
        // Se  forem diferentes entre eles, o grafo nao tem circuito
    return false;
}

/// FIM DA ORDENAÇÃO TOPOLOGICA ///////////////////////////

void Graph::Guloso(ofstream &output_file, int p)
{
	if(this->weighted_node) 
    {
        vector<list<Node> > listVector; //Note space between "> >"
        for(int i=0;i<p;i++) {
            listVector.push_back(criaLista());
        }

        srand(time(0));

        for(int i=0;i<p;i++) 
        {
            //list<Node>::iterator nodeIterator;
            int x = 1 + (rand() % this->order); // escolhendo número aleatorio
            Node *nodeAux = this->getNodeId(x); // pegando node referente a esse número
            if(nodeAux->getInDegree() == 1) {
                listVector[i].emplace_back(*nodeAux); // caso o node só tenha uma aresta a gente vai inserir o único vizinho direto na lista que o vizinho tá
                listVector[i].emplace_back(*nodeAux->getFirstEdge()); // único vizinho direto já pode pegar direto no getFirstEdge()
            } else {
                listVector[i].emplace_back(*nodeAux); // inserindo esse node na lista da posição i do vector
            }
        }  
        
        list<int> nodeWeight;
        list<int> nodeEdge;
        for(Node *node = this->first_node;node != nullptr;node = node->getNextNode())
        {
            if(node->getId() != listVector[0].begin()->getId() && node->getId() != listVector[1].begin()->getId() )
            {
                nodeWeight.push_back(node->getWeight());
                nodeEdge.push_back(node->getTotal_Edge());
            }
        }

        while(nodeWeight.size() != 0 ) //verificando se a lista esta vazia 
        {
            
        }
 






    } else {
        output_file << "O grafo não tem peso nas arestas" << endl;
    }
    
    
}

list<Node> Graph::criaLista() {
    list<Node> List;
    return List;
}



/// SEGUNDA ETAPA DO TRAB /////////////////////////////////




/// FIM DA SEGUNDA ETAPA DO TRAB //////////////////////////
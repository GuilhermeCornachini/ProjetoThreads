#include <iostream>
#include <time.h>
#include <thread>
#include <process.h>
#include <vector>
#include <mutex>
#include <pthread.h>
using namespace std;
typedef struct tag
{
    int inicio=0;
    int fim=0;
};
unsigned int *matriz;
vector<thread> threads(0);
vector<tag> Tags(0);
int n_primos=0;
mutex mtx,blocos;
void comandos();
void ChamaThreads();
int VerificaPrimos(int num);

int main ()
{
    int tammatrizX,tammatrizY;
    int N_threads;
    int comando=11;
    int seed;
    clock_t t;
    int tam_macrobloco, total_blocos;
    sched_param sch;
    int policy; 
    comandos();
    while(comando != 0)
    {
        cin>>comando;
        if(comando == 1)//tamanho da matriz
        {
            cout<<"Digite o tamanho x e y da matriz:";
            cin>>tammatrizX>>tammatrizY;
            matriz = (unsigned int *)malloc(sizeof(unsigned int)*tammatrizX*tammatrizY);
            //lembrar de numeros com % 0
        }
        if(comando == 2)//define seed
        {
            cout<<"Digite a seed:";
            cin>>seed;
        }
        if(comando == 3)//preencher
        {
            cout<<"Preenchendo"<<endl;
            srand(seed);
            for (int i = 0; i<tammatrizX*tammatrizY; i++)
                 matriz[i]=rand() % 100000000;
            cout<<"Preenchido!"<<endl;
        }
        if(comando == 4)//macroblocos
        {
            cout<<"Digite o tamanho dos macroblocos";
            cin>>tam_macrobloco;
            while( tammatrizY*tammatrizX % tam_macrobloco !=0 )
            {
                cout<<"Com tamanho do macrobloco digitado nao e possivel criar, digite outro numero"<<endl;
                cin>>tam_macrobloco;
            }
            tag aux;
            total_blocos=tammatrizX*tammatrizY/tam_macrobloco;
            for(int i=0; i<total_blocos;i++)
            {
                aux.inicio=tam_macrobloco*i;
                aux.fim=tam_macrobloco*(i+1);
                Tags.push_back(aux);
            }

        }
        if(comando == 5)//numero de threads
        {
            cout<<"Digite o numero de threads:";
            cin>>N_threads;
            for (int i = 0; i < N_threads; i++)
            { 
                threads.push_back(thread(ChamaThreads));
            }  
        }
        if(comando == 6)//executar
        {
            cout<<"Executando..."<<endl;
            t=clock(); 
            for (int i = 0; i < N_threads; i++)
            {
                threads[i].join();
            }
            for (int i = 0; i < N_threads; i++)
            {
                threads.pop_back();
            }
            t = clock() - t;
            cout<<"Executado"<<endl;
        }
        if(comando == 7)//definir prioridade
        {
            for(int i=0;i<N_threads;i++)
            {
                cout<<"Digite a prioridade da thread "<<i<<endl;
                cin>>sch.sched_priority;
                pthread_setschedparam(threads[i].native_handle(), SCHED_FIFO, &sch);
            }
            
        }
        if(comando == 8)//visualizar threads
        {
            for(int i=0; i<N_threads;i++)
            {
                pthread_getschedparam(pthread_self(), &policy, &sch);
                cout<<"Thread "<<i <<" prioridade:"<<sch.sched_priority<<endl;
            }
        }
        if(comando == 9)//visualizar inf
        {
            cout<<n_primos<<" Numeros primos"<<endl<<((double)t)/(CLOCKS_PER_SEC)<<" Segundos"
            <<endl<<"Utilizando "<<N_threads<<" Threads";
            cout<<endl<<"Em uma matriz "<<tammatrizX<<" x "<<tammatrizY<<endl;
            cout<<"Utilizando macroblocos de tamanho "<<tam_macrobloco<<" sendo divididos em um total de "
            <<total_blocos<<endl;
            n_primos=0;
        }
        if(comando == 10)//sair
            return 0;
    }
    return 0;
    
}

void ChamaThreads()
{
    int primo=0;
    while(1)
    {
        blocos.lock();
        int inicio=Tags.back().inicio;
        int fim=Tags.back().fim;
        Tags.pop_back();
        blocos.unlock();
        for(int i = inicio ; i < fim ; i++)
        {
            primo += VerificaPrimos(matriz[i]);
        }
    if(Tags.size() == 0)
        break;
    }
    mtx.lock();
    n_primos+=primo;
    mtx.unlock();
    _endthread();
}
int VerificaPrimos(int num)
{
    int div=0;
    int cont=0;
    for(int aux=2 ; aux<=num/2 ; aux++)
    {
        if(num%aux==0)
        {
            div++;
        }
    }
        if(div==0)
        {
            return 1;
        }else{
            return 0;
        }
        
}
void comandos()
{
    cout<<"Digite 1 para definir o tamanho da matriz."<<endl;
    cout<<"Digite 2 para definir semente para o gerador de números aleatórios."<<endl;
    cout<<"Digite 3 para preencher a matriz com números aleatórios."<<endl;
    cout<<"Digite 4 para definir o tamanho dos macroblocos."<<endl;
    cout<<"Digite 5 para definir o número de Threads."<<endl;
    cout<<"Digite 6 para executar."<<endl;
    cout<<"Digite 7 para mudar prioridade de uma ou mais Threads."<<endl;
    cout<<"Digite 8 para visualizar a prioridade das Threads."<<endl;
    cout<<"Digite 9 para visualizar o tempo de execução e quantidade de números primos."<<endl;
    cout<<"Digite 10 para encerrar."<<endl;
}


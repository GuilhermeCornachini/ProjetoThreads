#include <iostream>
#include <time.h>
#include <process.h>
#include <vector>
#include <windows.h>
#include <math.h>
using namespace std;
typedef struct tag
{
    int inicio=0;
    int fim=0;
};
unsigned int *matriz;
vector<tag> Tags(0);
int n_primos=0;
HANDLE mtx= CreateMutex(NULL, FALSE, NULL);
HANDLE blocos= CreateMutex(NULL, FALSE, NULL);
vector <HANDLE>threads(0);
HANDLE thread;
void prioridades();
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
    int policy;
    int prioridade;
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
                thread=CreateThread(NULL, 0,(LPTHREAD_START_ROUTINE)ChamaThreads,NULL,CREATE_SUSPENDED,NULL);
                threads.push_back(thread);
            }
        }
        if(comando == 6)//executar
        {
            cout<<"Executando..."<<endl;
            t=clock(); 
            for (int i = 0; i < N_threads; i++)
            { 
                ResumeThread(threads[i]);
            }  
            for (int i = 0; i < N_threads; i++)
            {
                WaitForSingleObject(threads[i],INFINITE);
               // threads.pop_back();
            }
            t = clock() - t;
            cout<<"Executado"<<endl;
        }
        if(comando == 7)//definir prioridade
        {
            prioridades();
            cin>>prioridade;
            for(int i=0;i<N_threads;i++)
            {
                switch (prioridade)
                {
                case 1:
                    for (int i=0;i<N_threads;i++)
                        SetThreadPriority(threads[i],THREAD_PRIORITY_IDLE);
                    break;
                case 2:
                    for (int i=0;i<N_threads;i++)
                        SetThreadPriority(threads[i],THREAD_PRIORITY_LOWEST);
                    break;
                case 3:
                    for (int i=0;i<N_threads;i++)
                        SetThreadPriority(threads[i],THREAD_PRIORITY_BELOW_NORMAL);
                    break;
                case 4:
                    for (int i=0;i<N_threads;i++)
                        SetThreadPriority(threads[i],THREAD_PRIORITY_NORMAL);
                    break;
                case 5:
                    for (int i=0;i<N_threads;i++)
                        SetThreadPriority(threads[i],THREAD_PRIORITY_ABOVE_NORMAL);
                    break;
                case 6:
                    for (int i=0;i<N_threads;i++)
                        SetThreadPriority(threads[i],THREAD_PRIORITY_HIGHEST);
                    break;
                case 7:
                    for (int i=0;i<N_threads;i++)
                        SetThreadPriority(threads[i],THREAD_PRIORITY_TIME_CRITICAL);
                    break;
                default:
                    for (int i=0;i<N_threads;i++)
                        SetThreadPriority(threads[i],THREAD_PRIORITY_NORMAL);
                    break;
                }
            }
            
        }
        if(comando == 8)//visualizar threads
        {
                prioridade = GetThreadPriority(threads[0]);
                cout<<"Todas as threads possuem prioridade ";
                switch (prioridade)
                {
                case 0:
                    cout<<"normal";
                    break;
                case 1:
                    cout<<"acima do normal";
                    break;
                case -1:
                    cout<<"abaixo do normal";
                    break;
                case 2:
                    cout<<"altissima";
                    break;
                case -2:
                    cout<<"abaixo do normal";
                    break;
                case 15:
                    cout<<"tempo critico";
                break;
                case -15:
                    cout<<"ociosa";
                    break;
                
            }
            cout<<endl;
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
        WaitForSingleObject(blocos,INFINITE);
        int inicio=Tags.back().inicio;
        int fim=Tags.back().fim;
        Tags.pop_back();
        ReleaseMutex(blocos);
        for(int i = inicio ; i < fim ; i++)
        {
             primo += VerificaPrimos(matriz[i]);
        }
    if(Tags.size() == 0)
        break;
    }
    WaitForSingleObject(mtx,INFINITE); 
    n_primos+=primo;
    ReleaseMutex(mtx);
}
int VerificaPrimos(int numero)
  {
  for ( int x = 2; x<=sqrt(numero); x++)
    //sqrt(numero)
    //Voce pode colocar para ir até numero/2 mas em meus testes levou 150 VEZES MAIS
     if( numero%x == 0 ) 
        return 0;

   return 1;
}
void comandos()
{
    cout<<"Digite 1 para definir o tamanho da matriz."<<endl;
    cout<<"Digite 2 para definir semente para o gerador de numeros aleatorios."<<endl;
    cout<<"Digite 3 para preencher a matriz com numeros aleatorios."<<endl;
    cout<<"Digite 4 para definir o tamanho dos macroblocos."<<endl;
    cout<<"Digite 5 para definir o numero de threads."<<endl;
    cout<<"Digite 6 para executar."<<endl;
    cout<<"Digite 7 para mudar prioridade de uma ou mais threads."<<endl;
    cout<<"Digite 8 para visualizar a prioridade das threads."<<endl;
    cout<<"Digite 9 para visualizar o tempo de execucao e quantidade de numeros primos."<<endl;
    cout<<"Digite 10 para encerrar."<<endl;
}
void prioridades()
{
    cout<<"Digite 1 para a prioridade ociosa"<<endl;
    cout<<"Digite 2 para a prioridade baixa"<<endl;
    cout<<"Digite 3 para a prioridade abaixo do normal"<<endl;
    cout<<"Digite 4 para a prioridade normal"<<endl;
    cout<<"Digite 5 para a prioridade acima do normal"<<endl;
    cout<<"Digite 6 para a prioridade altissima"<<endl;
    cout<<"Digite 7 para a prioridade tempo critico"<<endl;
    cout<<"Para qualquer valor diferente de 1-7 sera definida a prioridade normal"<<endl;
}

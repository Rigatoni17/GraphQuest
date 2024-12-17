#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>
/*Structure for the linked list*/
struct Node{
    char data[81];
    long edgeWeight;
    struct Node* next;
};
/*Function that inserts into the linked list*/
void insertList(struct Node* graph[], long i, long totalVertices, char vertex1[], char vertex2[], long edgeWeight){
    struct Node* temp = malloc(sizeof(struct Node));
    strcpy(temp->data, vertex2);
    temp->edgeWeight=edgeWeight;
    temp->next=NULL;
    struct Node* traverse = graph[i];
    if(traverse->next==NULL){
        graph[i]->next=temp;
    }
    else{
        while((traverse->next!=NULL)&&(strcmp(traverse->next->data, temp->data)<0)){
            traverse=traverse->next;
        }
        temp->next=traverse->next;
        traverse->next=temp;
    }
}
/*Function to add edge to graph*/
struct Node** addEdge(struct Node* graph[], long totalVertices, char vertex1[], char vertex2[], long edgeWeight){
    /*vertex1 -> vertex2*/
    for(long i=0; i<totalVertices; i++){
        if(strcmp(graph[i]->data, vertex1)==0){
            insertList(graph, i, totalVertices, vertex1, vertex2, edgeWeight);
        }
    }
    return graph;
}
/*Function that finds the index in the graph of the given vertex*/
long indexSearch(struct Node* graph[], long totalVertices, char vertex[]){
    long index=-1;
    for(long i=0; i<totalVertices; i++){
        if(strcmp(graph[i]->data, vertex)==0){
            index=i;
        }
    }
    return index;
}
/*Function that finds & prints the out degree of the vertex*/
long degreeOut(struct Node* graph[], long totalVertices, char vertex[]){
    long degree=0;
    for(int i=0; i<totalVertices; i++){
        if(strcmp(graph[i]->data, vertex)==0){
            struct Node* traverse = graph[i];
            while(traverse->next!=NULL){
                traverse=traverse->next;
                degree++;
            }
        }
    }
    return degree;
}
/*Recursive Function that finds the dfs traversal of the given vertex*/
void graphDFS(struct Node* graph[], long totalVertices, char vertex[], long visitArr[], long stack[], long *top){
    long index=indexSearch(graph, totalVertices, vertex);
    visitArr[index]=1;
    struct Node* traverse = graph[index];
    while(traverse!=NULL){
        long traverseIndex=indexSearch(graph, totalVertices, traverse->data);
        if(visitArr[traverseIndex]!=1){
            graphDFS(graph, totalVertices, traverse->data, visitArr, stack, top);
        }
        traverse=traverse->next;
    }
    stack[++*top]=index;
}
/*Function to top sort the graph*/
void topSort(struct Node* graph[], long totalVertices, long stack[], long *top){
    long visitArr[totalVertices];
    for(long i=0; i<totalVertices; i++){
        visitArr[i]=0;
    }
    long *pVisit=visitArr;
    char vertex[81];
    strcpy(vertex, graph[0]->data);
    graphDFS(graph, totalVertices, vertex, pVisit, stack, top);
    long count=1;
    while(count<=totalVertices){
        count=1;
        for(long index=0; index<totalVertices; index++){
            strcpy(vertex, graph[index]->data);
            for(long i=0; i<totalVertices; i++){
                if(strcmp(vertex, graph[i]->data)>0){
                    if(visitArr[i]!=1){
                        strcpy(vertex, graph[i]->data);
                    }
                }
            }
            if(visitArr[index]!=1){
                graphDFS(graph, totalVertices, vertex, pVisit, stack, top);
            }
        }
        for(long i=0; i<totalVertices; i++){
            if(visitArr[i]==1){
                count++;
            }
        }
    }
}
/*Function to implement algorithm1*/
void algorithm1(struct Node* graph[], long totalVertices, char vertex[]){
    long stack[totalVertices];
    long *pStack=stack;
    long top=-1;
    topSort(graph, totalVertices, pStack, &top);
    long distanceArr[totalVertices];
    for(long i=0; i<totalVertices; i++){
        distanceArr[i]=LONG_MAX;
    }
    long indexVertex=indexSearch(graph, totalVertices, vertex);
    distanceArr[indexVertex]=0;
    while(top!=-1){
        long indexU=stack[top--];
        struct Node* traverse=graph[indexU];
        long degree = degreeOut(graph, totalVertices, traverse->data);
        for(long i=0; i<degree+1; i++){
            long indexV = indexSearch(graph, totalVertices, traverse->data);
            if(distanceArr[indexU]!=LONG_MAX){
                if(distanceArr[indexV]>distanceArr[indexU]+traverse->edgeWeight){
                    distanceArr[indexV]=distanceArr[indexU]+traverse->edgeWeight;
                }
                traverse=traverse->next;
            }
        }
    }
    /*print distance array*/
    for(long i=0; i<totalVertices; i++){
        if(distanceArr[i]==LONG_MAX){
            printf("%s INF\n", graph[i]->data);
        }
        else{
            printf("%s %ld\n", graph[i]->data, distanceArr[i]);
        }
    }
}
/*Function that finds the topological order and checks if there are cycles*/
int detectCycle(struct Node* graph[], long totalVertices){
    int check=0;
    long stack[totalVertices];
    long *pStack=stack;
    long top=-1;
    topSort(graph, totalVertices, pStack, &top);
    while(top!=-1){
        long indexU=stack[top--];
        struct Node* traverse=graph[indexU];
        long degree = degreeOut(graph, totalVertices, traverse->data);
        for(long i=0; i<degree+1; i++){
            long indexV = indexSearch(graph, totalVertices, traverse->data);
            if(indexU>indexV){
                check=1;
            }
            traverse=traverse->next;
        }
    }
    return check;
}
/*Function to free the graph & the adjacency lists inside the graph*/
void freeGraph(struct Node* graph[], long totalVertices){
    for(long i=0; i<totalVertices; i++){
        struct Node* traverse = graph[i];
        while(traverse!=NULL){
            struct Node* temp = traverse;
            traverse=traverse->next;
            free(temp);
        }
    }
    free(graph);
}
/*Function to sort the graph in order*/
void sortGraph(struct Node* graph[], long totalVertices){
    struct Node* temp;
    for(long i=0; i<totalVertices; i++){
        for(long j=0; j<totalVertices; j++){
            if(strcmp(graph[i]->data, graph[j]->data)<0){
                temp=graph[i];
                graph[i]=graph[j];
                graph[j]=temp;
            }
        }
    }
}
int main(int argc, char** argv){
    if(argc<2){
        printf("Enter command line arguments");
        exit(EXIT_SUCCESS);
    }
    FILE*fp1=fopen(argv[1],"r");
    long totalVertices;
    /*opens & creates an empty graph*/
    fscanf(fp1, "%ld\n", &totalVertices);
    struct Node** graph = malloc(sizeof(struct Node*)*totalVertices);
    char vertex[81];
    for(long i=0; i<totalVertices; i++){
        fscanf(fp1, "%s\n", vertex);
        graph[i]=malloc(sizeof(struct Node));
        strcpy(graph[i]->data, vertex);
        graph[i]->edgeWeight=0;
        graph[i]->next=NULL;
    }
    char vertex1[81];
    char vertex2[81];
    long edgeWeight=0;
    /*adds the edges to the graph*/
    while(fscanf(fp1, "%s %s %ld\n", vertex1, vertex2, &edgeWeight)!=EOF){
        graph = addEdge(graph, totalVertices, vertex1, vertex2, edgeWeight);
    }
    sortGraph(graph, totalVertices);
    int check = detectCycle(graph, totalVertices);
    FILE*fp2=fopen(argv[2],"r");
    while(fscanf(fp2, "%s\n", vertex)!=EOF){
        printf("\n");
        if(check!=1){
            algorithm1(graph, totalVertices, vertex);
        }
        else{
            printf("Cycle\n");
        }
    }
    /*Clean up malloc & close file*/
    freeGraph(graph, totalVertices);
    fclose(fp1);
    //fclose(fp2);
    return EXIT_SUCCESS;
}
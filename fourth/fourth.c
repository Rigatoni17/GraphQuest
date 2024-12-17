#include<stdio.h>
#include<stdlib.h>
#include<string.h>
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
int indexSearch(struct Node* graph[], long totalVertices, char vertex[]){
    long index=-1;
    for(long i=0; i<totalVertices; i++){
        if(strcmp(graph[i]->data, vertex)==0){
            index=i;
        }
    }
    return index;
}
/*Recursive Function that finds & prints the dfs traversal of the given graph*/
void graphDFS(struct Node* graph[], long totalVertices, char vertex[], long visitArr[]){
    printf("%s ", vertex);
    long index=indexSearch(graph, totalVertices, vertex);
    visitArr[index]=1;
    struct Node* traverse = graph[index];
    while(traverse!=NULL){
        long traverseIndex=indexSearch(graph, totalVertices, traverse->data);
        if(visitArr[traverseIndex]!=1){
            graphDFS(graph, totalVertices, traverse->data, visitArr);
        }
        traverse=traverse->next;
    }
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
    long visitArr[totalVertices];
    for(long i=0; i<totalVertices; i++){
        visitArr[i]=0;
    }
    long *pVisit = visitArr;
    /*counter to check if all of the vertexes have been visited*/
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
                graphDFS(graph, totalVertices, vertex, pVisit);
            }
        }
        for(long i=0; i<totalVertices; i++){
            if(visitArr[i]==1){
                count++;
            }
        }
    }
    /*Clean up malloc & close file*/
    freeGraph(graph, totalVertices);
    fclose(fp1);
    return EXIT_SUCCESS;
}
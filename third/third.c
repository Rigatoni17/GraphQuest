#include<stdio.h>
#include<stdlib.h>
#include<string.h>
/*Structure for the linked list*/
struct Node{
    char data[81];
    struct Node* next;
};
/*Function that inserts into the linked list*/
void insertList(struct Node* graph[], long i, long totalVertices, char vertex1[], char vertex2[]){
    struct Node* temp = malloc(sizeof(struct Node));
    strcpy(temp->data, vertex2);
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
struct Node** addEdge(struct Node* graph[], long totalVertices, char vertex1[], char vertex2[]){
    /*vertex1 -> vertex2*/
    for(long i=0; i<totalVertices; i++){
        if(strcmp(graph[i]->data, vertex1)==0){
            insertList(graph, i, totalVertices, vertex1, vertex2);
        }
    }
    /*vertex2 -> vertex1*/
    for(long i=0; i<totalVertices; i++){
        if(strcmp(graph[i]->data, vertex2)==0){
            insertList(graph, i, totalVertices, vertex2, vertex1);
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
/*Function that finds & prints the BFS of the vertex*/
void listBFS(struct Node* graph[], long totalVertices, char vertex[]){
    long visitArr[totalVertices];
    for(long i=0; i<totalVertices; i++){
        visitArr[i]=0;
    }
    long index=indexSearch(graph, totalVertices, vertex);
    visitArr[index]=1;
    char *queue[totalVertices];
    queue[0]=vertex;
    long head=0;
    long tail=1;
    while(head!=tail){
        char current[81];
        strcpy(current, queue[head]);
        head++;
        for(long i=0; i<totalVertices; i++){
            if(strcmp(graph[i]->data, current)==0){
                struct Node* traverse = graph[i];
                traverse=traverse->next;
                while(traverse!=NULL){
                    long traverseIndex=indexSearch(graph, totalVertices, traverse->data);
                    if(visitArr[traverseIndex]!=1){
                        visitArr[traverseIndex]=1;
                        queue[tail]=traverse->data;
                        tail++;
                    }
                    traverse=traverse->next;
                }
            }
        }
        if((queue[head-1]!=NULL)&&(head!=tail)){
            printf("%s ", queue[head-1]);
        }
    }
    printf("%s\n", queue[head-1]);
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
        graph[i]->next=NULL;
    }
    char vertex1[81];
    char vertex2[81];
    /*adds the edges to the graph*/
    while(fscanf(fp1, "%s %s\n", vertex1, vertex2)!=EOF){
        graph = addEdge(graph, totalVertices, vertex1, vertex2);
    }
    sortGraph(graph, totalVertices);
    /*opens the queries & prints the results*/
    FILE*fp2=fopen(argv[2],"r");
    while(fscanf(fp2, "%s\n", vertex)!=EOF){
        listBFS(graph, totalVertices, vertex);
    }
    /*Clean up malloc & close file*/
    freeGraph(graph, totalVertices);
    fclose(fp1);
    fclose(fp2);
    return EXIT_SUCCESS;
}
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
/*Priority Queue Function that Insert Sorts*/
void insertPQueue(struct Node** queueHead, char vertex[], long distance){
    struct Node* temp=malloc(sizeof(struct Node));
    strcpy(temp->data, vertex);
    temp->edgeWeight=distance;
    temp->next=NULL;
    if(*queueHead==NULL){
        *queueHead=temp;
    }
    else if(distance<(*queueHead)->edgeWeight){
        temp->next=*queueHead;
        *queueHead=temp;
    }
    else{
        struct Node* traverse=*queueHead;
        while((traverse->next!=NULL)&&(traverse->next->edgeWeight<=distance)){
            traverse=traverse->next;
        }
        temp->next=traverse->next;
        traverse->next=temp;
    }
}
/*Priority Queue Delete Function & returns deleted node*/
struct Node* deletePQueue(struct Node** queueHead){
    struct Node* temp=*queueHead;
    *queueHead=(*queueHead)->next;
    temp->next=NULL;
    return temp;
}
/*Function that deletes the given node in the list*/
void deleteNode(struct Node** queueHead, char vertex[]){
    if(*queueHead!=NULL){
	    struct Node* temp=*queueHead;
        /*DELETE at the front of the list*/
        if(strcmp((*queueHead)->data, vertex)==0){
            *queueHead=(*queueHead)->next;
		    free(temp);
        }
        /*DELETE in the middle or end of the list*/
        else{
            struct Node *traverse = *queueHead;
            while((traverse->next!=NULL)&&(strcmp(traverse->next->data, vertex)!=0)){
                traverse = traverse->next;
            }
		    temp=traverse->next;
            traverse->next=traverse->next->next;
		    free(temp);
        }
    }
}
/*Function that uses Dijkstra's Algorithm to find the shortest path*/
void Dijkstra(struct Node* graph[], long totalVertices, char vertex[]){
    long distanceArr[totalVertices];
    struct Node* priorityqueue=NULL;
    long indexVertex=indexSearch(graph, totalVertices, vertex);
    distanceArr[indexVertex]=0;
    insertPQueue(&priorityqueue, vertex, distanceArr[indexVertex]);
    for(long i=0; i<totalVertices; i++){
        if(strcmp(graph[i]->data, vertex)!=0){
            distanceArr[i]=LONG_MAX;
            insertPQueue(&priorityqueue, graph[i]->data, distanceArr[i]);
        }
    }
    while(priorityqueue!=NULL){
        struct Node* u = deletePQueue(&priorityqueue);
        long indexU=indexSearch(graph, totalVertices, u->data);
        struct Node* traverse=graph[indexU];
        while(traverse!=NULL){
            long indexV=indexSearch(graph, totalVertices, traverse->data);
            long alt=LONG_MAX;
            if(distanceArr[indexU]!=LONG_MAX){
                alt=distanceArr[indexU]+traverse->edgeWeight;
            }
            if(alt<distanceArr[indexV]&&(distanceArr[indexU]!=LONG_MAX)){
                distanceArr[indexV]=alt;
                deleteNode(&priorityqueue, traverse->data);
                insertPQueue(&priorityqueue, traverse->data, alt);
            }
            traverse=traverse->next;
        }
        free(u);
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
    FILE*fp2=fopen(argv[2],"r");
    while(fscanf(fp2, "%s\n", vertex)!=EOF){
        Dijkstra(graph, totalVertices, vertex);
        printf("\n");
    }
    /*Clean up malloc & close file*/
    freeGraph(graph, totalVertices);
    fclose(fp1);
    fclose(fp2);
    return EXIT_SUCCESS;
}
#include <locale.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


/////////////////////////// ?? ?????? /////////////////////////
// ??? ????
enum TYPE_NODE {
    CROSSROAD,
    DEAD_END,
    EXIT,
};

// ????????? ????
struct Node {
    size_t x;
    size_t y;
    enum TYPE_NODE type;
    struct Node* next; 
};

// ????????? ?????? ?????
struct ListNode {
    struct Node* head;
};

// ????????? ???????????? ????
struct Graph {
    size_t size;
    struct ListNode* listNode;
};

// ?????? ??????????????? ????????? ?? ??????? 
const char *errmsgs[] = {
        "OK",
        "Graph is empty!",
        "Wrong type",
        "Number more than graph size",
        "File not open",
        "Unable o read" };

// ???????????? ???? ??? ??????????? ??????? 
const char *msgs[] = { 
        "1. Get random graph",
        "2. Read graph from file",
        "3. Write graph in file",
        "4. Add node in graph" ,
        "5. Add edge in graph",
        "6. Delete node",
        "7. Escape way",
        "8. Show graph",
        "9. Clear graph",
        "10. Way timing",
        "11. Exit" };

// ????? ??????????? 
const int NMsgs = sizeof(msgs) / sizeof(msgs[0]);
////////////////////////////////////////////////////////////////



/////////////////////// ????????? //////////////////////////////
struct Node* createNode(size_t, size_t, enum TYPE_NODE);
struct Node* copyNode(struct Node*);
void addNode(struct Graph*, struct Node*);
size_t getNumberNode(const struct Graph*, const struct Node*);
void addEdge(struct Graph*, size_t, size_t, bool);
void removeNode(struct Graph*, size_t);
struct Graph* createGraph(size_t);
void printGraph(const struct Graph*);
struct Graph* readGraphFromFile(char*);
void writeGraphToFile(char*, const struct Graph*);
bool isThereWay(struct Node*);
bool haveBeenHere(struct Graph*, struct Node*, size_t*, size_t);
struct Node* getAbsolutelyNode(struct Graph*, size_t);
struct Node* findWay(struct Graph*, struct Node*, size_t*, size_t);
void printWays(struct Graph*, size_t);
struct Graph* getRandomGraph();
void destroyGraph(struct Graph*);
char* getStrTypeNode(const enum TYPE_NODE);
////////////////////////////////////////////////////////////////


////////////////////// ?????? + ??????? ????? /////////////////
int dialog(const char* mes[], int max)
{
   int rc,gi;
    do
    {
        for ( int i = 0; i < max; i++ )
        {
            printf(" %s \n", mes[i]);
        }
        printf("Make your choice: ");
        gi = get_int(&rc);
        if ( rc > max )
        {
            printf("Enter a number from 1 to 11 \n");
        }
    }
    while ( rc < 1 || rc > max );   // ????????? ?? ????????????
    return rc;                  // ?????????? ????? ??????
}

char *get_str() 
{
	char *ptr = (char *)malloc(1);
	char buf[81];
	int n, len = 0;
	*ptr = '\0';
	do 
    {
		n = scanf("%80[^\n]", buf);
		if (n < 0) 
        {
			free(ptr);
			ptr = NULL;
			continue;
		}
		if (n == 0)
        {
			scanf("%*c");
        }
        else 
        {
			len += strlen(buf);
			ptr = (char *)realloc(ptr, len + 1);
			strcat(ptr, buf);
		}
	} 
    while (n > 0);
	return ptr;
}

int get_int(int *a) 
{
	int n;
	do 
    {
		n = scanf("%d", a, sizeof(int));
		if (n < 0)			/*????????? ????? ????? */
		{	
            return 0;
        }
        if (n == 0) 
        {			/*????????? ???????????? ?????? - ?????? */
			printf("%s\n", "Enter a number: \n");
			clean_stdin();
		}
	} 
    while (n == 0);
	clean_stdin();
	return 1;
}

void clean_stdin() 
{
   while(fgetc(stdin) != '\n');
}
////////////////////////////////////////////////////////////////


////////////////////// ??????? ?????????? ? ?????? ////////////
struct Node* createNode(size_t x, size_t y, enum TYPE_NODE type)            // ??????? ????????? ????? ???????
{
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->x = x;
    newNode->y = y;
    newNode->type = type;
    newNode->next = NULL;
    return newNode;
}

struct Node* copyNode(struct Node* node)            // ??????? ?????????? ???????
{
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->x = node->x;
    newNode->y = node->y;
    newNode->type = node->type;
    newNode->next = node->next;
    return newNode;
}

void addNode(struct Graph* graph, struct Node* node)            // ??????? ????????? ????? ??????? ? ???? ??????? ??? ??? ??????
{
    graph->size++;
    struct ListNode* newListNode = (struct ListNode*)malloc(graph->size * sizeof(struct ListNode));

    // ???????? ??????????
    for (size_t i = 0; i < graph->size - 1; i++)
    {
        newListNode[i].head = graph->listNode[i].head;
    }

    // ?????????? ????? ??????? ? ????? ?????? ??????
    newListNode[graph->size - 1].head = node;

    // ???? ???? ?? ??? ?????? ????????? ??????
    if (graph->size != 1)
    {
        free(graph->listNode);
    }
    graph->listNode = newListNode;
}

size_t getNumberNode(const struct Graph* graph, const struct Node* node)            // ??????? ?????????? ????? ????
{
    for (size_t i = 0; i < graph->size; i++)
    {
        if (graph->listNode[i].head->x == node->x && graph->listNode[i].head->y == node->y)
        {
          return i;
        }
    }
    return 0;
}

void addEdge(struct Graph* graph, size_t src, size_t dest, bool isNotDirected)          // ??????? ??????????? ????? ????? ? ????
{
    
    if (src > graph->size - 1 || dest > graph->size - 1)             // ???? ???????? ??? ???????? ????? ?????? ??????? ?????
    {
        printf("%s \n", errmsgs[3]);
        return;
    }
    if (src == dest)            // ???? ???????????? ???? ?????????? ?????
    {
        return;
    }
    size_t numberNode = 0;
    bool isAlreadyLinked = false;           // ???????? ?? ??????????? ?????
    struct Node* node = graph->listNode[src].head;

    // ???? ????????
    while (node) 
    {
        numberNode = getNumberNode(graph, node);
        if (numberNode == dest) 
        {
            isAlreadyLinked = true;
            break;
        }
        node = node->next;
    }

    // ???? ??????? ?? ???? ??????? ??????
    if (!isAlreadyLinked) 
    {
        struct Node* nodeDest = copyNode(graph->listNode[dest].head);
        nodeDest->next = graph->listNode[src].head->next;
        graph->listNode[src].head->next = nodeDest;
    }

    // ?????? ? ??? ???????
    if (isNotDirected) 
    {
        isAlreadyLinked = false;
        node = graph->listNode[dest].head;
        while (node) 
        {
            numberNode = getNumberNode(graph, node);
            if (numberNode == src) 
            {
                isAlreadyLinked = true;
                break;
            }
            node = node->next;
        }
        if (!isAlreadyLinked) 
        {
            struct Node* nodeSrc = copyNode(graph->listNode[src].head);
            nodeSrc->next = graph->listNode[dest].head->next;
            graph->listNode[dest].head->next = nodeSrc;
        }
    }
}

void removeNode(struct Graph* graph, size_t src)            // ???????? ???? ?? ?????
{
    if (src > graph->size - 1) 
    {
        printf("%s \n", errmsgs[3]);
        return;
    }

    struct Node* removingNode = graph->listNode[src].head;

    // ???????? ???? ????? ????????? ? ?????
    for (size_t i = 0; i < graph->size; i++) 
    {
        if (i != src) 
        {
            struct Node* prevNode = NULL;
            struct Node* node = graph->listNode[i].head;
            while (node) 
            {
                if (node->x == removingNode->x && node->y == removingNode->y) 
                {
                    if (node->next)
                    {
                        prevNode->next = node->next;
                    }
                    else
                    {
                        prevNode->next = NULL;
                    }
                    free(node);
                    break;
                }
                prevNode = node;
                node = node->next;
            }
        }
    }
    struct Node* nextRemovingNode = removingNode->next;

    // ????????? ?????? ???????? ?????? ?????
    while (nextRemovingNode) 
    {
        free(removingNode);
        removingNode = nextRemovingNode;
        nextRemovingNode = nextRemovingNode->next;
    }
    free(removingNode);

    // ???????????????? ?????? ?? ???????? ???????????
    graph->size--;
    struct ListNode* newListNode = (struct ListNode*)malloc(graph->size * sizeof(struct ListNode));
    size_t cPos = 0;
    for (size_t i = 0; i < graph->size + 1; i++) 
    {
        if (i != src) 
        {
            newListNode[cPos].head = graph->listNode[i].head;
            cPos++;
        }
    }

    if (graph->size != 1)
    {
        free(graph->listNode);
    }
    graph->listNode = newListNode;
}
////////////////////////////////////////////////////////////////


////////////////////// ??????? ?????????? ? ?????? ////////////
struct Graph* createGraph(size_t size)          // ??????? ????????? ???? 
{
    struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));
    graph->size = size;

    if (size > 0) 
    {
        graph->listNode = (struct ListNode*)malloc(size * sizeof(struct ListNode));
        for (size_t i = 0; i < size; i++)
        {
            graph->listNode[i].head = NULL;
        }
    }
    return graph;
}

void printGraph(const struct Graph* graph)
{
    for (size_t i = 0; i < graph->size; i++) {
        struct Node* mainNode = graph->listNode[i].head;
        struct Node* nextNode = mainNode->next;

        printf("\n List of vertex %zu\n: ", i);
        if(!mainNode -> next)
        {
            printf("(x: %zu, y: %zu, type: %s); \n", mainNode->x, mainNode->y,getStrTypeNode(mainNode->type));
            continue;
        }
        while (nextNode) {
            printf("Vertex #%zu (x: %zu, y: %zu, type: %s) -> Vertex #%zu (x: %zu, y: %zu, time: %s)\n",
                   i, mainNode->x, mainNode->y, getStrTypeNode(mainNode->type), getNumberNode(graph, nextNode),
                   nextNode->x, nextNode->y, getStrTypeNode(nextNode->type));
            nextNode = nextNode->next;
        }
        printf("\n");
    }
}

struct Graph* readGraphFromFile(char* pathname)         // ?????????? ????? ?? ?????
{
    struct Graph* graph = NULL;
    FILE* file = fopen(pathname, "rb");
    if (file == NULL) 
    {
        printf("%s \n", errmsgs[4]);
        return NULL;
    }
    size_t size = 0;
    char line[255];
    if (fgets(line, sizeof(line), file) == NULL) 
    {
        printf("%s  \n", errmsgs[5]);
        return NULL;
    }
    size = atoi(line);
    graph = createGraph(0);
    for (size_t i = 0; i < size; i++) 
    {
        if (fgets(line, sizeof(line), file) == NULL) 
        {
            printf("%s  \n", errmsgs[5]);
            return NULL;
        }
        size_t cPos;
        size_t cPosStr = 0;
        char str[10]; // 10 is max length of a word
        for (cPos = 0; cPos < sizeof(line); cPos++) 
        {
            if (isdigit(line[cPos]))
            {    
                str[cPosStr++] = line[cPos];
            }
            else
            {
                break;
            }
        }
        size_t x = atoi(str);

        // ???????? ??????
        memset(str, 0, 10 * sizeof(char));
        cPosStr = 0;
        for (cPos = cPos + 1; cPos < sizeof(line); cPos++) 
        {
            if (isdigit(line[cPos]))
            {    
                str[cPosStr++] = line[cPos];
            }
            else
            {
                break;
            }
        }
        size_t y = atoi(str);
        memset(str, 0, sizeof(str));
        cPosStr = 0;
        for (cPos = cPos + 1; cPos < sizeof(line); cPos++)
        {
            if (isalpha(line[cPos])) 
            {
                str[cPosStr++] = line[cPos];
                if (cPosStr == 10)
                {    
                    break;
                }
            } 
            else
            {    
                break;
            }
        }
        enum TYPE_NODE type;
        if (strstr(str, "crossroad") != 0)
        {
            type = CROSSROAD;
        }
        else if (strstr(str, "deadend") != 0)
        {
            type = DEAD_END;
        }
        else
        {
            type = EXIT;
        }
        addNode(graph, createNode(x, y, type));
    }
    for (size_t i = 0; i < size; i++) {
        if (fgets(line, sizeof(line), file) == NULL) 
        {
            printf("%s  \n", errmsgs[5]);
            return NULL;
        }
        size_t j = 0;
        while (isdigit(line[j])) 
        {
            if (line[j] == '1')
            {
                addEdge(graph, i, j, false);
            }
            j++;
        }
    }
    fclose(file);
    return graph;
}

void writeGraphToFile(char* pathname, const struct Graph* graph)            // ?????? ? ????
{
    FILE* file = fopen(pathname, "w");
    fprintf(file, "%zu\n", graph->size);
    for (size_t i = 0; i < graph->size; i++)
    {
        struct Node* node = graph->listNode[i].head;
        char* typeNodeStr = NULL;
        if (node->type == CROSSROAD)
        {
            typeNodeStr = "crossroad";
        }
        else if (node->type == DEAD_END)
        {  
            typeNodeStr = "deadend";
        }
        else
        {
            typeNodeStr = "exit";
        }
        fprintf(file, "%zu,%zu,%s\n", node->x, node->y, typeNodeStr);
    }

    // ?????????? ? ????
    for (size_t i = 0; i < graph->size; i++) {
        size_t* nodeWays = (size_t*)malloc(graph->size * sizeof(size_t));
        for (size_t j = 0; j < graph->size; j++)
        {    
            nodeWays[j] = 0;
        }
        struct Node* node = graph->listNode[i].head->next;
        while (node) 
        {
            size_t numberNode;
            for (numberNode = 0; numberNode < graph->size; numberNode++)
            {
                if (node->x == graph->listNode[numberNode].head->x && node->y == graph->listNode[numberNode].head->y) 
                {
                    nodeWays[numberNode] = 1;
                    break;
                }
            }
            node = node->next;
        }
        for (size_t j = 0; j < graph->size; j++)
        {    
            fprintf(file, "%zu", nodeWays[j]);
        }        
        fprintf(file, "\n");
        if (nodeWays)
        {
            free(nodeWays);
        }
    }
    fclose(file);
}

bool isThereWay(struct Node* node)          // ???????? ?? ????????????? ???? ?? ??????? ???????????? ??? ?????? ??????
{
    while (node->next) 
    {
        if (node->type == EXIT || node->type == DEAD_END)
        {
            break;
        }
        node = node->next;
    }
    return node->type == EXIT;
}

bool haveBeenHere(struct Graph* graph, struct Node* node, size_t* prevWay, size_t deep)         // ???????? ????????? ?? ?? ??? ??????? ? ???????? ??????
{
    for (size_t i = 0; i < deep; i++) 
    {
        if (node->x == graph->listNode[prevWay[i]].head->x && node->y == graph->listNode[prevWay[i]].head->y)
        {
            return true;
        }
    }
    return false;
}

struct Node* getAbsolutelyNode(struct Graph* graph, size_t src)         // ??????? ???????????? ?????????? ????????? ?? ????
{
    if (src > graph->size) 
    {
        printf("%s \n", errmsgs[3]);
        return NULL;
    }
    return graph->listNode[src].head;
}

struct Node* findWay(struct Graph* graph, struct Node* node, size_t* prevWay, size_t deep)          // ??????? ??? ?????? ????? ??????
{
    if (node->type == EXIT) 
    {
        return node;
    } 
    else if (node->type == CROSSROAD) 
    {
        struct Node* nextNode = node;
        while (nextNode->next) 
        {
            if (!haveBeenHere(graph, nextNode->next, prevWay, deep)) 
            {
                prevWay[deep + 1] = getNumberNode(graph, nextNode->next);
                struct Node* _nextNode = findWay(graph, getAbsolutelyNode(graph, prevWay[deep + 1]), prevWay, deep + 1);
                if (_nextNode) 
                {
                    if (isThereWay(_nextNode)) 
                    {
                        nextNode->next = _nextNode;
                        return nextNode;
                    }
                }
            }
            nextNode = nextNode->next;
        }
    }
    return NULL;
}

void printWays(struct Graph* graph, size_t src)         // ??????? ?????? ???? ?????? ???? ?? ???? ??? ????????? ?? ??????
{
    if (src > graph->size - 1) 
    {
        printf("%s \n", errmsgs[3]);
        return;
    }
    size_t* prevWay = (size_t*)malloc(graph->size * sizeof(size_t));
    memset(prevWay, 0, graph->size * sizeof(size_t));
    if (graph->size > 0)
    {
        prevWay[0] = src;
    }
    struct Node* srcNode = copyNode(graph->listNode[src].head);
    struct Node* way = findWay(graph, srcNode, prevWay, 0);

    // ???? ???? ??? ??????
    if (way) 
    {
        printf("Way found! \n");
        while (way) 
        {
            printf("-> (x: %zu, y: %zu) ", way->x, way->y);
            if (way->type == EXIT)
            {
                break;
            }
            else
            {   
                way = way->next;
            }
        }
        printf("-> exit\n");
    } 
    else 
    {
        printf("You shall not pass... \n");
    }
    if (prevWay)
    {
        free(prevWay);
    }
}

struct Graph* getRandomGraph()          // ???????? ?????????? ?????
{
    struct Graph* graph = createGraph(0);

    srand((unsigned)time(NULL)); // ????????? ???????

    size_t countNodes = rand() % 10 + 1;
    for (size_t i = 0; i < countNodes; i++) 
    {
        size_t x = rand() % 10;
        size_t y = rand() % 10;
        enum TYPE_NODE type = CROSSROAD;
        size_t randType = rand() % 3 + 1;
        switch (randType) 
        {
            case 1:
                type = CROSSROAD;
                break;
            case 2:
                type = DEAD_END;
                break;
            case 3:
                type = EXIT;
                break;
        }
        addNode(graph, createNode(x, y, type));
    }
    for (size_t i = 0; i < countNodes; i++) 
    {
        size_t src = rand() % countNodes;
        size_t dest = rand() % countNodes;
        addEdge(graph, src, dest, true);
    }
    return graph;
}

struct Graph* t_getRandomGraph(int count)
{
    struct Graph* graph = createGraph(0);
    srand((unsigned)time(NULL)); // ????????? ???????
    int countNodes = count;
    //printf(" Nodes number is %d \n but ditected is %d \n", countNodes, count);
    for (size_t i = 0; i < countNodes; i++) 
    {
        size_t x = rand() % 10;
        size_t y = rand() % 10;
        enum TYPE_NODE type = CROSSROAD;
        size_t randType = rand() % 3 + 1;
        switch (randType) 
        {
            case 1:
                type = CROSSROAD;
                break;
            case 2:
                type = DEAD_END;
                break;
            case 3:
                type = EXIT;
                break;
        }
        addNode(graph, createNode(x, y, type));
    }
    for (size_t i = 0; i < countNodes; i++) 
    {
        size_t src = rand() % countNodes;
        size_t dest = rand() % countNodes;
        addEdge(graph, src, dest, true);
    }
    
    return graph;
}

void destroyGraph(struct Graph* graph)           // ???????? ????? ?? ??????
{
    for (size_t i = 0; i < graph->size; i++) 
    {
        struct Node* node = graph->listNode[i].head;
        struct Node* nextNode = node->next;
        struct Node* prevNode = NULL;
        while (nextNode) 
        {
            prevNode = nextNode;
            nextNode = nextNode->next;
            free(prevNode);
        }
        if (nextNode)
        {
            free(nextNode);
        }
        if (node)
        {
            free(node);
        }   
        graph->size--;
    }
    if (graph)
    {
        free(graph);
    }
}

char* getStrTypeNode(const enum TYPE_NODE type)
{
    if (type == CROSSROAD) {
        return "crossroad";
    } else if (type == DEAD_END) {
        return "deadend";
    } else {
        return "exit";
    }
}

void createDot(const struct Graph* graph, char *fname)
{
    char *name = (char *)malloc(strlen(fname) + 5);
	strcpy(name, fname);
	strcat(name, ".dot");
	FILE *fd_dot = fopen(name, "w+");
	fprintf(fd_dot, "digraph G {node [shape=\"circle\"];rankdir=\"LR\";");
    for (size_t i = 0; i < graph->size; i++) 
    {
        struct Node* mainNode = graph->listNode[i].head;
        struct Node* nextNode = mainNode->next;
        if(!mainNode -> next)
        {
            fprintf(fd_dot, "\"%d - %d %s\";", mainNode->x, mainNode->y,getStrTypeNode(mainNode->type));
			continue;
        }
        while (nextNode) 
        {
            fprintf(fd_dot," \"%zu - %zu %s\" -> \"%zu - %zu %s\";", mainNode->x, mainNode->y, getStrTypeNode(mainNode->type), nextNode->x, nextNode->y, getStrTypeNode(nextNode->type));
            
            nextNode = nextNode->next;
        }
    }
    fprintf(fd_dot, "}");
	free(name);
	fclose(fd_dot);
	fd_dot = NULL;
}

int t_printWays(struct Graph* graph, size_t src)         // ??????? ?????? ???? ?????? ???? ?? ???? ??? ????????? ?? ??????
{
    //printf(" Error 1 \n");
    size_t* prevWay = (size_t*)malloc(graph->size * sizeof(size_t));
    //printf(" Error 2 \n");
    memset(prevWay, 0, graph->size * sizeof(size_t));
   // printf(" Error 3 \n");
    if (graph->size > 0)
    {
        prevWay[0] = src;
    }
    struct Node* srcNode = copyNode(graph->listNode[src].head);
    struct Node* way = findWay(graph, srcNode, prevWay, 0);

    // ???? ???? ??? ??????
    if (way) 
    {
        return 1;
    } 
    else 
    {
        return 2;
    }
    if (prevWay)
    {
        free(prevWay);
    }
    return 0;
}

void t_findway()
{
    struct Graph* graph = createGraph(0);
    int n = 10, cnt = 2500, i, m;
	clock_t first, last;
	srand(time(NULL));
	int ver[1000];
	int mid[10];
	int dtime;
	int ftime = 0;
	while (n-- > 0)
	{
        dtime = 0;
		for(int g = 0; g < 10; ++g)
		{
            graph = t_getRandomGraph(cnt);
            
            for( int i = 0; i < 1000; ++i)
            {
                int num = 35 + rand() % 893;
                ver[i] = num;
            }
            m = 0;
            first = clock();
            for( int i = 0; i < 1000; ++i)
            {
                if(t_printWays(graph, ver[i]) == 1)
                {
                    ++m;
                }
            }
            last = clock();
            mid[g] = last - first;
            dtime += mid[g];
            graph = t_getRandomGraph(cnt);
        }
        ftime = dtime / 10;
       
        printf("%d items was found\n", m);
		printf("test #%d, number of nodes = %d, time = %d\n", 10 - n, (10 - n) * cnt, ftime);

    }
    return 0;
}
////////////////////////////////////////////////////////////////


int main()
{
    struct Graph* graph = createGraph(0);
    int rc;
    while (1) 
    {  
        rc = dialog(msgs, NMsgs);   // ??????? ?????? ????
        if (rc == 1) 
        {
            if (graph)
            {
                destroyGraph(graph);
            }
            graph = getRandomGraph();
            printf("%s \n", errmsgs[0]);
        } 
        else if (rc == 2) 
        {
            char *pathname = NULL;
            printf("Enter file name: ");
            pathname = get_str();
            if (graph)
            {
                destroyGraph(graph);
            }
            graph = readGraphFromFile(pathname);
            printf("%s \n", errmsgs[0]);
        } 
        else if (rc == 3)
        {
            if (!graph) 
            {
                printf("%s \n", errmsgs[1]);
                
                continue;
            }
            char *pathname = NULL;
            printf("Enter file name: ");
            pathname = get_str();
            writeGraphToFile(pathname, graph);
            printf("%s \n", errmsgs[0]);
        } 
        else if (rc == 4) 
        {
            /*if (!graph) 
            {
                printf("%s \n", errmsgs[1]);
                continue;
            }*/
            int x, y, gi;
            char *typeStr = NULL;
            enum TYPE_NODE type;
            printf("Enter x: ");
            gi = get_int(&x);
            printf("Enter y: ");
            gi = get_int(&y);
            printf("Enter node type: \n crossroad, \n deadend, \n exit, \n Your choice: ");
            typeStr = get_str();
            if (strncmp(typeStr, "crossroad", 10) == 0) 
            {
                type = CROSSROAD;
            } 
            else if (strncmp(typeStr, "deadend", 8) == 0) 
            {
                type = DEAD_END;
            } 
            else if (strncmp(typeStr, "exit", 5) == 0) 
            {
                type = EXIT;
            } 
            else 
            {
                printf("%s \n", errmsgs[2]);
                continue;
            }
            addNode(graph, createNode(x, y, type));
            printf("%s \n", errmsgs[0]);
        } 
        else if (rc == 5) 
        {
            if (!graph) 
            {
                printf("%s \n", errmsgs[1]);
                
                continue;
            }
            int src, dest, gi;
            printf("Enter source number of node: ");
            gi = get_int(&src);
            printf("Enter destanation number of node: ");
            gi = get_int(&dest);
            addEdge(graph, src, dest, true);
            printf("%s \n", errmsgs[0]);
        } 
        else if (rc == 6) 
        {
            if (!graph) 
            {
                printf("%s \n", errmsgs[1]);
                continue;
            }
            int src, gi;
            printf("Enter number of node u want to delete: ");
            gi = get_int(src);
            removeNode(graph, src);
            printf("%s \n", errmsgs[0]);
        } 
        else if (rc == 7) 
        {
            if (!graph) 
            {
                printf("%s \n", errmsgs[1]);
                
                continue;
            }
            int src, gi;
            printf("Enter a number of node u want escape from: ");
            gi = get_int(&src);
            clock_t start = clock();
            printWays(graph, src);
            clock_t end = clock();
            double seconds = (double)(end - start) / CLOCKS_PER_SEC;
            printf("Escape time is: %f seconds\n", seconds);
            
        } 
        else if (rc == 8) 
        {
            char *pathname = NULL;
            printf("Enter the file name: ");
            pathname = get_str();
            createDot(graph, pathname);
            if (!graph) 
            {
                printf("%s \n", errmsgs[1]);
                continue;
            }
            printGraph(graph);
        } 
        else if (rc == 9)
        {
            if (graph)
            {
                destroyGraph(graph);
            }
            graph = createGraph(0);
        }
        else if (rc == 10)
        {
            t_findway();
        }
        else if (rc == NMsgs) 
        {
            if (graph)
            {
                destroyGraph(graph);
                printf("------------------------------------------------------------- \n");
                printf("See you");
            }
            return 0;
        }
        printf("------------------------------------------------------------- \n");
    }
    return 0;
}
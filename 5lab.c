#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>

///////////////////// ˜˜ ˜˜˜˜˜˜ //////////////////////
typedef struct Node
{
	char* key;
	char* info;
	struct Node* left;
	struct Node* right;
	struct Node* par;
}Node;

const char* msgs[] =
{
	"1. Add element",
	"2. Read tree from file",
	"3. Find by key",
	"4. Search key by N symbols",
	"5. Delete tree",
	"6. Timing",
	"7. Show tree more than defined key",
	"8. Show tree",
    "9. Exit"
};

const char *errmsgs[] = { "OK", "˜˜˜˜˜˜˜ ˜ ˜˜˜˜˜ ˜˜˜˜˜˜ ˜˜˜ ˜˜˜˜˜˜˜˜˜˜", "˜˜˜˜ ˜˜ ˜˜˜˜˜˜", "˜˜˜ ˜˜˜˜˜˜˜˜ ˜ ˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜˜˜˜", "˜˜˜˜˜˜˜ ˜˜˜˜˜" };

int m = 0;
const int NMsgs = sizeof(msgs) / sizeof(msgs[0]);
//////////////////////////////////////////////////////


///////////////////// ˜˜˜˜˜˜˜˜˜ ////////////////////////
int get_int(int* a);
char* get_str();
void d_add(Node* root);
int add(Node** root, char* key, char* info);
Node* addNode(char* key, char* info, Node* parent);
void d_r_from_file(Node* root);
char* r_from_file(FILE* file);
void d_find_by_key_N(Node* root);
Node* find_by_key(Node** root, char* key);
void find_by_key_N(Node** root, char* key, int n);
void delete(Node** root, char* key);
void d_delete(Node* root);
void delete_table(Node* root);
void d_show(Node* root);
Node* print_for_key(Node** root, char* key);
void d_show_tree(Node* root, int p, int s);
void showLine(char* c, int p, int s);
void print(Node* root);
///////////////////////////////////////////////////////


///////////////////// ˜˜˜˜˜˜˜ /////////////////////////
///////////////// ˜˜˜˜˜˜ + ˜˜˜˜˜˜˜ ˜˜˜˜˜ /////////////
int dialog(const char *msgs[], int N) 
{
	char *errmsgs = "";
	int rc, i, n;
	do 
    {
		puts(errmsgs);
		errmsgs = "Enter a number from 1 to 9 \n";
		for (i = 0; i < N; ++i)	// ˜˜˜˜˜ ˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜˜˜˜
		{	
            puts(msgs[i]);
        }
        printf("Your choice: ");
		n = get_int(&rc);	// ˜˜˜˜ ˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜˜˜˜˜
		if (n == 0)		// ˜˜˜˜˜ ˜˜˜˜˜ - ˜˜˜˜˜ ˜˜˜˜˜˜
		{
            rc = 0;
        }
    } 
    while (rc < 0 || rc >= N + 1);
	return rc;
}
 
int get_int(int *a) 
{
	int n;
	do 
    {
		n = scanf("%d", a, sizeof(int));
		if (n < 0)			/*˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜ ˜˜˜˜˜ */
		{	
            return 0;
        }
        if (n == 0) 
        {			/*˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜ - ˜˜˜˜˜˜ */
			printf("%s\n", "˜˜˜˜˜˜˜ ˜˜˜˜˜: \n");
			clean_stdin();
		}
	} 
    while (n == 0);
	clean_stdin();
	return 1;
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

void clean_stdin() 
{
   while(fgetc(stdin) != '\n');
} 
///////////////////////////////////////////////////////


////////////////// ˜˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜ /////////////////
void d_add(Node* root) // ˜˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜ ˜ ˜˜˜˜˜˜
{
	char* key;
	char* info;
	printf("˜˜˜˜˜˜˜ ˜˜˜˜: ");
	key = get_str();
	printf("˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜˜˜: ");
	info = get_str();
	if (add(root, key, info) == 2)
    {
		printf("%s \n", errmsgs[1]);
    }
    printf("%s \n", errmsgs[0]);
    return;
}
 
void d_r_from_file(Node* root) // ˜˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜˜˜ ˜˜ ˜˜˜˜˜
{
	FILE* file;
	char* filename;
	char* key = NULL, * info;
	char c = NULL;
	printf("Enter filename: ");
	filename = get_str();
	file = fopen(filename, "r");
	free(filename);
	if (!file)
	{
		printf("˜˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜ ˜˜˜˜ \n");
		return;
	}
	do
	{
		key = r_from_file(file);
		info = r_from_file(file);
		if (!key)
		{
			free(key);
			break;
		}
		else
		{
			add(root, key, info);
		}
 
	}
    while (1);
	fclose(file);
}

void d_find_by_key(Node* root) // ˜˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜ ˜˜˜˜˜˜ ˜˜ ˜˜˜˜˜
{
	char* key;
	printf("˜˜˜˜˜˜˜ ˜˜˜˜: ");
	key = get_str();
	Node* cur = find_by_key(root, key);
	if (!cur)
    {
		printf("%s \n", errmsgs[2]);
    }
    else
	{
		printf("Key: %s \t Info: %s \n", cur->key, cur->info);
	}
	free(key);
}

void d_find_by_key_N(Node* root) // ˜˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜ ˜˜˜˜˜˜ ˜˜ N ˜˜˜˜˜˜˜˜
{
	int k = 0, n = 0;
	char* key;
	printf("˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜ ˜˜˜ ˜˜˜˜˜˜: ");
	k = get_int(&n);
	printf("˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜: ");
	key = get_str();
	if (strlen(key) != n)
	{
		printf("˜˜˜˜ ˜˜ ˜˜˜˜˜˜˜˜˜ ˜˜ ˜˜˜˜˜ \n");
		free(key);
		return;
	}
	find_by_key_N(root, key, n);
	free(key);
}
  
void d_delete(Node* root) // ˜˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜
{
	char* key;
	printf("˜˜˜˜˜˜˜ ˜˜˜˜ ˜˜˜ ˜˜˜˜˜˜˜˜: ");
	key = get_str();
	delete(root, key);
	free(key);
}
 
void d_show(Node* root) // ˜˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜ ˜˜˜˜˜˜ ˜ ˜˜˜˜˜˜˜ ˜˜˜˜˜˜
{
	char* key;
	printf("˜˜˜˜˜˜˜ ˜˜˜˜: ");
	key = get_str();
	print_for_key(root, key);
	free(key);
}
 
int d_timingfindbkey() // ˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜˜˜˜˜ 
{
	printf("\n Timing of searching by key procedure \n");
	Node* root = NULL;
	int n = 10, cnt = 100000, i, m;
	clock_t first, last;
	srand(time(NULL));
	char* k;
	char* info = '\0';
	char* key[10000];
		int mid[10];
		int dtime = 0;
		int ftime = 0;
	while (n-- > 0)
	{
		dtime = 0;
		for(int g = 0; g < 10; ++g)
		{
			for (i = 0; i < 10000; ++i) // ˜˜˜˜˜˜ ˜˜˜˜˜˜ ˜˜˜˜˜˜ 
			{
				int len = rand() % 20 + 1;
				key[i] = (char*)malloc((len + 1) * sizeof(char));
				for (int j = 0; j < len; ++j)
				{
					key[i][j] = rand() % 56 + 65;
				}
				key[i][len] = '\0';
			}
			for (i = 0; i < cnt;) // ˜˜˜˜˜˜ ˜˜˜˜ ˜ ˜˜˜˜˜˜˜˜˜ ˜˜˜ ˜ ˜˜˜˜˜˜˜˜˜˜ ˜ ˜˜˜˜˜˜
			{
				int len = rand() % 20 + 1;
				k = (char*)malloc((len + 1) * sizeof(char));
				for (int j = 0; j < len; ++j)
				{
					k[j] = rand() % 56 + 65;
				}
				k[len] = '\0';
				int count = 0;
				count = add(&root, k, info);
				if (!count)
					++i;
				if (count == 2) // ˜˜˜˜ ˜˜˜˜ ˜˜ ˜˜˜˜˜˜˜˜˜˜˜ ˜ ˜˜˜˜˜˜, ˜˜˜˜˜ ˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜
				{
					free(k);
					free(info);
				}
			}
			m = 0;
			first = clock();
			for (i = 0; i < 10000; ++i)
				if (find_by_key(&root, key[i])) // ˜˜˜˜˜ ˜˜˜˜˜˜˜˜˜
					++m;
			last = clock();
			mid[g] = last - first;
			dtime += mid[g];
		}
		ftime = dtime / 10;
		for (i = 0; i < 10000; ++i) // ˜˜˜˜˜˜˜ ˜˜˜˜˜˜ ˜˜˜˜˜˜˜ ˜˜˜˜˜˜
			free(key[i]);
		printf("%d items was found\n", m);
		printf("test #%d, number of nodes = %d, time = %d\n", 10 - n, (10 - n) * cnt, ftime);
	}
	delete_table(root);
	return 1;
}

int d_timingfindbnkey()
{
	printf("\n Timing of searching by N symbols of key procedure \n");
	Node* root = NULL;
	int n = 10, cnt = 100000, i, m;
	clock_t first, last;
	srand(time(NULL));
	char* k;
	char* info = '\0';
	char* key[10000];
	int mid[10];
	int dtime = 0;
	int ftime = 0;
	while (n-- > 0)
	{
		dtime = 0;
		for(int g = 0; g < 10; ++g)
		{
			for (i = 0; i < 10000; ++i) // ˜˜˜˜˜˜ ˜˜˜˜˜˜ ˜˜˜˜˜˜ 
			{
				int len = rand() % 20 + 1;
				key[i] = (char*)malloc((len + 1) * sizeof(char));
				for (int j = 0; j < len; ++j)
				{
					key[i][j] = rand() % 56 + 65;
				}
				key[i][len] = '\0';
			}
			for (i = 0; i < cnt;) // ˜˜˜˜˜˜ ˜˜˜˜ ˜ ˜˜˜˜˜˜˜˜˜ ˜˜˜ ˜ ˜˜˜˜˜˜˜˜˜˜ ˜ ˜˜˜˜˜˜
			{
				int len = rand() % 20 + 1;
				k = (char*)malloc((len + 1) * sizeof(char));
				for (int j = 0; j < len; ++j)
				{
					k[j] = rand() % 56 + 65;
				}
				k[len] = '\0';
				int count = 0;
				count = add(&root, k, info);
				if (!count)
					++i;
				if (count == 2) // ˜˜˜˜ ˜˜˜˜ ˜˜ ˜˜˜˜˜˜˜˜˜˜˜ ˜ ˜˜˜˜˜˜, ˜˜˜˜˜ ˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜
				{
					free(k);
					free(info);
				}
			}
			m = 0;
			first = clock();
			for (i = 0; i < 10000; ++i)
				if (find_by_key_N(&root, key[i], strlen(key[i]) - rand() % 2)) // ˜˜˜˜˜ ˜˜˜˜˜˜˜˜˜
					++m;
			last = clock();
			mid[g] = last - first;
			dtime += mid[g];
		}
		ftime = dtime / 10;
		for (i = 0; i < 10000; ++i) // ˜˜˜˜˜˜˜ ˜˜˜˜˜˜ ˜˜˜˜˜˜˜ ˜˜˜˜˜˜
			free(key[i]);
		printf("%d items was found\n", m);
		printf("test #%d, number of nodes = %d, time = %d\n", 10 - n, (10 - n) * cnt, ftime);
	}
	delete_table(root);
	return 1;
}

void d_show_tree(Node* root, int p, int s) // ˜˜˜˜˜˜˜ ˜˜˜˜˜˜ ˜˜˜˜˜˜ ˜ ˜˜˜˜ ˜˜˜˜˜˜
{
	int i;
	if (root == NULL)
		return;
	printf("%s %s\n", root->key, root->info);
	if (root->left)
	{
		showLine("|\n", p, s);
		showLine("L: ", p, s);
		d_show_tree(root->left, p + 1, s + ((root->right == NULL ? 0 : 1) << p));
	}
	if (root->right)
	{
		showLine("|\n", p, s);
		showLine("R: ", p, s);
		d_show_tree(root->right, p + 1, s);
	}
}
///////////////////////////////////////////////////////


///////////////////// ˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜ ///////////////
Node* addNode(char* key, char* info, Node* parent) // ˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜
{
	Node* tmp = (Node*)malloc(sizeof(Node));
	tmp->left = tmp->right = NULL;
	tmp->info = info;
	tmp->key = key;
	tmp->par = parent;
	return tmp;
}

Node* find_by_key(Node** root, char* key) // ˜˜˜˜˜˜˜ ˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜ ˜˜ ˜˜˜˜˜
{
	int k;
	Node* tmp;
	tmp = *root;
	while (tmp)
	{
		if ((k = strcmp(tmp->key, key)) < 0)
		{
			tmp = tmp->right;
			continue;
		}
		else if ((k = strcmp(tmp->key, key)) > 0)
		{
			tmp = tmp->left;
			continue;
		}
		else if ((k = strcmp(tmp->key, key)) == 0)
        {
			return tmp;
        }
    }
	return NULL;
}

Node* minNode(Node* root) // ˜˜˜˜˜˜˜ ˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜ ˜ ˜˜˜˜˜˜˜˜˜
{
	while (root->left)
    {
		root = root->left;
    }
    return root;
}

Node* maxNode(Node* root) // ˜˜˜˜˜˜˜ ˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜ ˜ ˜˜˜˜˜˜˜˜˜
{
	while (root->right)
	{	
        root = root->right;
    }
    return root;
}
 
Node* nextElem(Node** root) // ˜˜˜˜˜˜˜ ˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜
{
	Node* cur = root, * ppar = NULL;
	if (cur->right)
	{	
        return minNode(cur->right);
    }
    ppar = cur->par;
	while (ppar && (cur == ppar->right))
	{
		cur = ppar;
		ppar = ppar->par;
	}
	return ppar;
}

Node* print_for_key(Node** root, char* key) // ˜˜˜˜˜˜˜ ˜˜˜˜˜˜ ˜ ˜˜˜˜˜˜˜ ˜˜˜˜˜˜ 
{
	Node* cur = NULL;
	cur = minNode(*root);
	if (strlen(key) == 0)
	{
		cur = minNode(*root);
		//cur = nextElem(cur);
	}
	else
    {
		cur = find_by_key(root, key);
    }
    while (cur && (*root != maxNode(cur)))
	{
		printf("Key: %s \t Info: %s\n", cur->key, cur->info);
		cur = nextElem(cur);
	}
	return;
}
///////////////////////////////////////////////////////


/////////////////// ˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜ ////////////////
int add(Node** root, char* key, char* info) // ˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜ ˜ ˜˜˜˜˜˜
{
	int k = 0;
	Node* tmp = NULL;
    Node* ins = NULL;
	if (*root == NULL) // ˜˜˜˜ ˜˜˜˜˜˜ ˜˜˜˜˜˜, ˜˜˜˜˜˜ ˜˜˜˜˜˜
	{
		*root = addNode(key, info, NULL);
		return 0;
	}
	else
	{
		tmp = *root;
		while (tmp)
		{
			if ((k = strcmp(tmp->key, key)) < 0) // ˜˜˜˜ ˜˜˜˜ ˜˜˜˜˜˜ ˜˜˜˜˜ ˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜ 
			{
				if (tmp->right)
				{
					tmp = tmp->right;
					continue;
				}
				else
				{
					tmp->right = addNode(key, info, tmp);
					return 0;
				}
			}
			else if ((k = strcmp(tmp->key, key)) > 0) // ˜˜˜˜ ˜˜˜˜ ˜˜˜˜˜˜ ˜˜˜˜˜ ˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜
			{
				if (tmp->left)
				{
					tmp = tmp->left;
					continue;
				}
				else
				{
					tmp->left = addNode(key, info, tmp);
					return 0;
				}
			}
			else
			{
				return 2; // ˜˜˜˜ ˜˜˜˜ ˜˜˜˜˜˜˜ ˜˜˜˜˜ ˜˜ ˜˜˜˜
			}
		}
	}
}

char* r_from_file(FILE* file)
{
	char* ptr = (char*)malloc(1);  //˜˜˜˜˜˜
	char buf[100]; //˜˜˜˜˜ ˜˜˜ ˜˜˜˜˜˜˜˜˜ ˜˜ ˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜
	int n = 0, len = 0; //len - ˜˜˜˜˜ ˜˜˜˜˜˜
	*ptr = '\0';
	do 
    {
		n = fscanf(file, "%99[^\n]", buf);
		if (n < 0) 
        {
			free(ptr);
			ptr = NULL;
			continue;
		}
		if (n == 0)
        {
			fscanf(file, "%*c");
        }
        else 
        {
			len += strlen(buf);
			ptr = (char*)realloc(ptr, len + 1); // ˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜
			strcat(ptr, buf);
		}
	} 
    while (n > 0);
	return ptr;
}
 
int findd = 0;
 
void find_by_key_N(Node** root, char* key, int n) // ˜˜˜˜˜˜˜ ˜˜˜˜˜˜ ˜˜ N ˜˜˜˜˜˜˜˜
{
	Node* tmp;
	int k = 0;
	tmp = *root;
	if (tmp)
	{
		char* erease_key = calloc(n, (n + 1) * sizeof(char));
		strncat(erease_key, tmp->key, n);
		if ((k = strcmp(erease_key, key)) == 0)
		{
			printf("Key: %s \t Info: %s\n", tmp->key, tmp->info);
			findd++;
		}
		find_by_key_N(&tmp->left, key, n);
		find_by_key_N(&tmp->right, key, n);
		free(erease_key);
	}
	return 1;
}
 
int t_find_by_key_N(Node** root, char* key, int n) // ˜˜˜˜˜˜˜ ˜˜˜˜˜˜ ˜˜ N ˜˜˜˜˜˜˜˜
{
	Node* tmp;
	int k = 0;
	tmp = *root;
	if (tmp)
	{
		char* erease_key = calloc(n, (n + 1) * sizeof(char));
		strncat(erease_key, tmp->key, n);
		if ((k = strcmp(erease_key, key)) == 0)
		{
			findd++;
			return 1;
		}
		t_find_by_key_N(&tmp->left, key, n);
		t_find_by_key_N(&tmp->right, key, n);
		free(erease_key);
	}
	return 0;
}

void delete(Node** root, char* key) // ˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜
{
	Node* cur = NULL, * pred = NULL;
	cur = find_by_key(root, key); // ˜˜˜˜˜ ˜˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜
	if (!cur->par && !cur->right && !cur->left)
	{
		free(cur->info);
		free(cur->key);
		free(cur);
		cur = NULL;
		*root = NULL;
		return;
	}
	if (!cur)
    {
    	return;
    }
    if (!cur->par) // ˜˜˜˜ ˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜ 
	{
		if (cur->right) // ˜˜˜˜ ˜˜˜˜ ˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜˜
		{
			pred = minNode(cur->right);
			Node* tmp = pred->par;
			free(cur->info);
			free(cur->key);
			cur->info = pred->info;
			cur->key = pred->key;
			tmp->left = pred->right;
			if (pred->right)
            {
				pred->right->par = tmp;
            }
            free(pred);
		}
		else // ˜˜˜˜ ˜˜˜˜ ˜˜˜˜˜˜ ˜˜˜˜˜
		{
			pred = maxNode(cur->left);
			Node* tmp = pred->par;
			free(cur->info);
			free(cur->key);
			cur->info = pred->info;
			cur->key = pred->key;
			tmp->right = pred->left;
			if (pred->left)
			{
            	pred->left->par = tmp;
            }
            if (!cur->left->left && !cur->left->right)
			{	
                cur->left = NULL;
            }
            free(pred);
		}
	}
	else // ˜˜˜˜ ˜˜˜˜˜˜˜˜˜ ˜˜ ˜˜˜˜˜˜
	{
		if (!cur->left && !cur->right) // ˜˜˜˜ ˜ ˜˜˜˜˜˜˜˜ ˜˜˜ ˜˜˜˜˜˜˜˜
		{
			pred = cur->par;
			if (cur == pred->right)
			{
            	pred->right = NULL;
            }
            else
            {
				pred->left = NULL;
            }
            free(cur->key);
			free(cur->info);
			free(cur);
		}
		else if (!cur->left && cur->right) // ˜˜˜˜ ˜ ˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜ ˜˜˜˜˜˜
		{
			pred = cur->par;
			if (cur == pred->right)
			{
				pred->right = cur->right;
				pred->right->par = cur->par;
			}
			else
			{
				pred->left = cur->right;
				pred->left->par = cur->par;
			}
			free(cur->info);
			free(cur->key);
			free(cur);
		}
		else if (cur->left && !(cur->right)) // ˜˜˜˜ ˜ ˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜ ˜˜˜˜˜
		{
			pred = cur->par;
			if (cur == pred->right)
			{
				pred->right = cur->left;
				pred->right->par = cur->par;
			}
			else
			{
				pred->left = cur->left;
				pred->left->par = cur->par;
			}
			free(cur->info);
			free(cur->key);
			free(cur);
		}
		else if (cur->left && cur->right) // ˜˜˜˜ ˜ ˜˜˜˜˜˜˜˜ ˜˜˜ ˜˜˜˜˜˜˜
		{
			if (!cur->right->left)
			{
				free(cur->info);
				free(cur->key);
				cur->info = cur->right->info;
				cur->key = cur->right->key;
				if (cur->right->right)
				{
					pred = cur->right->right;
					pred->par = cur;
				}
				free(cur->right);
				cur->right = pred;
			}
			else
			{
				pred = minNode(cur->right);
				free(cur->info);
				free(cur->key);
				cur->info = pred->info;
				cur->key = pred->key;
				Node* tmp = pred->par;
				free(pred);
				tmp->left = NULL;
			}
		}
	}
	return;
}

void delete_table(Node* root) // ˜˜˜˜˜˜˜ ˜˜˜˜˜ ˜˜˜˜˜˜
{
	if (root)
	{
		delete_table(root->left);
		delete_table(root->right);
		free(root->key);
		free(root->info);
		free(root);
	}
}
 
void print(Node* root) // ˜˜˜˜˜˜˜ ˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜
{
	if (root != NULL)
	{
		print(root->left);
		printf("%10d %10d", root->info, root->key);
		print(root->right);
	}
}
 
void showLine(char* c, int p, int s) // ˜˜˜˜˜˜˜ ˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜ 
{
	int t = s, i; 
	for (i = 0; i < p; i++) 
	{ 
		printf(t & 1 ? "|  " : "   "); t /= 2; 
	} 
	printf(c);
}
/////////////////////////////////////////////////////// 


int main(void)
{
	Node* root = NULL;
    setlocale(LC_ALL,"Russian"); 
	int rc, p = 0, s = 0;
	while (rc = dialog(msgs, NMsgs)) // ˜˜˜˜˜ ˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜˜ ˜˜˜˜
	{
		if (rc == 1)            // ˜˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜
        { 
			d_add(&root);
		}
        else if (rc == 2)            // ˜˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜ ˜˜ ˜˜˜˜˜
        {
			d_r_from_file(&root); 
		}
        else if (rc == 3)            // ˜˜˜˜˜ ˜˜ ˜˜˜˜˜
        {
			if(root)
			{	
				d_find_by_key(&root); 
			}
			else
			{
				printf("%s \n", errmsgs[4]);
			}	
		}
        else if (rc == 4)            // ˜˜˜˜˜ ˜˜ ˜˜˜˜˜˜ N ˜˜˜˜˜˜˜˜˜
		{	
			if(root)
			{
				d_find_by_key_N(&root); 
				if (!findd)
				{
					printf("%s \n",errmsgs[3]);
				}
			}
			else
			{
				printf("%s \n", errmsgs[4]);
			}
        }
		else if (rc == 5)			// ˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜ ˜˜ ˜˜˜˜˜˜
        {	
			if(root)
			{	
				d_delete(&root);			
			}
			else
			{
				printf("%s \n", errmsgs[4]);
			}          
		}
        else if (rc == 7)			// ˜˜˜˜˜ ˜˜˜˜˜˜˜ ˜ ˜˜˜˜˜˜˜ ˜˜˜˜˜
        {
			if(root)
			{	
				d_show(&root); 
			}
			else
			{
				printf("%s \n", errmsgs[4]);
			}        
		}
        else if (rc == 6)
        {
			d_timingfindbkey(); 
			d_timingfindbnkey();
		}
        else if (rc == 8) 
		{
			printf("\nTree: \n");
			d_show_tree(root, p, s);            // ˜˜˜˜˜ ˜˜˜˜˜˜ ˜ ˜˜˜˜ ˜˜˜˜˜˜
		}
        else if ( rc == 9)
        {
            printf("------------------------------------------------------------- \n");
            break;
        }
        printf("------------------------------------------------------------- \n");
	}
	delete_table(root); // ˜˜˜˜˜˜˜ ˜˜˜˜˜˜
	printf("˜˜ ˜˜˜˜˜ \n");
	return 0;
}
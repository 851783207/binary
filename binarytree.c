#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include <time.h>

#define SIZE 100//max word size
typedef struct node{
	int count;
	char* word;
	struct node *left;//leftchild
	struct node *right;//right child
}BTNode, *BTREE;
BTREE ROOT=NULL;

int count =0;
static int total_words=0;
static int different=0;

void get_word(char *temp_word, FILE *fp);//get word from the file
void FREE(BTREE root);//free memory
BTREE insert(BTREE T, char* temp_word);//insert words into BST
BTREE SEARCH(BTREE root);//find most common word

int main(void)
{
	char temp_word[SIZE] = {0};
   	FILE *fp;
    int i;
    clock_t start,end;
    double totaltime;
    BTREE root = NULL;

	if( (fp = fopen("Bulk.txt", "r")) == NULL )//Try opening the destination file
    {
         printf("Can not open the file!\n");
	}
    else
    {
         start = clock();
	while( !feof(fp) )
	{
		memset(temp_word,0,SIZE);//Initializes the string
		get_word(temp_word,fp);//Get the words one by one
		if(strlen(temp_word)>0)
        {
		   root = insert(root, temp_word);//If there exist a word, insert the word into the BST
		}

	}
        printf("Total number of words = %d\n",total_words);
        printf("Number of different words %d\nThe 100 most common words:\n",different);
        printf("WORD\t  NUMBER OF OCCURRENCES\n");

    for(i=0;i<100;i++){//print the 100 most common words
            root=SEARCH(root);
            printf("%-16s%d\n",ROOT->word,ROOT->count);
            ROOT->count=0;// change word's appear time to 0 and find the ith most frequently occurring words
            count=0;
            ROOT=NULL;
      }
      end = clock();
      totaltime = (double)(end-start)/CLOCKS_PER_SEC;//count consumed time(have not be freed)
      printf("Consumed time: %.2f seconds \n",totaltime);
       FREE(root);
    }
	return 0;
}

void get_word(char *temp_word, FILE *fp)
{
	int i = 0;
    char temp = '0';
    temp = fgetc(fp);
	while( !isalpha(temp)&&temp!='\'' ){
		temp = fgetc(fp);//while the first word is not letter, find the next one
		if(temp==EOF){
            return;
		}
	}

	while(isalpha(temp) || temp=='\'' ){
	    if(isalpha(temp)){
            temp = toupper(temp);
	    }
		temp_word[i] = temp;
		i++;
		temp = fgetc(fp);
	}//get a whole word
    total_words++;//total word +1
}


BTREE insert(BTREE p, char* temp_word)
{
	int m;
	if(p == NULL)//if the leaf node (root) has not been created yet, create one
	{
		p = (BTREE)malloc(sizeof(BTNode));//Allocate space
		//strcpy(p->word,temp_word);//fail to run file "Bulk", maybe because of buffer overflow
		p->word=strdup(temp_word);//import word into BST->word by pointer
		p->count = 1;
		p->right = p->left = NULL;
		different++;
	}

	else if( (m  = strcmp(temp_word, p->word)) == 0){//if word is the same, count +1;
		p->count++;
	}
	else if( m < 0 ){// if the insert word < leaf node, try inserting it into the left node
		p->left = insert(p->left, temp_word);
	}
	else{//if >, insert into right node
		p->right = insert(p->right, temp_word);
	}
	return p;//return root address
}

BTREE SEARCH(BTREE root){
        if(root!=NULL) {
                if(root->count>count){
                    count=root->count;
                    ROOT=root;//Get the address of whose "count" value is bigger (the biggest)
                }
        SEARCH(root->left);//Postorder traversal
        SEARCH(root->right);
        //left->right->root
    }
    return root;

}

void FREE(BTREE root){
    if(root!=NULL){
        FREE(root->left);
        free(root->word);
        free(root);
        FREE(root->right);
    }
}

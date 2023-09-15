/*
 *  Minos Stavrakakis - csd4120
 *  
 */


/*************************************************************
 * @file   main.c                                            *
 * @author Papaioannou Antonis 						  	     *
 *                                                    	     *
 * @brief  Source file for the needs of CS-240b project 2018 *
 ************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "Shopping.h"

#define BUFFER_SIZE 1024  /* Maximum length of a line in input file */

/* Uncomment the following line to enable debugging prints
 * or comment to disable it */
#define DEBUG

#ifdef DEBUG
#define DPRINT(...) fprintf(stderr, __VA_ARGS__);
#else  /* DEBUG */
#define DPRINT(...)
#endif /* DEBUG */


int PrintCustomers (void);
int PrintShops (void);
void inOrderCProductsPrint(struct C_Product *root);

/*~~~~~Globals~~~~~*/
int m; /*used as size of hash table*/
int pconst1,pconst2;
/*~~~~~~~~~~~~~~~~~*/

/**
 * @brief Finds and returns the lower prime value that
 *        surpasses customer given number
 *
 * @return prime value on success
 *         -1 on failure
 */
int findPrimeSize(void)
{
    int i;
    for(i=0;i<368;i++){
        if(((short)primes_g[i]) > max_customers_g)
            break;
    }
    return (short)primes_g[i];
}


/**
 * @brief Optional function to initialize data structures that
 *        need initialization
 *
 * @return 1 on success
 *         0 on failure
 */
int Initialize (void)
{
    srand(getpid());

    /*Prime Constants initialization*/
    pconst1 = primes_g[rand()%368];
    pconst2 = primes_g[rand()%368];

    /*Hash table initialization*/
    customers_hash=NULL;

	/*Shop Tree initialization*/
	shopSentinel = (struct Shop*)malloc(sizeof(struct Shop));
	if(!shopSentinel)
        return 0;
    shopSentinel->sid = -1;
    shopSentinel->type = -1;
    shopSentinel->products = NULL;
    shopSentinel->lc = NULL;
    shopSentinel->rc = NULL;
	shopRoot = shopSentinel;




	return 1;
}

/**
 * @brief InOrder traversal
 *
 */
void inOrderSoloShop(struct Shop *root)
{
    if(root==shopSentinel)
        return;

    inOrderSoloShop(root->lc);
    printf("<%d,%d> ", root->sid, root->type);
    inOrderSoloShop(root->rc);

    return;
}

/**
 * @brief Search Shop
 *
 * @param sid       The shop's id
 *
 * @return struct Shop* on success
 *         NULL 		on failure
 */
struct Shop* SearchShop(int sid)
{
	struct Shop *tempS = shopRoot;
	/*Using Sentinel to terminate search*/
	shopSentinel->sid = sid;

	while(tempS->sid!=sid){
        if(tempS->sid < sid){
            tempS = tempS->rc;
        } else {
            tempS = tempS->lc;
        }
	}

	if(tempS==shopSentinel){
		/*Return def value to Header->sid*/
		shopSentinel->sid = -1;
		return NULL;
	}
	else{
        /*Return def value to Header->sid*/
        shopSentinel->sid = -1;
		return tempS;
	}
}

/**
 * @brief Search Product
 *
 * @param sid       The Shop's id
 * @param pid       The Products's id
 *
 * @return struct Product* on success
 *         NULL 		   on failure
 */
struct Product* SearchProduct(int sid, int pid)
{
	struct Shop *tempS = shopRoot;
	struct Product *tempP;

	tempS = SearchShop(sid);
	if(!tempS)
		return NULL;

	tempP = tempS->products;
	while((tempP != NULL)&&(tempP->pid != pid))
		tempP = tempP->next;

	if(tempP)
		return tempP;
	else
		return NULL;
}

/**
 * @brief Register shop
 *
 * @param sid   The shop's id
 * @param type  The shop's type
 *
 * @return 1 on success
 *         0 on failure
 */
int RegisterShop (int sid, int type)
{
    struct Shop *temp, *p, *prev;
    p = shopRoot;

    temp = (struct Shop*)malloc(sizeof(struct Shop));
    /*return 0 in case of failed malloc*/
    if(temp==NULL)
        return 0;
    temp->sid = sid;
    temp->type = type;
    temp->products = NULL;
    temp->lc = shopSentinel;
    temp->rc = shopSentinel;

    /*temp be the root of the tree*/
    if(p==shopSentinel){
        shopRoot = temp;
        printf("\tShops = ");
        inOrderSoloShop(shopRoot);
        printf("\nDONE\n");
        return 1;
    }

    while(p!=shopSentinel){
        if(p->sid > sid){
            prev = p;
            p = p->lc;
        } else if (p->sid < sid){
            prev = p;
            p = p->rc;
         } else {
            /*else means sid already exist*/
            free(temp);
            return 0;
         }
    }

    if(prev->sid < sid){
        prev->rc = temp;
    } else if (prev->sid > sid){
        prev->lc = temp;
    } else{
        free(temp);
        return 0;
    }

    printf("\tShops = ");
    inOrderSoloShop(shopRoot);
	printf("\nDONE\n");
	return  1;
}

/**
 * @brief Supply shop
 *
 * @param sid       The shop's id
 * @param pid       The product's id
 * @param quantity  The product's quantity
 * @param price     The product's price
 *
 * @return 1 on success
 *         0 on failure
 */
int SupplyShop (int sid, int pid, int quantity, int price)
{
	struct Shop *tempS;
	struct Product *tempP;
	struct Product *prevP;
	struct Product *newPro;

	tempP = SearchProduct(sid, pid);
	if(tempP){
		tempP->qty += quantity;
		tempP->price = price;
		PrintShops();
		return 1;
	}

	tempS = SearchShop(sid);
	if(!tempS)
		return 0;

	newPro = (struct Product*) malloc(sizeof(struct Product));
	if(!newPro)
		return 0;

	newPro->pid = pid;
	newPro->qty = quantity;
	newPro->price = price;

	if(tempS->products==NULL){
		tempS->products = newPro;
		newPro->next = NULL;
	} else {
		tempP = tempS->products;
		while((tempP != NULL)&&(tempP->pid < pid)){
			prevP = tempP;
			tempP = tempP->next;
		}
		if(tempP==tempS->products){
            newPro->next = tempP;
            tempS->products = newPro;
		}else {
            newPro->next = tempP;
            prevP->next = newPro;}
	}
	PrintShops();
	return 1;
}

/*******************************
 *       Hash Functions        *
 *******************************/

 /**
 * @brief Calculate elements 1st index on hash table
 *
 * @param cid The customer's id
 *
 * @return int index on success
 *         -1 on failure
 */
int H1(int key)
{
    return key%m;
}

 /**
 * @brief Calculate pace in case of collision
 *
 * @param key, i
 *
 * @return int index on success
 *         -1 on failure
 */
int H2(int key,int i)
{
    int step;
    step = ((H1(key)+pconst1)+i)%m;

    return step;

}

/********************************/


/**
 * @brief Find customer
 *
 * @param cid The customer's id
 *
 * @return Customer* on success
 *         NULL on failure
 */
struct Customer * findCustomer(int cid)
{
    int index,i=1;
    index = H1(cid);
    while((customers_hash[index]!=NULL) &&
          (customers_hash[index]->cid!=cid))
            index = H2(cid,i++);

    if(customers_hash[index]==NULL)
        return NULL;
    else
        return customers_hash[index];
}

/**
 * @brief Register customer
 *
 * @param cid The customer's id
 *
 * @return 1 on success
 *         0 on failure
 */
int RegisterCustomer (int cid)
{
	int index,i=1;
	struct Customer *tempC;

    /*Hash table initialization*/
    if(customers_hash==NULL){
        m = (int)findPrimeSize();
        customers_hash = (struct Customer**)malloc(sizeof(struct Customer*)*m);
        for(i=0;i<m;i++)
            customers_hash[i] = NULL;
    }

	index = H1(cid);

	while((customers_hash[index]!=NULL) &&
          (customers_hash[index]->cid!=cid)&&
          (customers_hash[index]->cid!= -1)){
        index = H2(cid,i++);
	}
    /*Case customer already exists*/
    if((customers_hash[index]!=NULL)&&
       (customers_hash[index]->cid==cid)){

        return 0;
    } else {

        tempC = (struct Customer*)malloc(sizeof(struct Customer));
        if(tempC==NULL)
            return 0;
        tempC->cid = cid;
        tempC->shopping_size = 0;
        tempC->shopping_tree = NULL;
        /*add new customer to hashtable*/
        customers_hash[index] = tempC;
    }

    printf("\tCustomers = ");
    for(i=0;i<m;i++){
        if(customers_hash[i]!=NULL)
            printf("<%d> ", customers_hash[i]->cid);
    }
	printf("\nDONE\n");
	return 1;
}

/**
 * @brief Populates C_Product tree
 *
 * @param root      C_Product tree root
 * @param pid       The product's id
 * @param quantity  The quantity of the product
 *
 * @return struct C_Product* on success
 *         NULL              on failure
 */
struct C_Product * insertCProduct(struct C_Product *root,
                                  int pid, int qty)
{
    struct C_Product *tempCP;

    if(root==NULL){
        tempCP = (struct C_Product*)malloc(sizeof(
                                    struct C_Product));
        if(tempCP==NULL)
            return NULL;
        tempCP->pid = pid;
        tempCP->qty = qty;
        tempCP->lc = NULL;
        tempCP->rc = NULL;

        return tempCP;
    }

    if(pid > (root->pid)){
        root->rc = insertCProduct(root->rc, pid, qty);
    }else if(pid < root->pid){
        root->lc = insertCProduct(root->lc, pid, qty);
    }else{ /*(root->pid == pid) case*/
        root->qty += qty;
    }

    return root;
}

/**
 * @brief Add to shopping list
 *
 * @param cid       The customer's id
 * @param pid       The product's id
 * @param quantity  The quantity of the product
 *
 * @return 1 on success
 *         0 on failure
 */
int AddToShoppingList (int cid, int pid, int quantity)
{
    struct Customer *tempC;
    struct C_Product *tempCP;

    if(!(tempC=findCustomer(cid)))
        return 0;

    tempCP = tempC->shopping_tree =
            insertCProduct(tempC->shopping_tree, pid, quantity);

    if(!tempCP)
        return 0;

    tempC->shopping_size++;
    PrintCustomers();
	return 1;
}

/**
 * @brief Find minimum elmnt in C_Product tree
 *
 * @param root      C_Product tree root
 *
 * @return struct C_Product* on success
 *         NULL              on failure
 */
struct C_Product * findMinCP(struct C_Product *root)
{
    if(root==NULL)
        return NULL;

    if(root->lc)
        return findMinCP(root->lc);
    else
        return root;
}

/**
 * @brief Returns elmnt from C_Product tree
 *
 * @param root      C_Product tree root
 * @param pid       The product's id
 *
 * @return struct C_Product* on success
 *         NULL              on failure
 */
struct C_Product * searchCProduct(struct C_Product *root, int pid)
{
	if(root==NULL)
		return NULL;

	if(pid > root->pid)
		return searchCProduct(root->rc, pid);
	else if(pid < root->pid)
		return searchCProduct(root->lc, pid);
	else /* elmnt found!*/
		return root;
}

/**
 * @brief Deletes elmnt from C_Product tree
 *
 * @param root      C_Product tree root
 * @param pid       The product's id
 *
 * @return struct C_Product*
 */
struct C_Product * deleteCProduct(struct C_Product *root, int pid)
{
    struct C_Product *tempCP;

    if(root==NULL){
        return root;

    } else if(pid < root->pid) {
        root->lc = deleteCProduct(root->lc, pid);

    } else if(pid > root->pid) {
        root->rc = deleteCProduct(root->rc, pid);

    } else { /*case: pid been found*/
        /*case: node due to be deleted is internal with 2
          child nodes*/
        /*n*/
        if((root->rc) && (root->lc)){

            /*node is being replaced with it's previous in InOrder
              with the utilization of findMinCP()*/
            tempCP = findMinCP(root->rc);
            root->pid = tempCP->pid;
            root->qty = tempCP->qty;
            /*callin delete recursivley from node of interest->rc,
              function will locate pid and choose 1/0 children case*/
            root->rc = deleteCProduct(root->rc, tempCP->pid);

        /*case: node has one to zero children*/
        } else {
            tempCP = root;

            /*case: only rc or 0 children*/
            if(root->lc==NULL)
                root = root->rc;
            /*case: only lc*/
            else if(root->rc==NULL)
                root = root->lc;

            free(tempCP);
        }
    }

    return root;
}

/**
 * @brief Delete from shopping list
 *
 * @param cid  The customer's id
 * @param pid  The product's id
 *
 * @return 1 on success
 *         0 on failure
 */
int DeleteFromShoppingList (int cid, int pid)
{
	struct Customer *tempC;
	struct C_Product *tempCP;

	tempC = findCustomer(cid);
	if(tempC==NULL)
        return 0;

    tempCP = tempC->shopping_tree;
    tempC->shopping_tree = deleteCProduct(tempCP, pid);

    tempC->shopping_size--;
    PrintCustomers();
	return 1;
}

/**
 * @brief Delete from shopping list
 *
 * @param cid  The customer's id
 * @param pid  The product's id
 *
 * @return 1 on success
 *         0 on failure
 */
int DeleteFromShoppingListNP (int cid, int pid)
{
	struct Customer *tempC;
	struct C_Product *tempCP;

	tempC = findCustomer(cid);
	if(tempC==NULL)
        return 0;

    tempCP = tempC->shopping_tree;
    tempC->shopping_tree = deleteCProduct(tempCP, pid);

    tempC->shopping_size--;
	return 1;
}


/**
 * @brief InOrder traversal of Customer
 *        shopping tree
 *
 * @param cid The customer's id
 *
 * @return struct C_Product* on success
 *         NULLL on failure
 */
void inOrderCProductsCopy(struct C_Product *root,
                      struct C_Product *CParray, int* index)
{
    if(!root)
        return;

    inOrderCProductsCopy(root->lc, CParray, index);
    {
        CParray[*index].lc = NULL;
        CParray[*index].rc = NULL;
        CParray[*index].pid = root->pid;
        CParray[*index].qty = root->qty;
        (*index)++;
    }
    inOrderCProductsCopy(root->rc, CParray, index);
    return;
}

/**
 * @brief Copies Customer's shopping tree
 *        to an array
 *
 * @param cid The customer's id
 *
 * @return struct C_Product* on success
 *         NULLL on failure
 */
struct C_Product * CPArrayzation(struct Customer *cus,
                                 struct C_Product *CParray )
{
    int size = cus->shopping_size;
    int *index, zero=0;
    CParray = (struct C_Product*)malloc(
                        sizeof(struct C_Product)*size);
    if(!CParray)
        return NULL;

    index = &zero;
    inOrderCProductsCopy(cus->shopping_tree,
                                   CParray, index);

    return CParray;
}

/**
 * @brief Delete from shop product list. NOT PRINT
 *
 * @param cid  The shops's id
 * @param pid  The product's id
 *
 * @return 1 on success
 *         0 on failure
 */
int DeleteFromShopProductListNP (int sid, int pid)
{
	struct Shop *tempS;
	struct Product *tempP;
	struct Product *prevP;

	if(!(tempS=SearchShop(sid)))
		return 0;

	tempP = tempS->products;
	while(tempP!=NULL){
		prevP = tempP;
		tempP = tempP->next;
		if(prevP->pid==pid){
			tempS->products = tempP;
			free(prevP);
			return 1;
		}
		if(tempP&&(tempP->pid==pid)){
			prevP->next = tempP->next;
			free(tempP);
			return 1;
		}
	}
	return 0;
}

/**
 * @brief Traverse Shop tree, traverse shop's
 *        product list and compare with shopping list
 *        array, if product on both: reduce qty and if
 *        reach zero delete from either
 *
 * @param root Shop tree
 * @param CParray shopping list
 * @param int CParray size
 *
 */
void inOrderGS (struct Shop *root, int cid,
               struct C_Product *CParray, int size)
{
    struct Product *tempP,*prevP;
    int index, SPdel, CPdel;

    if(root == shopSentinel)
        return;

    inOrderGS(root->lc, cid, CParray, size);

    {
    	tempP = root->products;
    	index = 0, SPdel=1,CPdel=1;
    	while((tempP!=NULL) && (index<size)){

            if(tempP->pid > CParray[index].pid){
                index++;

            } else if(tempP->pid < CParray[index].pid){
                tempP = tempP->next;

            } else if(tempP->pid == CParray[index].pid){
                /*decrease products (shoplist & customerlist
                until either gets depleted first*/
                while((tempP->qty)&&(CParray[index].qty)){
                    tempP->qty--;
                    CParray[index].qty--;
                }
                /* check if shop product stock depleted and
                if so delete from shop catalogue,
                store deletion status in var SPdel*/
                if(tempP->qty==0){
                    /*Progress pointer*/
                    prevP = tempP;
                    tempP = tempP->next;
                    SPdel = DeleteFromShopProductListNP(
                                root->sid, prevP->pid);
                }

                if(CParray[index].qty==0){
                    /*CPdel = DeleteFromShoppingListNP(cid,
                                    CParray[index].pid);*/
                    /*Progress index*/
                    index++;
                }
                /*if depleted product fails to delete exit w 0*/
                if(!(SPdel) || !(CPdel)){
                    printf("\n\nProduct %d, Deletion Failed!\n\n",
                           CParray[index].pid);
                    return;
                }

            }
    	}
    }

    inOrderGS(root->rc, cid, CParray, size);
}

/**
 * @brief Updates Customer Shopping tree with
 *        values of CParray after GoShopping
 *
 * @param root Shopping List tree
 * @param CParray shopping list
 * @param int CParray size
 *
 */
void updateCustomerShpList(struct Customer *cus,
                           struct C_Product *CParray, int size)
{
    int i;
    for(i=0;i<size;i++){

        if(CParray[i].qty==0)
            DeleteFromShoppingListNP(cus->cid, CParray[i].pid);
        else
            searchCProduct(cus->shopping_tree, CParray[i].pid)->qty = CParray[i].qty;
    }
    return;
}

/**
 * @brief Go shopping
 *
 * @param cid The customer's id
 *
 * @return 1 on success
 *         0 on failure
 */
int GoShopping (int cid)
{
	struct Customer *tempC;
	struct C_Product *CParray=NULL;
	int i, s;

	tempC = findCustomer(cid);
	if(tempC==NULL)
        return 0;

    CParray = CPArrayzation(tempC, CParray);

    s = tempC->shopping_size;

    printf("\nArray Contents:\n\n");
    for(i=0;i<s;i++){
        printf("\t<%d,%d>\n",CParray[i].pid, CParray[i].qty);
    }
    printf("\nDONE\n");

    inOrderGS (shopRoot, cid, CParray, s);

    printf("\tCustomer%d = ", tempC->cid);
    inOrderCProductsPrint(tempC->shopping_tree);
    printf("\n");

    PrintShops();
    printf("\n");

    printf("\nArray Contents:\n\n");
    for(i=0;i<s;i++){
        printf("\t<%d,%d>\n",CParray[i].pid, CParray[i].qty);
    }
    printf("\nDONE\n");

    updateCustomerShpList(tempC, CParray, s);
    printf("\tCustomer%d = ", tempC->cid);
    inOrderCProductsPrint(tempC->shopping_tree);
    printf("\n");

    PrintShops();
    printf("\n");

	return 1;
}

/*
 *  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 *  %          MERGE EVENT FUNCTIONS           %
 *  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 */

/**
 * @brief Merges 2 ordered CParrays to a new
 *        ordered CParray
 *
 * @param CParray1 shopping list
 * @param int CParray1 size1
 * @param CParray2 shopping list
 * @param int CParray2 size2
 * @param CParrayM result array
 *
 * @return int true size on success
 *         0             on failure
 */
int mergeArrays(struct C_Product *CParray1, int size1,
                struct C_Product *CParray2, int size2,
                struct C_Product *CParrayM)
{
    int i=0, j=0, index=0;

    while((i < size1)&&(j < size2)){

            if(CParray1[i].pid < CParray2[j].pid){
                CParrayM[index++] = CParray1[i++];

            } else if(CParray1[i].pid > CParray2[j].pid){
                CParrayM[index++] = CParray2[j++];

             /*case of same pid in both arrays*/
            } else {
                CParrayM[index] = CParray1[i++];
                CParrayM[index].qty += CParray2[j].qty;
                index++; j++;
            }
    }

    while(i < size1)
        CParrayM[index++] = CParray1[i++];

    while(j < size2)
        CParrayM[index++] = CParray2[j++];

    return index;
}

/**
 * @brief Unload CParray elmts to a C_Product BST
 *
 * @param CParray pointer to a C_Product array
 * @param head  index of head
 * @param tail  index of tail
 *
 * @return struct C_Product * on success
 *         NULL               on failure
 */
struct C_Product * array2Tree(struct C_Product *CParray, int head,
                                                          int tail)
{
    struct C_Product *newCP;
    int mid;

    if(head > tail)
        return NULL;

    mid = (head + tail) / 2;
    newCP = (struct C_Product*)malloc(sizeof(struct C_Product));
    newCP->pid = CParray[mid].pid;
    newCP->qty = CParray[mid].qty;

    newCP->lc = array2Tree(CParray, head, mid-1);
    newCP->rc = array2Tree(CParray, mid+1, tail);

    return newCP;
}

/**
 * @brief Delete entire C_Product BTS
 *
 * @param root pointer to root of the tree
 *
 */
void deleteCPtree(struct C_Product *root)
{
    if(root==NULL)
        return;

    deleteCPtree(root->lc);
    deleteCPtree(root->rc);
    free(root);

    return;
}

/**
 * @brief Customers shop together
 *
 * @param cid1  The id of the first customer
 * @param cid2  The id of the second customer
 *
 * @return 1 on success
 *         0 on failure
 */
int ShopTogether (int cid1, int cid2)
{
	struct Customer *tempC1, *tempC2;
	struct C_Product *CParr1=NULL, *CParr2=NULL, *CParrM=NULL;
	int size1, size2, sizeM;

	tempC1 = findCustomer(cid1);
	tempC2 = findCustomer(cid2);

	if((!tempC1)||(!tempC2))
        return 0;

    size1 = tempC1->shopping_size;
    size2 = tempC2->shopping_size;

    CParr1 = CPArrayzation(tempC1, CParr1);
    CParr2 = CPArrayzation(tempC2, CParr2);

    /*mem alloc for joined shopping lists array*/
    CParrM = (struct C_Product*)malloc(
                        sizeof(struct C_Product)*(size1+size2));

    if(!CParrM)
        return 0;

    /*populates CParrayM and returs it's size*/
    sizeM = mergeArrays(CParr1, size1, CParr2, size2, CParrM);

    /*memory oprtimization*/
    CParrM = (struct C_Product*)realloc(CParrM,
                                    sizeof(struct C_Product)*sizeM);

    /*delete cid1 customer's C_Product tree*/
    deleteCPtree(tempC1->shopping_tree);


    /*delete cid2 customer's C_Product tree*/
    deleteCPtree(tempC2->shopping_tree);

    /*"delete" customer cid2*/
    tempC2->shopping_size = 0;
    tempC2->cid = -1;
    tempC2->shopping_tree = NULL;

    /*create minimum height tree from CParrayM and assign
      to customer */
    tempC1->shopping_tree = array2Tree(CParrM, 0, sizeM-1);
    tempC1->shopping_size = sizeM;

    printf("\tCustomer%d = ", cid1);
    inOrderCProductsPrint(tempC1->shopping_tree);
    printf("\n\nDONE\n");

	return 1;
}

/*
 *  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 */

/*
 *  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 *  %%%%%%%% CATEGORIZE EVENT FUNCTIONS %%%%%%%%
 *  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 */

/**
 * @brief Split BST to two (<=key), (>key)
 *
 * @param R  BST root
 * @param K  Key Value
 * @param T1 BST (<=key)
 * @param T2 BST (>key)
 *
 */
void SplitShopLoop(struct Shop *R, int K, struct Shop **T1,
                    struct Shop **T2)
{

    struct Shop *D1=NULL, *D2=NULL;
    struct Shop *q=R, *q1=NULL, *q2=NULL, *p=NULL;


    while((q!=shopSentinel)&&(q!=NULL)){
        if(K < q->type){
            p = q->lc;
            q->lc = NULL;
            if(D2==NULL){
                D2 = q;
                q2 = q;
            } else {
                q2->lc = q;
                q2 = q;
            }
        } else {
            p = q->rc;
            q->rc = NULL;
            if(D1==NULL){
                D1 = q;
                q1 = q;
            } else {
                q1->rc = q;
                q1 = q;
            }
        }
        q = p;
    }
    *T1 = D1;
    *T2 = D2;
    return;
}

/**
 * @brief InOrder traversal shops
 *        prints shop products
 *
 */
void inOrderShopsNoSentinel(struct Shop *root)
{
    struct Shop *tempS = root;
	struct Product *tempP;

    if((tempS==NULL)||(tempS==shopSentinel))
        return;

    inOrderShopsNoSentinel(tempS->lc);
    printf("\tShop%d = ", tempS->sid);
    for(tempP=tempS->products;tempP!=NULL;tempP=tempP->next){
        printf("<%d,%d,%d> ",
            tempP->pid, tempP->qty, tempP->price);
		}
    printf("\n");
    inOrderShopsNoSentinel(tempS->rc);

    return;
}

/**
 * @brief Categorize shops
 *
 * @return 1 on success
 *         0 on failure
 */
int CategorizeShops (void)
{
    struct Shop *D1=NULL,*D2=NULL,*D3=NULL,
                *D4=NULL,*D5=NULL,*DD=NULL;

    SplitShopLoop(shopRoot, 1, &D1, &DD);
    SplitShopLoop(DD, 2, &D2, &DD);
    SplitShopLoop(DD, 3, &D3, &DD);
    SplitShopLoop(DD, 4, &D4, &D5);


    /*Print Output*/
    printf("\n Type1\n");
    inOrderShopsNoSentinel(D1);
    printf("\n Type2\n");
    inOrderShopsNoSentinel(D2);
    printf("\n Type3\n");
    inOrderShopsNoSentinel(D3);
    printf("\n Type4\n");
    inOrderShopsNoSentinel(D4);
    printf("\n Type5\n");
    inOrderShopsNoSentinel(D5);
    printf("\nDONE\n");

	return 1;
}

/*
 *  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 */

/**
 * @brief InOrder traversal shops
 *        prints shop products
 *
 */
void inOrderShops(struct Shop *root)
{
    struct Shop *tempS = root;
	struct Product *tempP;

    if(tempS==shopSentinel)
        return;

    inOrderShops(tempS->lc);
    printf("\tShop%d = ", tempS->sid);
    for(tempP=tempS->products;tempP!=NULL;tempP=tempP->next){
        printf("<%d,%d,%d> ",
            tempP->pid, tempP->qty, tempP->price);
		}
    printf("\n");
    inOrderShops(tempS->rc);

    return;
}

/**
 * @brief InOrder traversal and
 *        prints C_Products
 *
 */
void inOrderCProductsPrint(struct C_Product *root)
{
    if(!root)
        return;

    inOrderCProductsPrint(root->lc);
    printf("<%d,%d> ", root->pid, root->qty);
    inOrderCProductsPrint(root->rc);
    return;
}

/**
 * @brief Print shops
 *
 * @return 1 on success
 *         0 on failure
 */
int PrintShops (void)
{
	inOrderShops(shopRoot);
	printf("\nDONE\n");
	return 1;
}

/**
 * @brief Print customers
 *
 * @return 1 on success
 *         0 on failure
 */
int PrintCustomers (void)
{
	int i;

	for(i=0;i<m;i++){
        if((customers_hash[i]!=NULL)&&
           (customers_hash[i]->cid!= (-1))){
            printf("\tCustomer%d = ", customers_hash[i]->cid);
            inOrderCProductsPrint(customers_hash[i]->shopping_tree);
            printf("\n");
        }
	}
	printf("\nDONE\n");
	return 1;
}

/**
 * @brief Free resources
 *
 * @return 1 on success
 *         0 on failure
 */
int FreeAll(void)
{
	return 1;
}


/**
 * @brief The main function
 *
 * @param argc Number of arguments
 * @param argv Argument vector
 *
 * @return 0 on success
 *         1 on failure
 */
int main(int argc, char** argv)
{
	FILE *fin = NULL;
	char buff[BUFFER_SIZE], event;

	/* Check command buff arguments */
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <input_file> \n", argv[0]);
		return EXIT_FAILURE;
	}

	/* Open input file */
	if (( fin = fopen(argv[1], "r") ) == NULL ) {
		fprintf(stderr, "\n Could not open file: %s\n", argv[1]);
		perror("Opening test file\n");
		return EXIT_FAILURE;
	}

	/* Initializations */
	Initialize();

	/* Read input file buff-by-buff and handle the events */
	while (fgets(buff, BUFFER_SIZE, fin)) {

		DPRINT("\n>>> Event: %s", buff);

		switch(buff[0]) {

			/* Comment */
			case '#':
				break;

			/* dummy event '0'. Get the total number of customers */
			case '0':
				{
					sscanf(buff, "%c %u", &event, &max_customers_g);
					DPRINT("%c MAX CUSTOMERS: %u\n", event, max_customers_g);
					break;
				}

				/* Register shop
				 * R <sid> <type> */
			case 'R':
				{
					int sid, type;

					sscanf(buff, "%c %d %d", &event, &sid, &type);
					DPRINT("%c %d %d\n", event, sid, type);

					if (RegisterShop(sid, type)) {
						DPRINT("%c %d %d succeeded\n", event, sid, type);
					} else {
						fprintf(stderr, "%c %d %d failed\n", event, sid, type);
					}

					break;
				}

				/* Supply shop
				 * S <sid> <pid> <quantity> <price> */
			case 'S':
				{
					int sid, pid, quantity, price;

					sscanf(buff, "%c %d %d %d %d", &event, &sid, &pid, &quantity, &price);
					DPRINT("%c %d %d %d %d\n", event, sid, pid, quantity, price);

					if (SupplyShop(sid, pid, quantity, price)) {
						DPRINT("%c %d %d %d %d succeeded\n", event, sid, pid, quantity, price);
					} else {
						fprintf(stderr, "%c %d %d %d %d failed\n", event, sid, pid, quantity, price);
					}

					break;
				}

				/* Register customer
				 * C <cid> */
			case 'C':
				{
					int cid;

					sscanf(buff, "%c %d", &event, &cid);
					DPRINT("%c %d\n", event, cid);

					if (RegisterCustomer(cid)) {
						DPRINT("%c %d succeeded\n", event, cid);
					} else {
						fprintf(stderr, "%c %d failed\n", event, cid);
					}

					break;
				}

				/* Add to shopping list
				 * L <cid> <pid> <quantity> */
			case 'L':
				{
					int cid, pid, quantity;

					sscanf(buff, "%c %d %d %d", &event, &cid, &pid, &quantity);
					DPRINT("%c %d %d %d\n", event, cid, pid, quantity);

					if (AddToShoppingList(cid, pid, quantity)) {
						DPRINT("%c %d %d %d succeeded\n", event, cid, pid, quantity);
					} else {
						fprintf(stderr, "%c %d %d %d failed\n", event, cid, pid, quantity);
					}

					break;
				}

				/* Delete from shopping list
				 * D <cid> <pid> */
			case 'D':
				{
					int cid, pid;

					sscanf(buff, "%c %d %d", &event, &cid, &pid);
					DPRINT("%c %d %d\n", event, cid, pid);

					if (DeleteFromShoppingList(cid, pid)) {
						DPRINT("%c %d %d succeeded\n", event, cid, pid);
					} else {
						fprintf(stderr, "%c %d %d failed\n", event, cid, pid);
					}

					break;
				}

				/* Go shopping
				 * G <cid> */
			case 'G':
				{
					int cid;

					sscanf(buff, "%c %d", &event, &cid);
					DPRINT("%c %d\n", event, cid);

					if (GoShopping(cid)) {
						DPRINT("%c %d succeeded\n", event, cid);
					} else {
						fprintf(stderr, "%c %d failed\n", event, cid);
					}

					break;
				}

				/* Shop together
				 * M <cid1> <cid2> */
			case 'M':
				{
					int cid1, cid2;

					sscanf(buff, "%c %d %d", &event, &cid1, &cid2);
					DPRINT("%c %d %d\n", event, cid1, cid2);

					if (ShopTogether(cid1, cid2)) {
						DPRINT("%c %d %d succeeded\n", event, cid1, cid2);
					} else {
						fprintf(stderr, "%c %d %d failed\n", event, cid1, cid2);
					}

					break;
				}

				/* Categorize shops
				 * T */
			case 'T':
				{
					sscanf(buff, "%c", &event);
					DPRINT("%c\n", event);

					if (CategorizeShops()) {
						DPRINT("%c succeeded\n", event);
					} else {
						fprintf(stderr, "%c failed\n", event);
					}

					break;
				}

				/* Print shops
				 * X */
			case 'X':
				{
					sscanf(buff, "%c", &event);
					DPRINT("%c\n", event);

					if (PrintShops()) {
						DPRINT("%c succeeded\n", event);
					} else {
						fprintf(stderr, "%c failed\n", event);
					}

					break;
				}

				/* Print customers
				 * Y */
			case 'Y':
				{
					sscanf(buff, "%c", &event);
					DPRINT("%c\n", event);

					if (PrintCustomers()) {
						DPRINT("%c succeeded\n", event);
					} else {
						fprintf(stderr, "%c failed\n", event);
					}

					break;
				}

				/* Empty line */
			case '\n':
				break;

				/* Ignore everything else */
			default:
				DPRINT("Ignoring buff: %s \n", buff);

				break;
		}
	}

	FreeAll();
	return (EXIT_SUCCESS);
}

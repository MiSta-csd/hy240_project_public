/*************************************************************
 * @file   main.c                                            *
 * @author Nikolaos Batsaras <batsaras@csd.uoc.gr>  	     *
 *                                                    	     *
 * @brief  Source file for the needs of CS-240b project 2018 *
 ************************************************************/

/*
 *  Minos Stavrakakis - csd4120
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
struct Product tp = { 0,0,999999999,NULL };

/**
 * @brief Optional function to initialize data structures that
 *        need initialization
 *
 * @return 1 on success
 *         0 on failure
 */
int Initialize (void)
{
    unsigned int i;
	/*Array Initialization*/
	for(i=0;i<5;i++){
        CheapestProducts[i]=&tp;
	}

	/* Initialize Shop List*/
	H = (struct Shop*) malloc(sizeof(struct Shop));
	H->sid = -1;
	H->type = -1;
	H->products = NULL;
	H->next = H;
	H->prev = H;

	/* Initialize Customer List */
	Sentinel = (struct Customer*) malloc(
							sizeof(struct Customer));
	Sentinel->cid = -1;
	Sentinel->shopping_list = NULL;
	Sentinel->next = NULL;
	Customers = Sentinel;

	return 1;
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
	struct Shop *tempS = H->next;
	/*Using Header as a Sentinel*/
	H->sid = sid;

	while(tempS->sid!=sid)
		tempS = tempS->next;

	if(tempS==H){
		/*Return def value to Header->sid*/
		H->sid = -1;
		return NULL;
	}
	else{
        /*Return def value to Header->sid*/
        H->sid = -1;
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
	struct Shop *tempS = H->next;
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
	struct Shop *newShop;

	if(SearchShop(sid))
		return 0;

	newShop = (struct Shop*) malloc(sizeof(struct Shop));

	if(!newShop) /* malloc success check */
		return 0;

	/* No Shops in list case */
	if(H->prev==H){
		newShop->sid = sid;
		newShop->type = type;
		newShop->products = NULL;
		newShop->next = H;
		newShop->prev = H;

		H->next = newShop;
		H->prev = newShop;
	} else {
		newShop->sid = sid;
		newShop->type = type;
		newShop->products = NULL;
		newShop->next = H->next;
		H->next->prev = newShop;
		newShop->prev = H;
		H->next = newShop;
	}

	printf("\tShops = ");
	for(newShop=H->next;newShop!=H;newShop=newShop->next){
		printf("<%d,%d> ", newShop->sid,newShop->type);
	}
	printf("\nDONE\n");
	return 1;
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
		tempP->quantity += quantity;
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
	newPro->quantity = quantity;
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

/**
 * @brief Search Customer
 *
 * @param cid       The shop's id
 *
 * @return struct Customer* on success
 *         NULL 		   on failure
 */
struct Customer* SearchCustomer(int cid)
{
	struct Customer *tempC = Customers;
	Sentinel->cid = cid;

	while(tempC->cid!=cid)
		tempC = tempC->next;

	if(tempC==Sentinel)
		return NULL;
	else
		return tempC;
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
	struct Customer *tempC = Customers;

	if(SearchCustomer(cid))
		return 0;

	tempC = (struct Customer*) malloc(sizeof(struct Customer));
	tempC->cid = cid;
	tempC->shopping_list = NULL;

	if(Customers==Sentinel){
		Customers = tempC;
		tempC->next = Sentinel;
	} else {
		tempC->next = Customers;
		Customers = tempC;

	}
	printf("\tCustomers = ");
	for(tempC=Customers;tempC!=Sentinel;tempC=tempC->next){
		printf("<%d> ", tempC->cid);
	}
	printf("\nDONE\n");
	return 1;
}

/**
 * @brief Search C_Product
 *
 * @param cid       The Customer's id
 * @param pid       The C_Product's id
 *
 * @return struct C_Product* on success
 *         NULL 		   	 on failure
 */
struct C_Product* SearchC_Product(int cid, int pid)
{
	struct Customer *tempC = Customers;
	struct C_Product *tempCP;

	tempC = SearchCustomer(cid);
	if(!tempC)
		return NULL;

	tempCP = tempC->shopping_list;
	while((tempCP != NULL)&&(tempCP->pid != pid))
		tempCP = tempCP->next;

	if(tempCP)
		return tempCP;
	else
		return NULL;
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
	struct C_Product *prevCP;
	struct C_Product *newCP;

	if(!(tempC=SearchCustomer(cid)))
		return 0;

	if((tempCP=SearchC_Product(cid, pid))){
		tempCP->quantity += quantity;
		PrintCustomers();
		return 1;
	}

	newCP = (struct C_Product*) malloc(sizeof(struct C_Product));
	newCP->pid = pid;
	newCP->quantity = quantity;
	if(!newCP)
		return 0;

    /*Case Shopping list is empty*/
	if(tempC->shopping_list == NULL){
		tempC->shopping_list = newCP;
		newCP->next = NULL;
	} else {
		tempCP = tempC->shopping_list;
		while((tempCP != NULL)&&(tempCP->pid < pid)){
			prevCP = tempCP;
			tempCP = tempCP->next;
		}
		/*Case that prd has to be added 1st*/
		if(tempCP==tempC->shopping_list){
            newCP->next = tempCP;
            tempC->shopping_list = newCP;
		} else {
            newCP->next = tempCP;
            prevCP->next = newCP;
		}
	}
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
int DeleteFromShoppingList (int cid, int pid)
{
	struct Customer *tempC;
	struct C_Product *tempCP;
	struct C_Product *prevCP;

	if(!(tempC=SearchCustomer(cid))){
		PrintCustomers();
		return 0;
	}

	tempCP = tempC->shopping_list;
	while(tempCP!=NULL){
		prevCP = tempCP;
		tempCP = tempCP->next;
		if(prevCP->pid==pid){
			tempC->shopping_list = tempCP;
			free(prevCP);
			PrintCustomers();
			return 1;
		}
		if(tempCP&&(tempCP->pid==pid)){
			prevCP->next = tempCP->next;
			free(tempCP);
			PrintCustomers();
			return 1;
		}
	}
	PrintCustomers();
	return 0;
}

/**
 * @brief Delete from shopping list. NOT PRINT
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
	struct C_Product *prevCP;

	if(!(tempC=SearchCustomer(cid))){
		PrintCustomers();
		return 0;
	}

	tempCP = tempC->shopping_list;
	while(tempCP!=NULL){
		prevCP = tempCP;
		tempCP = tempCP->next;
		if(prevCP->pid==pid){
			tempC->shopping_list = tempCP;
			free(prevCP);
			return 1;
		}
		if(tempCP&&(tempCP->pid==pid)){
			prevCP->next = tempCP->next;
			free(tempCP);
			return 1;
		}
	}
	PrintCustomers();
	return 0;
}

/**
 * @brief Delete from shop product list
 *
 * @param cid  The shops's id
 * @param pid  The product's id
 *
 * @return 1 on success
 *         0 on failure
 */
int DeleteFromShopProductList (int sid, int pid)
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
			PrintShops();
			return 1;
		}
		if(tempP&&(tempP->pid==pid)){
			prevP->next = tempP->next;
			free(tempP);
			PrintShops();
			return 1;
		}
	}
	return 0;
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
 * @brief Go shopping
 *
 * @param cid The customer's id
 *
 * @return 1 on success
 *         0 on failure
 */
int GoShopping (int cid)
{
	struct Shop *tempS;
	struct Customer *tempC;
	struct Product *tempP,*prevP;
	struct C_Product *tempCP,*prevCP;
	int SPdel=1,CPdel=1;

	if(!(tempC=SearchCustomer(cid)))
		return 0;

	/*Traverse through each shop*/
	for(tempS=H->next; tempS!=H; tempS=tempS->next){
		/*Traverse through each product in the shoppinglist*/
		tempCP=tempC->shopping_list;
		tempP=tempS->products;
		while(tempCP&&tempP){

                if(tempP->pid > tempCP->pid){
                    tempCP = tempCP->next;


                } else if(tempP->pid < tempCP->pid){
                    tempP = tempP->next;


				}else if(tempP->pid == tempCP->pid){
					/*decrease products (shoplist & customerlist
					until either gets depleted first*/
					while((tempP->quantity)&&(tempCP->quantity)){
						tempP->quantity--;
						tempCP->quantity--;
					}
					/* check if shop product stock depleted and
					if so delete from shop catalogue,
					store deletion status in var SPdel*/
					if(tempP->quantity==0){
                        /*Progress both pointers*/
                        prevP = tempP;
                        tempP = tempP->next;
						SPdel = DeleteFromShopProductListNP(
							tempS->sid, prevP->pid);
					}
					/* check if customer product needs "repleted" and
					if so delete from shoppinglist,
					store deletion status in var CPdel*/
					if(tempCP->quantity==0){
                        /*Progress both pointers*/
                        prevCP = tempCP;
                        tempCP = tempCP->next;
						CPdel = DeleteFromShoppingListNP(
							tempC->cid, prevCP->pid);
					}
                    /*if depleted product fails to delete exit w 0*/
                    if(!(SPdel) || !(CPdel))
                        return 0;

				}
        }
    }


	PrintCustomers();
	PrintShops();
	return 1;
}

/**
 * @brief Store close
 *
 * @param sid1  The id of the acquisitor shop
 * @param sid2  The id of the closing shop
 *
 * @return 1 on success
 *         0 on failure
 */
int StoreClose (int sid1, int sid2)
{
    struct Shop *s1;
    struct Shop *s2;
    struct Product *s1p;
    struct Product *prevs1p;
    struct Product *s2p;
    struct Product *prevs2p;

    if(!(s1 = SearchShop(sid1)))
        return 0;

    if(!(s2 = SearchShop(sid2)))
        return 0;

    s1p = s1->products;
    s2p = s2->products;

    if(!s1p){
        s1->products = s1->products;
        s2->next->prev = s2->prev;
        s2->prev->next = s2->next;
        free(s2);
        PrintShops();
        return 1;
    }

    prevs1p = s1p;
    while(s2p){
        /**/
        if(s1p->pid > s2p->pid){

            if(s1p==s1->products){
                s1->products = s2p;
                s2p = s2p->next;
                s1->products->next = s1p;
                prevs1p = s1->products;

            }else {
                prevs1p->next = s2p;
                s2p = s2p->next;
                prevs1p->next->next = s1p;
                prevs1p = prevs1p->next;

            }

        } else if(s1p->pid == s2p->pid){
            s1p->quantity += s2p->quantity;
            prevs2p = s2p;
            s2p = s2p->next;
            /*free assimilated product node*/
            free(prevs2p);

        } else if(s1p->pid < s2p->pid) {
            prevs1p = s1p;
            s1p = s1p->next;

        }
        /*if no more products on shop1 list add rest
        of shop2 products on tail*/
        if(!s1p){
            prevs1p->next = s2p;
            break;
        }
    }

    /* free Shop node */
    s2->next->prev = s2->prev;
    s2->prev->next = s2->next;
    free(s2);
    PrintShops();

	return 1;
}

/**
 * @brief Categorize shops
 *
 * @return 1 on success
 *         0 on failure
 */
int CategorizeShops (void)
{
    struct Shop *tempS,*tempS2;
    struct Product *tempP;
    unsigned int i;

    /*ARRAY Initialization*/
    for(i=0;i<5;i++)
        ShopTypes[i]=NULL;

    tempS = H->next;
    while(tempS!=H){
        /*Store the next step in Shop list traversal*/
        tempS2 = tempS->next;
        /*shop.type used as array index*/
        tempS->next = ShopTypes[tempS->type];
        ShopTypes[tempS->type] = tempS;
        /*Progress Shop traversing pointer*/
        tempS = tempS2;
    }

    /*Print shit*/
     for(i=0;i<5;i++){
        tempS = ShopTypes[i];
        printf(" Type%d\n",i);
        while(tempS){
            printf("\n\tShop%d = ",tempS->sid);
            tempP = tempS->products;
            while(tempP){
                printf("<%d,%d,%d> ", tempP->pid,
                       tempP->quantity, tempP->price);
                tempP = tempP->next;
            }
            tempS = tempS->next;
            printf("\n");
        }
     }
    printf("\nDONE\n");
	return 1;
}

/**
 * @brief Sort cheapest products array
 *
 * @return 1 on success
 *         0 on failure
 */
int SortArray(void)
{
    struct Product *temp;
    unsigned int i,j;

    for(i=0;i<5;i++){
        for(j=i+1;j<5;j++){
            if(CheapestProducts[i]->price >
               CheapestProducts[j]->price){

                temp = CheapestProducts[i];
                CheapestProducts[i] = CheapestProducts[j];
                CheapestProducts[j] = temp;
            }
        }
    }



    return 1;
}

/**
 * @brief Find cheapest products
 *
 * @return 1 on success
 *         0 on failure
 */
int FindCheapestProducts (void)
{
	struct Shop *tempS = H->next;
	struct Product *tempP;
	unsigned int i;

	while(tempS!=H){
        tempP = tempS->products;
        while(tempP){

            if(tempP->price<CheapestProducts[4]->price){
                for(i=0;i<5;i++){
                    if(CheapestProducts[i]->pid==tempP->pid){
                        CheapestProducts[i]=tempP;
                        break; /*post-Edit MLK*/
                    }
                }
                if(i==5)
                    CheapestProducts[4]=tempP;
                SortArray();
            }

            tempP = tempP->next;
        }
	tempS = tempS->next;
	}

	printf("\tCheapest Products: ");
	for(i=0;i<5;i++){
        printf("<%d,%d,%d> ", CheapestProducts[i]->pid,
               CheapestProducts[i]->quantity,
               CheapestProducts[i]->price);
	}
	printf("\nDONE\n");
	return 1;
}

/**
 * @brief Print shops
 *
 * @return 1 on success
 *         0 on failure
 */
int PrintShops (void)
{
	struct Shop *tempS;
	struct Product *tempP;

	for(tempS=H->next;tempS!=H;tempS=tempS->next){
		printf("\tShop%d = ", tempS->sid);
		for(tempP=tempS->products;tempP!=NULL;tempP=tempP->next){
			printf("<%d,%d,%d> ",
				tempP->pid, tempP->quantity, tempP->price);
		}
        printf("\n");
	}
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
	struct Customer *tempC;
	struct C_Product *tempCP;

	for(tempC=Customers;tempC!=Sentinel;tempC=tempC->next){
		printf("\tCustomer%d = ", tempC->cid);
		for(tempCP=tempC->shopping_list;tempCP!=NULL;
								tempCP=tempCP->next){
			printf("<%d,%d> ", tempCP->pid, tempCP->quantity);
		}
        printf("\n");
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
    struct Shop *tempS,*prevS;
	struct Product *tempP,*prevP;
	struct Customer *tempC,*prevC;
	struct C_Product *tempCP,*prevCP;
	unsigned int i;

	/* free Shops */
	for(i=0;i<5;i++){
        tempS=ShopTypes[i];
        while(tempS!=NULL){
            tempP=tempS->products;
            while(tempP!=NULL){
                prevP = tempP;
                tempP = tempP->next;
                free(prevP);
            }
        prevS = tempS;
        tempS = tempS->next;
        free(prevS);
        }
	}
	free(H);

	/* free Customers */
	tempC = Customers;
	while(tempC!=Sentinel){
        tempCP = tempC->shopping_list;
        while(tempCP!=NULL){
            prevCP = tempCP;
            tempCP = tempCP->next;
            free(prevCP);
        }
        prevC = tempC;
        tempC = tempC->next;
        free(prevC);
	}
	free(Sentinel);

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

				/* Store close
				 * M <sid1> <sid2> */
			case 'M':
				{
					int sid1, sid2;

					sscanf(buff, "%c %d %d", &event, &sid1, &sid2);
					DPRINT("%c %d %d\n", event, sid1, sid2);

					if (StoreClose(sid1, sid2)) {
						DPRINT("%c %d %d succeeded\n", event, sid1, sid2);
					} else {
						fprintf(stderr, "%c %d %d failed\n", event, sid1, sid2);
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

				/* Find cheapest products
				 * F */
			case 'F':
				{
					sscanf(buff, "%c", &event);
					DPRINT("%c\n", event);

					if (FindCheapestProducts()) {
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

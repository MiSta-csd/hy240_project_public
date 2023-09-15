/*************************************************************
 * @file   Shopping.h                                        *
 * @author Nikolaos Batsaras <batsaras@csd.uoc.gr>  	     *
 *                                                    	     *
 * @brief  Header file for the needs of CS-240b project 2018 *
 ************************************************************/

/*
 *  Minos Stavrakakis - csd4120
 */

#ifndef __SHOPPING_H__
#define __SHOPPING_H__


struct Shop {
	int sid;
	int type;
	struct Product *products;
	struct Shop *next;
	struct Shop *prev;
};

struct Customer {
	int cid;
	struct C_Product *shopping_list;
	struct Customer *next;
};

struct Product {
	int pid;
	int quantity;
	int price;
	struct Product *next;
};

struct C_Product {
	int pid;
	int quantity;
	struct C_Product *next;
};


/* Global, pointer to the header node of the circular, double-linked list of shops */
struct Shop *H;

/* Global, pointer to the head of the list of customers */
struct Customer *Customers;

/* Global, pointer to the sentinel node of the list of customers */
struct Customer *Sentinel;

/* Global, array of pointers for the use in event 'Categorize Stores' */
struct Shop *ShopTypes[5];

/* Global, array of pointers for the use in event 'Find Cheapest Products' */
struct Product *CheapestProducts[5];


#endif /* __SHOPPING_H__ */

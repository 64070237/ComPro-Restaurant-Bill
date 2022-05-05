#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void billheader(char name[50], char date[30], char time[30]){
	printf("\n\n");
	printf("\t    ComPro Restaurant");
	printf("\n\t   -------------------");
	printf("\nTime: %s", time);
	printf("\nDate: %s", date);
	printf("\nInvoice To: %s", name);
	printf("\n");
	printf("---------------------------------------\n");
	printf("Items\t\t");
	printf("Qty\t\t");
	printf("Total\t\t");
	printf("\n---------------------------------------");
	printf("\n\n");
}
void billbody(char item[30], int qty, float price){
	if((strlen(item))<8){
		printf("%s\t\t", item);
	}
	else if((strlen(item))>=8){
		printf("%s\t", item);
	}
	printf(" %d\t\t", qty);
	printf("%.2f\t\t", qty * price);
	printf("\n");
}

struct items{
	char item[20];
	float price;
	int qty;
};

struct orders{
	char customer[50];
	char date[50];
	char time[30];
	int numOfItems;
	int disper;
	struct items itm[50];
};

void billfooter(float total, int disper){
	printf("\n");
	float per = disper;
	float percent = per/100;
	float discount = percent * total;
	float netTotal = total - discount;
	float vat = 0.07 * total;
	float grandTotal = netTotal + vat;
	printf("---------------------------------------\n");
	printf("Sub Total\t\t\t%.2f", total);
	if(percent!=0){
		printf("\nDiscount %d%%\t\t\t%.2f", disper, discount);
	}
	else{
		printf("\nNo Discount \t\t\t%.2f", disper, discount);
	}
	printf("\n\t\t\t\t-------");
	printf("\nNet Total\t\t\t%.2f", netTotal);
	printf("\nVAT 7%%\t\t\t\t%.2f", vat);
	printf("\n---------------------------------------");
	printf("\nGrand Total\t\t\t%.2f", grandTotal);
	printf("\n---------------------------------------\n");
}

int main(){
	int option,i,n;
	struct orders ord;
	struct orders order;
	char savebill;
	char checkDis;
	char contFlag = 'y';
	char name[50];
	FILE *fp;
	
	char check = 'y';
	
	while(contFlag == 'y'){
		system("cls");
		float total = 0;
		ord.disper = 0;
		int invoiceFound = 0;
		printf("\n\t===========ComPro Restaurant===========\n");
		printf("\nPlease select your prefered operation:");
		printf("\n1. Generate Invoice");
		printf("\n2. Show all Invoice");
		printf("\n3. Search Invoice");
		printf("\n4. Exit");
		
		printf("\n\nYour choice:\t");
		scanf("%d", &option);
		fgetc(stdin);
		
		switch(option){
			case 1:
				system("cls");
				printf("\nPlease enter the name of the customer:\t");
				fgets(ord.customer,50,stdin);
				ord.customer[strlen(ord.customer)-1] = 0;
				strcpy(ord.time,__TIME__);
				strcpy(ord.date,__DATE__);
				printf("Please enter the numbers of items:\t");
				scanf("%d", &n);
				ord.numOfItems = n;
				
				for(i=0;i<n;i++){
					fgetc(stdin);
					printf("\n\n");
					printf("Please enter the item %d:\t", i+1);
					fgets(ord.itm[i].item,20,stdin);
					ord.itm[i].item[strlen(ord.itm[i].item)-1] = 0;
					printf("Please enter the quantity: \t");
					scanf("%d", &ord.itm[i].qty);
					printf("Please enter the unit price: \t");
					scanf("%f", &ord.itm[i].price);
					total += ord.itm[i].qty * ord.itm[i].price;
				}
				
				printf("\nDo you want to give a discount? [y/n]:\t");
				scanf("%s", &checkDis);
				
				if(checkDis=='y'){
					printf("Please enter percentage of a discount [0-100]:\t");
					scanf("%d", &ord.disper);
				}
				
				billheader(ord.customer, ord.date, ord.time);
				for(i=0;i<n;i++){
					billbody(ord.itm[i].item, ord.itm[i].qty, ord.itm[i].price);
				}
				billfooter(total, ord.disper);
				
				printf("\nDo you want to save the invoice? [y/n]:\t");
				scanf("%s", &savebill);
				
				if(savebill == 'y'){
					fp = fopen("RestaurantBill.dat","a+");
					fwrite(&ord,sizeof(struct orders),1,fp);
					if(fwrite != 0)
					printf("\nSuccessfully saved");
					else
					printf("\nError saving");
					fclose(fp);
				}
				break;
			case 2:
				system("cls");
				fp = fopen("RestaurantBill.dat","r");
				printf("\n    *****Your Previous Invoices*****\n");
				while(fread(&order,sizeof(struct orders),1,fp)){
					float tempTotal = 0;
					billheader(order.customer,order.date, order.time);
					for(i=0;i<order.numOfItems;i++){
						billbody(order.itm[i].item, order.itm[i].qty, order.itm[i].price);
						tempTotal += order.itm[i].qty * order.itm[i].price;
					}
					billfooter(tempTotal,order.disper);
				}
				fclose(fp);
				break;
			case 3:
				printf("Please enter the name of customer:\t");
				//fgetc(stdin);
				fgets(name, 50, stdin);
				name[strlen(name)-1] = 0;
				
				system("cls");
				fp = fopen("RestaurantBill.dat","r");
				printf("\n\t*****Invoices of %s*****\n", name);
				while(fread(&order,sizeof(struct orders),1,fp)){
					float tempTotal = 0;
					if (!strcmp(order.customer,name)){
						billheader(order.customer,order.date, order.time);
						for(i=0;i<order.numOfItems;i++){
							billbody(order.itm[i].item, order.itm[i].qty, order.itm[i].price);
							tempTotal += order.itm[i].qty * order.itm[i].price;
						}
						billfooter(tempTotal,order.disper);
						invoiceFound = 1;
					}
				}
				if(!invoiceFound){
					printf("Sorry, the invoice for %s does not exits", name);
				}
				fclose(fp);
				break;
			case 4:
				printf("\n\n\t\t Bye Bye :)\n\n");
				exit(0);
				break;
				
			default:
				printf("Sorry, invalid option");
				break;		
		}
		printf("\n\nDo you want to perform another operation? [y/n]: ");
		scanf("%s", &contFlag);
		if(contFlag == 'n'){
			printf("\n\n\t\t Bye Bye :)");
		}
	}
	printf("\n\n");
	
	return 0;
}

//Yavuz Selim Çağan 18011067
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct PGMFORMAT {//To keep informatin about an image
    int rows;
    int columns;
    int maxColor;
    int **Image;
}PGMFORMAT;

void readAnImage();//It reads image from a file.
void compressImage(PGMFORMAT*,char*);//It compresses the image.
void extractImage();//Controls and Extract a compressed image
int control(FILE*,char*);//Controls if the compressed image is correct.
void changeAllthePixels(char*);//Changes the  same pixels user wants with another pixel before extract.
void changeTheSpesificPixel(char*);//Change the pixel at the location before extract.
void histogramOfImage(char*);//Gives histogram of image before extract.
void compressedPGMProcedures(char*);//After compressing Program leads user here
void printTheImage(char*);


    int main(int argc, char **argv)
    {
        readAnImage();//Calls reading function.
        return 0;
    }


void compressedPGMProcedures(char* name) {//The methods of the compressed file.
    int x=1;
    while(x!=5) {
        printf("\nEnter the prosedure number you want to carry on:\n"//Because we have to proceed with compressed file, the code segment becomes longer.
                "1-Change the all same pixels with another\n"
                "2-Change the pixel at spesific cordinate\n"
                "3-Get histogram of Image\n"
                "4-Print the matrix\n"
                "5-Extract image into a file and exit: ");
        scanf("%d",&x);
        switch(x) {
            case 1:
                changeAllthePixels(name);
                break;
            case 2:
                changeTheSpesificPixel(name);
                break;
            case 3:
                histogramOfImage(name);
                break;
            case 4:
                printTheImage(name);
                break;
            default:
                x=5;
                extractImage(name);
                break;
        }   
    }
}

void printTheImage(char *name) {
    int column,row,maxValue,counter,value,counterin,newLine =0;
    FILE *fpr =fopen(name,"r");
    printf("P2\n");
    fscanf(fpr,"%d %d %d",&row,&column,&maxValue);
    printf("%d %d\n",column,row);
    printf("%d\n",maxValue);
    counter = row*column;
    while(counter > 0) {
        fscanf(fpr,"%d %d",&counterin,&value);
        while(counterin > 0) {
            printf("%3d ",value);
            counter--;
            counterin--;
            newLine++;
            if(newLine>=column) {
                printf("\n");
                newLine = 0;
            }
        }
    }
    fclose(fpr);
    
}
void extractImage(char *name) {
    int column,row,maxValue,counter,value,counterin,newLine =0;
    FILE *fpr =fopen(name,"r");
    if(!control(fpr,name)) {
        FILE *fpw = fopen("test_decoded.pgm","w");
        
        fprintf(fpw,"P2\n");
        fscanf(fpr,"%d %d %d",&row,&column,&maxValue);
        fprintf(fpw,"%d %d\n",column,row);
        fprintf(fpw,"%d\n",maxValue);
        
        
        counter = row*column;
        while(counter > 0) {
            fscanf(fpr,"%d %d",&counterin,&value);
            while(counterin > 0) {
                fprintf(fpw,"%d ",value);
                counter--;
                counterin--;
                newLine++;
                if(newLine>=column) {
                    fprintf(fpw,"\n");
                    newLine = 0;
                }
            }
        }
        fclose(fpw);
    }
    fclose(fpr);
}


void readAnImage() {//First Procedure is reading an Image
    PGMFORMAT* image = (PGMFORMAT*)malloc(sizeof(PGMFORMAT));//To keep the informatin about image.
    FILE *fp;
    int i,j;
    char comment[255];//Comment lines....
    char format[5];//To read format...
    char name[50];//File name...
    printf("The procedure for compressing is quick but printing it to screen can be long.Please wait\n");
    printf("Enter the file name of the image (Example:Image.pgm)");//Asks the name of file
    scanf(" %[^\n]",name);
    fp = fopen(name,"r");
    if(fp == NULL) {//Checks if there is the file 
        printf("ERROR: THE NAME OF FILE IS WRONG!!!");
        return;
    }
    fscanf(fp,"%s",format);
    if(strcmp(format,"P2")) { //Checks format of the image
       printf("UNREADABLE FORMAT!!!"); 
    }
    else {
        fscanf (fp, "%s", comment);//It reads comment lines in image file.
        while (comment[0] == '#') {
            fgets (comment, 255, fp);
            fscanf (fp, "%s", comment);
        }
        sscanf (comment, "%d", &image->columns);//Gets column number
        fscanf(fp,"%d ",&image->rows);//Gets row number
        fscanf(fp,"%d",&image->maxColor);//Gets max color the image can take
        image->Image = (int**)malloc(image->rows*sizeof(int*));//Allocate the image
        for(i=0;i<image->rows;i++) {
            image->Image[i] = (int*)malloc(image->columns*sizeof(int));
            for(j=0;j<image->columns;j++) {
                fscanf(fp,"%d",&image->Image[i][j]);//Gets image to matris
            }
        }
    }


    compressImage(image,"test_encoded.txt");//Call compression function
    
    free(image);
    fclose(fp);
    
    //The procedure with the compressed file starts here.
    compressedPGMProcedures("test_encoded.txt");//New procedure starts!!!!
    free(name);
}

void compressImage(PGMFORMAT *image,char* name) {
    FILE *fp;
    int i,j;
    int counter,value;

    fp=fopen(name,"w");
    printf("THE COMPRESSED FILE:\n");
    fprintf(fp,"%d %d %d ",image->rows,image->columns,image->maxColor);
    printf("%d %d %d ",image->rows,image->columns,image->maxColor);
    value = image->Image[0][0];
    counter = 0;
    for(i=0;i<image->rows;i++) {
        for(j=0;j<image->columns;j++) {
            if(image->Image[i][j] == value) //If the pixel is same with previous It adds 1 everytime.
                counter++;
            else {
                fprintf(fp,"%d %d ",counter,value);//When the pixel is different It writes the file and continue.
                printf("%d %d ",counter,value);
                value = image->Image[i][j];
                counter = 1;
            }
        }
    }
    fprintf(fp,"%d %d",counter,value);//This is for the last pixels. And I delete the space at the end of file.
    printf("%d %d\n",counter,value);
    fclose(fp);
}


void histogramOfImage(char *name) {
    int maxColor,columns,rows,pixelCount,counterin,value,i;
    FILE *fp = fopen(name,"r");
    fscanf(fp,"%d %d %d",&rows,&columns,&maxColor);
    int *histogram = (int*)calloc(maxColor+1,sizeof(int));

    pixelCount = rows*columns;
    
    while(pixelCount>0) {
        fscanf(fp,"%d %d",&counterin,&value);
        histogram[value]+=counterin; 
        pixelCount-=counterin;
    }
    for(i=0;i<=maxColor;i++) {
        if(histogram[i] != 0)
            printf("%d rengi %d, ",i,histogram[i]);
    }
    printf("defa gecmistir.");
    free(histogram);    
    fclose(fp);
}

void changeTheSpesificPixel(char *name) {
    FILE *fp = fopen(name,"r");
    int rows,columns,maxColor,size=0,i;
    int flag;
    int CordinateX,CordinateY,newValue,*arr,value,Cordinate,counter,index,count;
    printf("The cordinates of value you want to change:(EX:2 5) ");
    scanf("%d %d",&CordinateX,&CordinateY);
    printf("The value you want to add instead: ");
    scanf("%d",&newValue);
    
    fscanf(fp,"%d %d %d",&rows,&columns,&maxColor);
    
    while(!feof(fp)) {//Finds the length of the compressed image file.
        fscanf(fp,"%d",&i);
        size++;
    }
    rewind(fp);
    fscanf(fp,"%d %d %d",&rows,&columns,&maxColor);//To go further
    
    arr = (int*)calloc(size+10,sizeof(int));//Array with line lenght of the file//It is the maximum length file can be,
    arr[0]=rows; 
    arr[1]=columns;
    arr[2] = maxColor;
    
    Cordinate = (CordinateX*columns+CordinateY+1);//The order of the pixel user wants to change
    counter  = 0;//Total count
    index = 3 ;
    flag = 0;
    
    if(CordinateX < rows && CordinateY <columns) 
        flag =1;//After changing, It stops controlling the values and put in array
    else 
        printf("WARNING: WRONG CORDINATE IS ENTERED!!!");
            
    while(flag) {
        fscanf(fp,"%d %d",&count,&value);
        if(counter+count < Cordinate) {//It controls if the program reaches the total count
            arr[index] = count;
            arr[index+1] = value;
            index +=2;
            counter +=count;//Keep counting
        }
        else if(value == newValue) {
            flag =0;
            arr[index] = count;
            arr[index+1] = value;
            index +=2;
        }//Doğru çalışıyor:)
        else if(count == 1) {//If the count is 1, we should check both right and left of array.
            if(index == 3) {//If it is at the beginning
                arr[index] = 1;
                arr[index+1] = newValue;
                fscanf(fp,"%d %d",&count,&value);
                if(newValue == value)//If the next value is same with newOne
                    arr[index]+=count;
                else {
                    index+=2;
                    arr[index] = count;
                    arr[index+1] = value;
                }
                index+=2;
            }
            else if(counter+count == (rows*columns)) {//If it is at the end of array
                if(arr[index-1] == newValue)
                    arr[index-2]+=1;//If the previous value is same with newOne
                else {
                    arr[index] = 1;
                    arr[index+1] = newValue;
                }
            }
            else {//If it is somewhere else//We should check right and left
                if(arr[index-1] == newValue)//It is for left
                    arr[index-2]+=1;
                else {
                    arr[index] = 1;
                    arr[index+1] = newValue;
                    index +=2;
                }
                fscanf(fp,"%d %d",&count,&value);
                if(value == newValue) {//It if for right. There may be 3 pixel same next to each other. So we keep adding them each other.
                    arr[index-2]+=count;
                }
                else {
                    arr[index] = count;
                    arr[index+1] = value;
                    index+=2;
                }
            }
            flag=0;
        }//Tek elemanlı pixel dizileri doğru çalışıyor.:)
        else {
            if(counter+1 == Cordinate) {//If it is start of the pixel group.
                if(index == 3) {//It control if it start of the image. Because If it is, Program won't try to look if the previous pixel is the same with new one.
                    arr[index] = 1;
                    arr[index+1] = newValue;
                    index +=2;
                }
                else {//If not
                    if(arr[index-1] == newValue)//It checks if previous pixel is the same value
                        arr[index-2]+=1;
                    else {
                        arr[index+1] = newValue;
                        arr[index] = 1;
                        index +=2;
                    }
                }
                arr[index] = count-1;
                arr[index+1] = value;
                index +=2;
            }
            else if(counter+count == Cordinate ||counter+count == (rows*columns) ) {//If it is end of the pixel group.
                arr[index] = count-1;
                arr[index+1] = value;
                index +=2;
                arr[index] = 1;
                arr[index+1] = newValue;
                index+=2;
                if(counter+count != (rows*columns)) {//If it is at the end of array
                    fscanf(fp,"%d %d",&count,&value);
                    if(arr[index-1] == value) {//If the next pixel is same with new one It adds theme.
                        arr[index-2]+=count;
                    }
                    else {
                        arr[index] = count;
                        arr[index+1] = value;
                        index+=2;
                    }
                }
            }
            else { // If it is between start and end...
                arr[index] = Cordinate-counter-1;
                arr[index+1] = value;
                index +=2;
                arr[index] = 1;
                arr[index+1] = newValue;
                index +=2;
                arr[index] = count+counter-Cordinate;
                arr[index+1] = value;
                index +=2;
            }
            flag = 0;
        }
    }
    while(!feof(fp)) {
        fscanf(fp,"%d %d",&count,&value);
        arr[index] = count;
        arr[index+1] = value;
        index +=2;
    }
    
    fclose(fp);
    //Starts writing new file.
    
    FILE *fp2 = fopen(name,"w");
    
    fprintf(fp2,"%d %d %d ",rows,columns,maxColor);
    printf("%d %d %d ",rows,columns,maxColor);
    fprintf(fp2,"%d %d ",arr[3],arr[4]);
    printf("%d %d ",arr[3],arr[4]);
    
    i=5;
    while(arr[i+2]!=0) {
        fprintf(fp2,"%d %d ",arr[i],arr[i+1]);
        printf("%d %d ",arr[i],arr[i+1]);
        i+=2;
    }
    fprintf(fp2,"%d %d",arr[i],arr[i+1]);// It also deletes the last space.
    printf("%d %d",arr[i],arr[i+1]);
    free(arr);
    fclose(fp2);
}


void changeAllthePixels(char *name) {
    int value,prevValue,prevValueIn,count;
    int i=0,size=0;
    int rows,columns,maxColor,*arr;
    FILE *fp = fopen(name,"r");
    int oldValue,newValue;
    printf("The value you want to change: ");
    scanf("%d",&oldValue);
    printf("The value you want to add instead: ");
    scanf("%d",&newValue);
    
    fscanf(fp,"%d %d %d",&rows,&columns,&maxColor);
    
    while(!feof(fp)) {//Finds the length of the compressed image file.
        fscanf(fp,"%d",&i);
        size++;
    }
    
    rewind(fp);
    fscanf(fp,"%d %d %d",&rows,&columns,&maxColor);//To go further
    arr = (int*)calloc(size+5,sizeof(int));//Array with line lenght of the file//It is the maximum length file can be
    arr[0]=rows; 
    arr[1]=columns;
    arr[2] = maxColor;

    fscanf(fp,"%d %d",&count,&value);
    
    arr[3] = count;
    if(oldValue == value) //checks if the value needto be changed
        arr[4] = newValue;
    else 
        arr[4] = value;

    prevValueIn = 4;
    prevValue = arr[4];
    
    while(!feof(fp)) {
        fscanf(fp,"%d %d",&count,&value);
        if(oldValue == value) { //checks if the value needto be changed
            if(prevValue == newValue) // checks if the previous value same with the new one and if true It directly adds count of new value to previous.
                arr[prevValueIn-1] += count;
            else {
                prevValueIn +=2; // If not It creates new place for the new value.
                prevValue = newValue;
                arr[prevValueIn-1] = count;
                arr[prevValueIn] = newValue;
            }
        }
        else {
            if(prevValue == value) // same procedure with the previous one but this time controls if the value is same with previous one
                arr[prevValueIn-1] += count;
            else {
                prevValueIn +=2;
                prevValue = value;
                arr[prevValueIn-1] = count;
                arr[prevValueIn] = value;
            }
        }
    }
    fclose(fp);
    
    //Starts writing new file.
    FILE *fp2 = fopen(name,"w");
    
    fprintf(fp2,"%d %d %d ",rows,columns,maxColor);
    printf("%d %d %d ",rows,columns,maxColor);
    fprintf(fp2,"%d %d ",arr[3],arr[4]);
    printf("%d %d ",arr[3],arr[4]);
    i=5;
    while(arr[i+2]!=0) {
        fprintf(fp2,"%d %d ",arr[i],arr[i+1]);
        printf("%d %d ",arr[i],arr[i+1]);
        i+=2;
    }
    fprintf(fp2,"%d %d",arr[i],arr[i+1]);// It also deletes the last space.
    printf("%d %d",arr[i],arr[i+1]);
    fclose(fp2);
    free(arr);
}

int control(FILE *fp,char *name) {
    int row,column,maxValue;
    int counter = 0;
    int count,value,previousValue=-1;
    fp = fopen(name,"r");
    fscanf(fp,"%d %d %d",&row,&column,&maxValue);
    
    fscanf(fp,"%d %d",&count,&value);
    if(value >= 0 && value<=maxValue) {
        previousValue=value;
        counter +=count;
        if(counter > column*row)  {
            printf("WARNING: MORE PIXELS THAN PREDICTION DETECTED");
            return -1;
        }
    }
    else {
        printf("PIXEL VALUE IS HIGHER THAN MAX");
        return -1;
    }
    
    while(!feof(fp)) {
        fscanf(fp,"%d %d",&count,&value);
        if(value >= 0 && value<=maxValue && value != previousValue) {
            previousValue=value;
            counter +=count;
            if(counter > column*row)  {
                printf("WARNING: MORE PIXELS THAN PREDICTION DETECTED");
                return -1;
            }
        } 
        else if(value>maxValue) {
            printf("PIXEL VALUE IS HIGHER THAN MAX");
            return -1;
        }
        else if(value >= 0) {
            printf("Pixel value is lower");
        }
        else {
            printf("WARNING: PUXEL VALUE IS THE SAME WITH PREVIOUS PIXEL VALUE");
            return -1;
        }
    }
    return 0;
}
